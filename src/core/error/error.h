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

#ifndef _YLG_ERROR_ERROR_H_
#define _YLG_ERROR_ERROR_H_

#include <climits>
#include <cstdint>
#include <system_error>

namespace ylg {
namespace error {

enum class ErrorCode : int32_t
{
    Unknown = -2,
    Error   = -1,
    Success = 0,

    // net error code
    NetException,
    LibException,
    MemException,
    TryAgain,
    ConnectionAborted,
    ReceivedTooLarge,
    WritedException,
    ConnectionIsNotReady,
    InvalidHTTPMethod,
    RepeatedHTTPURI,
    HTTPRespondFailed,

    // application error code
    ParseConfigFailure,

    // assist error code
    Overflow,
    FileException,
    InvalidParameter,
    InvalidUser,

    // max code
    MaxValue = USHRT_MAX,
};

class ErrorCodeCategory final : public std::error_category
{
public:
    static ErrorCodeCategory& Instance();

public:
    const char* name() const noexcept override;
    std::string message(int code) const override;
};

std::error_code make_error_code(ErrorCode e);
bool            IsSuccess(const std::error_code& ec);
std::string     ToString(int ec);

} // namespace error
} // namespace ylg

namespace std {
template <>
struct is_error_code_enum<ylg::error::ErrorCode> : std::true_type
{
};

} // namespace std

#endif

