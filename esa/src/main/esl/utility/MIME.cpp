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

#include <esl/utility/MIME.h>

namespace esl {
namespace utility {

template<>
const std::string& MIME::toString(MimeType mimeType) noexcept {
	static const std::string strEmpty;
	static const std::string strTextPlain = "text/plain";
	static const std::string strTextHtml = "text/html";
	static const std::string strTextXml = "text/xml";
	static const std::string strTextCsv = "text/comma-separated-values";
	static const std::string strApplicationXml = "application/xml";
	static const std::string strApplicationJson = "application/json";

	switch(mimeType) {
	case textPlain:
		return strTextPlain;
	case textHtml:
		return strTextHtml;
	case textXml:
		return strTextXml;
	case textCsv:
		return strTextCsv;
	case applicationXml:
		return strApplicationXml;
	case applicationJson:
		return strApplicationJson;
	}
	return strEmpty;
}

} /* namespace utility */
} /* namespace esl */

