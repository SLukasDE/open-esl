/*
MIT License
Copyright (c) 2019-2021 Sven Lukas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ESL_UTILITY_READERBUFFERED_H_
#define ESL_UTILITY_READERBUFFERED_H_

#include <esl/utility/Reader.h>

#include <cstdint>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace esl {
namespace utility {

class ReaderBuffered : public esl::utility::Reader {
public:
	ReaderBuffered(esl::utility::Reader& baseReader);

	std::size_t read(void* data, std::size_t size) override;

	// returns available bytes to read.
	// npos is returned if available size is unknown.
	std::size_t getSizeReadable() const override;

	void setBaseReader(esl::utility::Reader& baseReader);

	void reset();

private:
	using CacheLine = std::vector<std::uint8_t>;

	std::reference_wrapper<esl::utility::Reader> baseReader;

	bool completed = false;
	std::size_t currentIndex = npos;
	std::size_t currentPos = 0;

	std::vector<std::unique_ptr<CacheLine>> buffers;
};

} /* namespace utility */
} /* namespace esl */

#endif /* ESL_UTILITY_READERBUFFERED_H_ */