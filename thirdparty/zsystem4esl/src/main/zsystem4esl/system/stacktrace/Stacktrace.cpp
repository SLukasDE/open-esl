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

#include <zsystem4esl/system/stacktrace/Stacktrace.h>

namespace zsystem4esl {
inline namespace v1_6 {
namespace system {
namespace stacktrace {

Stacktrace::Stacktrace(const esl::system::ZSStacktraceFactory::Settings& aSettings)
: settings(aSettings)
{ }

void Stacktrace::dump(std::ostream& stream) const {
	std::list<std::string> elements = createElementsReduced();

	for (const auto& entry : elements) {
		stream << entry << "\n";
	}
}

void Stacktrace::dump(esl::monitoring::Streams::Real& stream, esl::monitoring::Streams::Location location) const {
	std::list<std::string> elements = createElementsReduced();

	for (const auto& entry : elements) {
		stream(location.object, location.function, location.file, location.line) << entry << "\n";
	}
}

void Stacktrace::dumpFull(std::ostream& stream) const {
	for (const auto& entry : backtrace.getElements()) {
		stream << entry << "\n";
	}
}


void Stacktrace::dumpFull(esl::monitoring::Streams::Real& stream, esl::monitoring::Streams::Location location) const {
	for (const auto& entry : backtrace.getElements()) {
		stream(location.object, location.function, location.file, location.line) << entry << "\n";
	}
}

std::unique_ptr<esl::system::Stacktrace> Stacktrace::clone() const {
	return std::unique_ptr<esl::system::Stacktrace>(new Stacktrace(*this));
}

std::list<std::string> Stacktrace::createElementsReduced() const {
	zsystem::Backtrace newBacktrace;

	auto rIter1 = newBacktrace.getElements().crbegin();
	auto rIter2 = backtrace.getElements().crbegin();
	while(rIter1 != newBacktrace.getElements().crend() && rIter2 != backtrace.getElements().crend() && *rIter1 == *rIter2) {
		++rIter1;
		++rIter2;
	}

	std::list<std::string> elements;
	for(;rIter2 != backtrace.getElements().crend(); ++rIter2) {
		elements.push_front(*rIter2);
	}
	for(unsigned int skipElements = settings.skipEntries; skipElements > 0 && !elements.empty(); --skipElements) {
		elements.pop_front();
	}
	return elements;
}

} /* namespace stacktrace */
} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace zsystem4esl */
