/*
MIT License
Copyright (c) 2019-2023 Sven Lukas

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

#include <zsystem4esl/system/process/Reader.h>

namespace zsystem4esl {
inline namespace v1_6 {
namespace system {
namespace process {

Reader::Reader(zsystem::process::FileDescriptor& aFileDescriptor)
: fileDescriptor(aFileDescriptor)
{ }

std::size_t Reader::read(void* data, std::size_t size) {
	return fileDescriptor.read(data, size);
}

std::size_t Reader::getSizeReadable() const {
	return Reader::npos;
}

bool Reader::hasSize() const {
	return true;
}

std::size_t Reader::getSize() const {
	return fileDescriptor.getFileSize();
}

} /* namespace process */
} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace zsystem4esl */
