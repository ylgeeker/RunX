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

#ifndef _YLG_CORE_NET_MESSAGE_H_
#define _YLG_CORE_NET_MESSAGE_H_

#include <cstdint>

namespace ylg {
namespace net {

#define MESSAGE_MAGIC 0xbeeabeaf

struct Header
{
    uint32_t _version   = 0;
    uint32_t _msgType   = 0;
    uint32_t _tag       = 0;
    uint32_t _dataSize  = 0;
    uint64_t _magic     = MESSAGE_MAGIC;
    uint64_t _sequence  = 0;
    uint64_t _timestamp = 0;
};

class Message final
{
public:
    Message() = default;
    Message(const Header& header, const char* payload, uint32_t payloadSize);
    ~Message();

public:
    enum
    {
        MESSAGE_HEADER_SIZE = sizeof(Header),
        MAX_MESSAGE_SIZE    = 1024 * 1024 * 1024

    };

public:
    void          Reset(const Header& header, const char* payload, uint32_t payloadSize);
    void          ResetHeader(const Header& header);
    void          ResetPayload(const char* data, uint32_t dataSize);
    const Header& GetHeader() const;
    const char*   GetData() const;
    uint32_t      GetDataSize() const;
    const char*   GetPayload() const;
    uint32_t      GetPayloadSize() const;

private:
    Header   _header;
    char*    _data     = nullptr; // The header is already included in the data.
    uint32_t _dataSize = 0;
};

void Hton(Header& header);
void Ntoh(Header& header);

} // namespace net
} // namespace ylg
#endif

