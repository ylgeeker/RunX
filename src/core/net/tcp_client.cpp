/**
 * MIT License
 *
 * Copyright (c) 2024 ylgeeker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "core/net/tcp_client.h"
#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/message.h"
#include "core/net/tcp_connection.h"
#include "core/net/tcp_handler.h"

#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>

#include <memory.h>
#include <string>

namespace ylg {
namespace net {

TCPClient::TCPClient()
{
}

TCPClient::~TCPClient()
{
}

void TCPClient::ReadCallback(bufferevent* bev, void* ctx)
{
    TCPConnection* connection = static_cast<TCPConnection*>(ctx);

    Message msg;
    auto    errcode = connection->Read(msg);
    if ((int)error::ErrorCode::TryAgain == errcode.value())
    {
        return;
    }

    if (!error::IsSuccess(errcode))
    {
        LOG_ERROR("failed to read data from connection:{}", connection->GetRemoteAddress());
    }

    auto handler = static_cast<TCPClient*>(connection->GetHandler());
    handler->_functor->HandleData(connection, msg);
}

void TCPClient::EventCallback(bufferevent* bev, short events, void* ctx)
{
    TCPConnection* connection = static_cast<TCPConnection*>(ctx);
    TCPClient*     client     = (TCPClient*)connection->GetHandler();

    if (events & BEV_EVENT_CONNECTED)
    {
        client->_functor->OnConnection(connection);
        client->_connections.Push(connection);
        return;
    }

    if (events & BEV_EVENT_EOF)
    {
        client->_functor->OnDisconnection(connection);
        client->_connections.Delete(connection);
        delete connection;
        return;
    }

    if (events & BEV_EVENT_ERROR)
    {
    }

    if (events & BEV_EVENT_TIMEOUT)
    {
        LOG_WARN("connection timeout. connection:{}", connection->ID());
    }
}

void TCPClient::SetCallback(TCPHandlerCallbackFunctor functor)
{
    _functor = functor;
}

std::error_code TCPClient::Connect(const std::string& ip, uint16_t port, uint32_t poolSize)
{
    _base = event_base_new();
    if (!_base)
    {
        LOG_ERROR("failed to create evnet base:{}:{}", ip, port);
        return error::ErrorCode::LibException;
    }

    _remoteIP   = ip;
    _remotePort = port;

    evutil_addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    auto             remotePort = std::to_string(_remotePort);
    evutil_addrinfo* servinfo;

    if (getaddrinfo(_remoteIP.c_str(), remotePort.c_str(), &hints, &servinfo) != 0)
    {
        event_base_free(_base);
        return error::ErrorCode::ConnectionAborted;
    }

    evutil_addrinfo* p = nullptr;
    for (p = servinfo; p != nullptr; p = p->ai_next)
    {
        _bev = bufferevent_socket_new(_base, -1, BEV_OPT_CLOSE_ON_FREE);
        if (bufferevent_socket_connect(_bev, p->ai_addr, p->ai_addrlen) < 0)
        {
            bufferevent_free(_bev);
            continue;
        }

        auto conn = new TCPConnection(-1, p->ai_addr, p->ai_addrlen);
        conn->BindHandler(_bev, this);
        bufferevent_setcb(_bev, &TCPClient::ReadCallback, nullptr, &TCPClient::EventCallback, conn);
        bufferevent_enable(_bev, EV_READ | EV_WRITE);
        break;
    }

    freeaddrinfo(servinfo);
    _asyncRun = std::async(std::launch::async, &TCPClient::Run, this);
    return error::ErrorCode::Success;
}

void TCPClient::Close()
{
    if (!_base)
    {
        return;
    }

    event_base_loopbreak(_base);
    bufferevent_free(_bev);
    _asyncRun.wait();
}

std::error_code TCPClient::Send(const Message& msg)
{
    if (_connections.Count() == 0)
    {
        return error::ErrorCode::ConnectionIsNotReady;
    }

    auto idx = _connIndex++ % _connections.Count();
    return _connections.At(idx)->Send(msg);
}

void TCPClient::Run()
{
    int exitedCode = 0;
    do {
        exitedCode = event_base_loop(_base, EVLOOP_NO_EXIT_ON_EMPTY);
    } while (exitedCode != -1);

    LOG_WARN("tcp client run exited. exited code:{}", exitedCode);
    bufferevent_free(_bev);
}

} // namespace net
} // namespace ylg

