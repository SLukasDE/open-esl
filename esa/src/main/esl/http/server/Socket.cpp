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

#include <esl/http/server/Socket.h>
#include <esl/object/Properties.h>
#include <esl/Module.h>

namespace esl {
namespace http {
namespace server {

module::Implementation& Socket::getDefault() {
	static module::Implementation implementation;
	return implementation;
}

Socket::Socket(std::uint16_t port, requesthandler::Interface::CreateInput createInput,
		std::initializer_list<std::pair<std::string, std::string>> settings,
		const std::string& implementation)
: Interface::Socket(),
  socket(esl::getModule().getInterface<Interface>(implementation).createSocket(port, createInput, object::Properties(std::move(settings))))
{ }

Socket::Socket(std::uint16_t port, requesthandler::Interface::CreateInput createInput,
		const object::Values<std::string>& settings,
		const std::string& implementation)
: Interface::Socket(),
  socket(esl::getModule().getInterface<Interface>(implementation).createSocket(port, createInput, settings))
{ }

void Socket::addTLSHost(const std::string& hostname, std::vector<unsigned char> certificate, std::vector<unsigned char> key) {
	socket->addTLSHost(hostname, certificate, key);
}

void Socket::addObjectFactory(const std::string& id, ObjectFactory objectFactory) {
	socket->addObjectFactory(id, objectFactory);
}

bool Socket::listen() {
	return socket->listen();
}

void Socket::release() {
	socket->release();
}

} /* namespace server */
} /* namespace http */
} /* namespace esl */
