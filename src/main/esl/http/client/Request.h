/*
MIT License
Copyright (c) 2019 Sven Lukas

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

#ifndef SRC_MAIN_C___RSM_SERVICE_CLIENT_REQUEST_H_
#define SRC_MAIN_C___RSM_SERVICE_CLIENT_REQUEST_H_

#include <esl/http/client/Interface.h>
#include <string>
#include <map>

namespace esl {
namespace http {
namespace client {

//class Connection;

class Request {
friend class Interface::Connection; // fuer readCallback;
public:
	Request(const std::string& servicePath, const std::string& contentType);
	virtual ~Request() = default;

	const std::string& getServicePath() const noexcept;
	const std::string& getContentType() const noexcept;
	const std::map<std::string, std::string>& getHeaders() const noexcept;
	void addHeader(const std::string& key, const std::string& value);

private:
	std::string servicePath;
	std::string contentType;
	std::map<std::string, std::string> headers;
};

} /* namespace client */
} /* namespace http */
} /* namespace esl */

#endif /* SRC_MAIN_C___RSM_SERVICE_CLIENT_REQUEST_H_ */