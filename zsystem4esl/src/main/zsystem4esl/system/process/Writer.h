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

#ifndef ZSYSTEM4ESL_SYSTEM_PROCESS_WRITER_H_
#define ZSYSTEM4ESL_SYSTEM_PROCESS_WRITER_H_

#include <esl/io/Writer.h>

#include <zsystem/process/FileDescriptor.h>

#include <string>

namespace zsystem4esl {
inline namespace v1_6 {
namespace system {
namespace process {

class Writer : public esl::io::Writer {
public:
	Writer(zsystem::process::FileDescriptor& fileDescriptor);

	std::size_t write(const void* data, std::size_t size) override;
	std::size_t getSizeWritable() const override;

private:
	zsystem::process::FileDescriptor& fileDescriptor;
};

} /* namespace process */
} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_SYSTEM_PROCESS_WRITER_H_ */
