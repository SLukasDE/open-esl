/*
MIT License
Copyright (c) 2019-2025 Sven Lukas

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

#ifndef ESL_COM_BASIC_CLIENT_CONNECTION_H_
#define ESL_COM_BASIC_CLIENT_CONNECTION_H_

#include <esl/com/basic/client/Request.h>
#include <esl/com/basic/client/Response.h>
#include <esl/io/Input.h>
#include <esl/io/Output.h>

#include <functional>

namespace esl {
inline namespace v1_6 {
namespace com {
namespace basic {
namespace client {

class Connection {
public:
	Connection() = default;
	virtual ~Connection() = default;

	virtual Response send(const Request& request, io::Output output, std::function<io::Input (const Response&)> createInput) const = 0;
	virtual Response send(const Request& request, io::Output output, io::Input input) const = 0;
};

} /* namespace client */
} /* namespace basic */
} /* namespace com */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_COM_BASIC_CLIENT_CONNECTION_H_ */