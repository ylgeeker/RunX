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

#ifndef _YLG_SERVER_CONTROLLER_ROUTE_ROUTE_H_
#define _YLG_SERVER_CONTROLLER_ROUTE_ROUTE_H_

#include "core/net/tcp_connection.h"
#include "server/controller/route/agent_session.h"

#include "core/container/safe_map.h"

#include <system_error>

class Route final
{
public:
    Route()  = default;
    ~Route() = default;

public:
    std::error_code CreateLocalSession(ylg::net::TCPConnection* conn);
    std::error_code CreateRemoteSession(AgentSessionPtr session);
    AgentSessionPtr FindAgentSession(const std::string& agentID);
    std::error_code RemoveLocalSession(ylg::net::TCPConnection* conn);
    std::error_code RemoveAgentSession(const std::string& agentID);
    std::error_code Run();
    void            Close();

private:
    // Key: Connection ID, Value: Agent ID
    ylg::container::SafeMap<std::string, std::string> _connAgentIDs;

    // Key:AgentID, Value: Agent Session
    ylg::container::SafeMap<std::string, AgentSessionPtr> _agents;
};

using RoutePtr = std::shared_ptr<Route>;

#endif

