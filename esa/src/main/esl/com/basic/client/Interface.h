/*
MIT License
Copyright (c) 2019-2022 Sven Lukas

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

#ifndef ESL_COM_BASIC_CLIENT_INTERFACE_H_
#define ESL_COM_BASIC_CLIENT_INTERFACE_H_

#include <esl/com/basic/client/Request.h>
#include <esl/com/basic/client/Response.h>
#include <esl/module/Interface.h>
#include <esl/Module.h>
#include <esl/io/Input.h>
#include <esl/io/Output.h>

#include <functional>
#include <memory>

namespace esl {
namespace com {
namespace basic {
namespace client {

struct Interface : esl::module::Interface {
	/* *************************************** *
	 * definitions required for this interface *
	 * *************************************** */

	using CreateInput = std::function<esl::io::Input (const Response&)>;

	class Connection {
	public:
		Connection() = default;
		virtual ~Connection() = default;

		virtual Response send(const Request& request, esl::io::Output output, CreateInput createInput) const = 0;
		virtual Response send(const Request& request, esl::io::Output output, esl::io::Input input) const = 0;
	};

	using CreateConnection = std::unique_ptr<Connection> (*)(const Settings& settings);

	class ConnectionFactory : public object::Interface::Object {
	public:
		virtual std::unique_ptr<Connection> createConnection() const = 0;
	};

	using CreateConnectionFactory = std::unique_ptr<ConnectionFactory> (*)(const Settings& settings);

	/* ************************************ *
	 * standard API definition of interface *
	 * ************************************ */

	static inline const char* getType() {
		return "esl-com-basic-client";
	}


	/* ************************************ *
	 * extended API definition of interface *
	 * ************************************ */

	static std::unique_ptr<const esl::module::Interface> createInterface(const char* implementation, CreateConnectionFactory createConnectionFactory) {
		return std::unique_ptr<const esl::module::Interface>(new Interface(implementation, createConnectionFactory));
	}

	Interface(const char* implementation, CreateConnectionFactory aCreateConnectionFactory)
	: esl::module::Interface(esl::getModule().getId(), getType(), implementation, esl::getModule().getApiVersion()),
	  createConnectionFactory(aCreateConnectionFactory)
	{ }

	/* **************************** *
	 * start extension of interface *
	 * **************************** */
	CreateConnectionFactory createConnectionFactory;
};

} /* namespace client */
} /* namespace basic */
} /* namespace com */
} /* namespace esl */

#endif /* ESL_COM_BASIC_CLIENT_INTERFACE_H_ */
