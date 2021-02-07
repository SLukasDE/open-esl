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

#ifndef ESL_HTTP_CLIENT_REQUESTHANDLERFILE_H_
#define ESL_HTTP_CLIENT_REQUESTHANDLERFILE_H_

#include <esl/http/client/RequestHandler.h>
#include <esl/utility/MIME.h>

#include <string>
#include <fstream>
#include <memory>

namespace esl {
namespace http {
namespace client {

class RequestHandlerFile: public RequestHandler {
public:
	RequestHandlerFile(utility::MIME contentType, std::string filename);

	std::size_t producer(char* buffer, std::size_t count) override;

	bool hasSize() const override;
	std::size_t getSize() const override;
	bool isEmpty() const override;

	const std::string& getFilename() const noexcept;

private:
	std::ifstream& getFileStream() const;

	std::string filename;

	mutable std::unique_ptr<std::ifstream> file;
	mutable std::size_t size = 0;

	std::size_t pos = 0;
};

} /* namespace client */
} /* namespace http */
} /* namespace esl */

#endif /* ESL_HTTP_CLIENT_REQUESTHANDLERFILE_H_ */