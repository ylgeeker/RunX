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

#ifndef _YLG_SERVER_CONTROLLER_H_
#define _YLG_SERVER_CONTROLLER_H_

#include "internal/controller_protocol.h"
#include "server/controller/processor/processor.h"
#include "server/controller/processor/register_agent_processor.h"
#include "server/controller/route/route.h"

#include "core/net/tcp_connection.h"
#include "core/net/tcp_handler.h"
#include "core/net/tcp_server.h"

#include <cstdint>
#include <future>
#include <memory>

class Controller final : public ylg::net::TCPHandlerCallback, public std::enable_shared_from_this<Controller>
{
public:
    Controller();
    ~Controller() = default;

public:
    virtual void OnConnection(ylg::net::TCPConnection* connection);
    virtual void OnDisconnection(ylg::net::TCPConnection* connection);
    virtual void HandleData(ylg::net::TCPConnection* connection, const ylg::net::Message& msg);

public:
    void RegisterProcessor(ylg::internal::MessageType type, MsgProcessorPtr processor);
    void Run(const std::string& listenIP, uint16_t listenPort);
    void Close();

private:
    std::future<void>      _asyncRun;
    std::string            _listenIP;
    uint16_t               _listenPort = 0;
    ylg::net::TCPServerPtr _server     = nullptr;
    RoutePtr               _route      = nullptr;
};

#endif

