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

#include <zsystem/process/ConsumerFile.h>

namespace zsystem {
namespace process {

ConsumerFile::ConsumerFile(FileDescriptor aFileDescriptor)
: fileDescriptor(std::move(aFileDescriptor))
{ }

std::size_t ConsumerFile::read(FileDescriptor& fileDescriptor) {
	if(currentPos >= currentSize) {
		currentPos = 0;
		currentSize = fileDescriptor.read(buffer, sizeof(buffer));
	}

	if(currentSize == process::FileDescriptor::npos) {
		return process::FileDescriptor::npos;
	}

	std::size_t count = getFileDescriptor().write(&buffer[currentPos], currentSize - currentPos);
	if(count != process::FileDescriptor::npos) {
		currentPos += count;
	}

	return count;
}

FileDescriptor& ConsumerFile::getFileDescriptor() & {
	return fileDescriptor;
}

FileDescriptor&& ConsumerFile::getFileDescriptor() && {
	return std::move(fileDescriptor);
}

} /* namespace process */
} /* namespace zsystem */
