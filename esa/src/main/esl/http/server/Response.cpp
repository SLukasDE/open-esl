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

#include <esl/http/server/Response.h>

namespace esl {
namespace http {
namespace server {

Response::Response(unsigned short aStatusCode, const utility::MIME& contentType, std::string aRealmId) noexcept
: headers{{"Content-Type", contentType.toString()}},
  statusCode(aStatusCode),
  realmId(std::move(aRealmId))
{
//	headers["Content-Type"] = contentType.toString();
}


bool Response::isValid() const noexcept {
	return (headers.find("Content-Type")->second != "");
}

unsigned short Response::getStatusCode() const noexcept {
	return statusCode;
}

const std::string& Response::getRealmId() const noexcept {
	return realmId;
}

void Response::addHeader(const std::string& key, const std::string& value) {
	headers[key] = value;
}

const std::map<std::string, std::string>& Response::getHeaders() const {
	return headers;
}

} /* namespace server */
} /* namespace http */
} /* namespace esl */
