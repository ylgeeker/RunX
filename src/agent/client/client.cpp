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

#include "agent/client/client.h"
#include "core/error/error.h"
#include "core/log/log.h"
#include "core/net/message.h"
#include "core/net/tcp_client.h"
#include "core/net/tcp_connection.h"

#include <cstdint>

Client::Client()
{
}

Client::~Client()
{
}

void Client::OnConnection(ylg::net::TCPConnection* connection)
{
    LOG_DEBUG("connected to the remote server. connection:{}", connection->ID());

    // only test
    const std::string data = "Hello YlG Request";
    ylg::net::Header  header;
    header._dataSize = data.size();
    ylg::net::Hton(header);

    ylg::net::Message msg;
    msg.Reset(header, data.data(), data.size());

    auto errcode = connection->Send(msg);
    if (!ylg::error::IsSuccess(errcode))
    {
        LOG_ERROR("can not send send to remote server. errcode:{}", errcode.value());
    }
}

void Client::OnDisconnection(ylg::net::TCPConnection* connection)
{
    LOG_DEBUG("disconnected with the remote server. connection:{}", connection->ID());
}

void Client::HandleData(ylg::net::TCPConnection* connection, const ylg::net::Message& msg)
{
    LOG_DEBUG("new message{} size:{}", msg.GetPayload(), msg.GetPayloadSize());
}

void Client::Connect(const std::string& remoteIP, uint16_t remotePort)
{
    _remoteIP   = remoteIP;
    _remotePort = remotePort;

    _client = std::make_shared<ylg::net::TCPClient>();
    _client->SetCallback(shared_from_this());
    auto errcode = _client->Connect(_remoteIP, _remotePort);
    if (!ylg::error::IsSuccess(errcode))
    {
        LOG_FATAL("can not connect the remote server. address:{}:{}", _remoteIP, _remotePort);
    }
}

void Client::Close()
{
    if (!_client)
    {
        return;
    }

    _client->Close();
}
