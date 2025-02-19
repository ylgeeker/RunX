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

#include "server/controller/route/route.h"
#include "core/net/tcp_connection.h"
#include "server/controller/route/agent_session.h"

#include "internal/error.h"
#include <system_error>

std::error_code Route::CreateLocalSession(ylg::net::TCPConnection* conn)
{
    return ylg::internal::ErrorCode::Success;
}

std::error_code Route::CreateRemoteSession(AgentSessionPtr session)
{
    return ylg::internal::ErrorCode::Success;
}

AgentSessionPtr Route::FindAgentSession(const std::string& agentID)
{
    return nullptr;
}

std::error_code Route::RemoveLocalSession(ylg::net::TCPConnection* conn)
{
    return ylg::internal::ErrorCode::Success;
}

std::error_code Route::RemoveAgentSession(const std::string& agentID)
{
    return ylg::internal::ErrorCode::Error;
}

std::error_code Route::Run()
{
    return ylg::internal::ErrorCode::Success;
}

void Route::Close()
{
}

