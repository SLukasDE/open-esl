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

#include <esl/processing/task/Task.h>

namespace esl {
namespace processing {
namespace task {

Task::Task(std::shared_ptr<Binding> aBinding)
: binding(aBinding)
{ }

void Task::sendEvent(const object::IObject& object) {
	if(binding) {
		binding->sendEvent(object);
	}
}

void Task::cancel() {
	if(binding) {
		return binding->cancel();
	}
}

Status Task::getStatus() const {
	if(binding) {
		return binding->getStatus();
	}
	return Status::unknown;
}

object::IContext* Task::getContext() const {
	if(binding) {
		return binding->getContext();
	}
	return nullptr;
}

std::exception_ptr Task::getException() const {
	if(binding) {
		return binding->getException();
	}
	return nullptr;
}

} /* namespace task */
} /* namespace processing */
} /* namespace esl */
