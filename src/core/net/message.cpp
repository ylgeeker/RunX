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

#include "core/net/message.h"

#include <cstdint>
#include <cstring>
#include <new>

#include <arpa/inet.h>
#include <endian.h>
#include <memory.h>
#include <stdlib.h>

namespace ylg {
namespace net {

Message::Message(const Header& header, const char* payload, uint32_t payloadSize)
{
    Reset(header, payload, payloadSize);
}

Message::~Message()
{
    if (_data)
    {
        free(_data);
        _data = nullptr;
    }

    _dataSize = 0;
}

void Message::Reset(const Header& header, const char* payload, uint32_t payloadSize)
{
    ResetHeader(header);
    ResetPayload(payload, payloadSize);
}

void Message::ResetHeader(const Header& header)
{
    memcpy(&_header, &header, MESSAGE_HEADER_SIZE);
}

void Message::ResetPayload(const char* payload, uint32_t payloadSize)
{
    if (_data)
    {
        free(_data);
        _data = nullptr;
    }

    _dataSize = MESSAGE_HEADER_SIZE + payloadSize;
    _data     = (char*)malloc(sizeof(char) * _dataSize);
    if (!_data)
    {
        throw std::bad_alloc();
    }

    memcpy(_data, &_header, MESSAGE_HEADER_SIZE);
    memcpy(_data + MESSAGE_HEADER_SIZE, payload, payloadSize);
}

const Header& Message::GetHeader() const
{
    return _header;
}

const char* Message::GetData() const
{
    return _data;
}

uint32_t Message::GetDataSize() const
{
    return _dataSize;
}

const char* Message::GetPayload() const
{
    return _data + MESSAGE_HEADER_SIZE;
}

uint32_t Message::GetPayloadSize() const
{
    return _dataSize - MESSAGE_HEADER_SIZE;
}

void Hton(Header& header)
{
    header._version   = htonl(header._version);
    header._msgType   = htonl(header._msgType);
    header._tag       = htonl(header._tag);
    header._dataSize  = htonl(header._dataSize);
    header._magic     = htole64(header._magic);
    header._sequence  = htole64(header._sequence);
    header._timestamp = htole64(header._timestamp);
}

void Ntoh(Header& header)
{
    header._version   = ntohl(header._version);
    header._msgType   = ntohl(header._msgType);
    header._tag       = ntohl(header._tag);
    header._dataSize  = ntohl(header._dataSize);
    header._magic     = be64toh(header._magic);
    header._sequence  = be64toh(header._sequence);
    header._timestamp = be64toh(header._timestamp);
}

} // namespace net
} // namespace ylg

