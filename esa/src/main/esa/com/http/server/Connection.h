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

#ifndef ESA_COM_HTTP_SERVER_CONNECTION_H_
#define ESA_COM_HTTP_SERVER_CONNECTION_H_

#include <esa/com/http/server/Response.h>
#include <esa/io/Output.h>

#include <boost/filesystem.hpp>

#include <memory>

namespace esa {
inline namespace v1_6 {
namespace com {
namespace http {
namespace server {

class Connection {
public:
	Connection() = default;
	virtual ~Connection() = default;

	virtual bool send(const Response& response, io::Output output) = 0;
	virtual bool send(const Response& response, boost::filesystem::path path) = 0;
};

} /* namespace server */
} /* namespace http */
} /* namespace com */
} /* inline namespace v1_6 */
} /* namespace esa */

#endif /* ESA_COM_HTTP_SERVER_CONNECTION_H_ */
