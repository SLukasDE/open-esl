/*
MIT License
Copyright (c) 2019, 2020 Sven Lukas

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

#include <esl/http/client/Connection.h>
#include <esl/http/client/Interface.h>
#include <esl/Module.h>
#include <esl/object/ValueSettings.h>

namespace esl {
namespace http {
namespace client {

namespace {
std::string defaultImplementation;
}

Connection::Connection(const utility::URL& hostUrl,
		std::initializer_list<std::pair<std::string, std::string>> values,
		const std::string& implementation)
: Interface::Connection(),
  connection(esl::getModule().getInterface<Interface>(implementation).createConnection(hostUrl, object::ValueSettings(std::move(values))))
{ }

Connection::Connection(const utility::URL& hostUrl,
		const object::Values<std::string>& values,
		const std::string& implementation)
: Interface::Connection(),
  connection(esl::getModule().getInterface<Interface>(implementation).createConnection(hostUrl, values))
{
}

Response Connection::send(RequestDynamic& request, ResponseHandler* responseHandler) const {
	return connection->send(request, responseHandler);
}

Response Connection::send(const RequestStatic& request, ResponseHandler* responseHandler) const {
	return connection->send(request, responseHandler);
}

Response Connection::send(const RequestFile& request, ResponseHandler* responseHandler) const {
	return connection->send(request, responseHandler);
}

void Connection::setDefaultImplementation(std::string implementation) {
	defaultImplementation = std::move(implementation);
}

const std::string& Connection::getDefaultImplementation() {
	return defaultImplementation;
}

} /* namespace client */
} /* namespace http */
} /* namespace esl */
