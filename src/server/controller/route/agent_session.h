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

#ifndef _YLG_SERVER_CONTROLLER_ROUTE_AGENT_SESSION_H_
#define _YLG_SERVER_CONTROLLER_ROUTE_AGENT_SESSION_H_

#include "server/controller/route/service_session.h"

#include "core/net/tcp_connection.h"

#include <cstdint>

struct AgentSession final
{
    std::string                _agentID;
    std::string                _serviceID;
    float                      _cpuUsage         = 0.0f;
    float                      _memUsage         = 0.0f;
    uint64_t                   _createdTimestamp = 0;
    uint64_t                   _syncedTimestamp  = 0;
    ServiceSessionPtr          _serviceSession   = nullptr;
    ylg::net::TCPConnectionPtr _connection       = nullptr;
};

using AgentSessionPtr = std::shared_ptr<AgentSession>;

#endif

