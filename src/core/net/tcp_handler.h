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

#ifndef _YLG_CORE_NET_TCP_HANDLER_H_
#define _YLG_CORE_NET_TCP_HANDLER_H_

#include "core/container/safe_map.h"
#include "core/net/tcp_connection.h"

#include <event2/bufferevent.h>

#include <future>
#include <memory>

namespace ylg {
namespace net {

class TCPHandlerCallback
{
public:
    TCPHandlerCallback()          = default;
    virtual ~TCPHandlerCallback() = default;

public:
    virtual void OnConnection(TCPConnection* connection)                   = 0;
    virtual void OnDisconnection(TCPConnection* connection)                = 0;
    virtual void HandleData(TCPConnection* connection, const Message& msg) = 0;
};

using TCPHandlerCallbackFunctor = std::shared_ptr<TCPHandlerCallback>;

class TCPHandler final
{
public:
    TCPHandler();
    ~TCPHandler();

public:
    static void ReadCallback(bufferevent* bev, void* ctx);
    static void EventCallback(bufferevent* bev, short events, void* ctx);

public:
    void            SetCallback(TCPHandlerCallbackFunctor functor);
    void            BindConnection(evutil_socket_t fd, sockaddr* address, int socklen);
    std::error_code Start();
    std::error_code Stop();

private:
    void Run();

private:
    TCPHandlerCallbackFunctor _functor = nullptr;

    event_base*       _base = nullptr;
    std::future<void> _asyncRun;

    container::SafeMap<std::string, TCPConnection*> _connections;
};

using TCPHandlerPtr = std::shared_ptr<TCPHandler>;

} // namespace net
} // namespace ylg
#endif

