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

#ifndef ZSYSTEM4ESL_SYSTEM_SIGNAL_SIGNAL_H_
#define ZSYSTEM4ESL_SYSTEM_SIGNAL_SIGNAL_H_

#include <esl/system/Signal.h>
#include <esl/utility/Signal.h>
#include <esl/object/Object.h>

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace zsystem4esl {
namespace system {
namespace signal {

class Signal : public esl::system::Signal {
public:
	static std::unique_ptr<esl::system::Signal> create(const std::vector<std::pair<std::string, std::string>>& settings);

	static inline const char* getImplementation() {
		return "zsystem4esl";
	}

	Signal(const std::vector<std::pair<std::string, std::string>>& settings);

	esl::system::Signal::Handler createHandler(const esl::utility::Signal& signal, std::function<void()> function) override;

private:
	bool threadedSignalHandler = true;
};

} /* namespace signal */
} /* namespace system */
} /* namespace zsystem4esl */

#endif /* ZSYSTEM4ESL_SYSTEM_SIGNAL_SIGNAL_H_ */
