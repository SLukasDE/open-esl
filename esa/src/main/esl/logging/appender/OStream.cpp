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

#include <esl/logging/appender/OStream.h>
#include <esl/logging/Logger.h>
#include <iostream>
#include <cstdio>

namespace esl {
namespace logging {
namespace appender {


OStream::OStream(std::ostream& oStreamTrace,
		std::ostream& oStreamDebug,
		std::ostream& oStreamInfo,
		std::ostream& oStreamWarn,
		std::ostream& oStreamError)
: Appender(),
  oStreamTrace(oStreamTrace),
  oStreamDebug(oStreamDebug),
  oStreamInfo(oStreamInfo),
  oStreamWarn(oStreamWarn),
  oStreamError(oStreamError)
{
}

void OStream::flushNewLine(const Location& location, bool enabled) {
	switch(getRecordLevel()) {
	case RecordLevel::OFF:
		return;
	case RecordLevel::ALL:
		break;
	default: /* RecordLevel::SELECTED */
		if(!enabled) {
			return;
		}
		break;
	}

	std::ostream& oStream = getOStream(location.level);
	if(!isFirstCharacterInLine) {
		oStream << "\n";
		isFirstCharacterInLine = true;
	}
	oStream.flush();
}

void OStream::write(const Location& location, bool enabled, const char* ptr, std::size_t size) {
	switch(getRecordLevel()) {
	case RecordLevel::OFF:
		return;
	case RecordLevel::ALL:
		break;
	default: /* RecordLevel::SELECTED */
		if(!enabled) {
			return;
		}
		break;
	}

	std::ostream& oStream = getOStream(location.level);
	const char* begin = ptr;

	for(auto iter = ptr; iter != &ptr[size]; ++iter) {
		if(isFirstCharacterInLine) {
			oStream << getLayout().makePrefix(location);
			isFirstCharacterInLine = false;
		}

		if(*iter == '\n') {
			oStream << std::string(begin, iter) << "\n";
			isFirstCharacterInLine = true;
			begin = iter+1;
		}
	}
	oStream << std::string(begin, &ptr[size]);
}

std::ostream& OStream::getOStream(Level level) {
	switch(level) {
	case Level::TRACE:
		return oStreamTrace;
	case Level::DEBUG:
		return oStreamDebug;
	case Level::INFO:
		return oStreamInfo;
	case Level::WARN:
		return oStreamWarn;
	case Level::ERROR:
		return oStreamError;
	default:
		break;
	}

	return oStreamError;
}

} /* namespace appender */
} /* namespace logging */
} /* namespace esl */
