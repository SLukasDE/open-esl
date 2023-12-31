/*
Copyright (c) 2019-2023 Sven Lukas

Logbook is distributed under BSD-style license as described in the file
LICENSE, which you should have received as part of this distribution.
*/

#ifndef LOGBOOK_STREAMBUFFER_H_
#define LOGBOOK_STREAMBUFFER_H_

#include <streambuf>
#include <vector>
#include <functional>

namespace logbook {
inline namespace v0_4 {

class StreamBuffer : public std::streambuf {
public:
	StreamBuffer(std::size_t bufferSize);

private:
    int_type overflow(int_type ch) override;
    int sync() override;

    std::size_t bufferSize;
    std::vector<char> buffer;
};

} /* inline namespace v0_4 */
} /* namespace logbook */

#endif /* LOGBOOK_STREAMBUFFER_H_ */
