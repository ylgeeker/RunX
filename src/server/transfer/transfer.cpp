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

#include "server/transfer/transfer.h"

#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/message.h"
#include "core/net/tcp_connection.h"
#include "core/net/tcp_server.h"

Transfer::Transfer()
{
}

Transfer::~Transfer()
{
}

void Transfer::OnConnection(ylg::net::TCPConnection* connection)
{
    LOG_DEBUG("new connection.{}", connection->ID());
}

void Transfer::OnDisconnection(ylg::net::TCPConnection* connection)
{
    LOG_DEBUG("connection disconnection.{}", connection->ID());
}

void Transfer::HandleData(ylg::net::TCPConnection* connection, const ylg::net::Message& msg)
{
    LOG_DEBUG("new message{} size:{}", msg.GetPayload(), msg.GetPayloadSize());

    // only test
    /*
    const std::string data = "Hello YlG Response";
    ylg::net::Header  header;
    header._dataSize = data.size();
    ylg::net::Hton(header);

    ylg::net::Message rsp;
    rsp.Reset(header, data.data(), data.size());
    auto errcode = connection->Send(rsp);
    if (!ylg::net::IsSuccess(errcode))
    {
        LOG_ERROR("can not send send to remote server. errcode:{}", errcode.value());
    }
    */
}

void Transfer::Run(const std::string& listenIP, uint16_t listenPort)
{
    _listenIP   = listenIP;
    _listenPort = listenPort;

    auto runFunctor = [&]() {
        _server = std::make_shared<ylg::net::TCPServer>(_listenIP, _listenPort);
        _server->SetCallback(shared_from_this());
        auto errcode = _server->Run();
        if (!ylg::error::IsSuccess(errcode))
        {
            LOG_FATAL("the tcp server can not be started. errcode:{}", errcode.value());
        }
    };

    _asyncRun = std::async(std::launch::async, runFunctor);
}

void Transfer::Close()
{
    auto errcode = _server->Close();
    if (!ylg::error::IsSuccess(errcode))
    {
        LOG_WARN("failed to stop tcp server. errcode:{}", errcode.value());
    }

    _asyncRun.wait();
}

