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

#ifndef ESL_SYSTEM_TASK_H_
#define ESL_SYSTEM_TASK_H_

#include <esl/object/Context.h>
#include <esl/object/Procedure.h>

#include <chrono>
#include <exception>
#include <functional>
#include <memory>

namespace esl {
inline namespace v1_6 {
namespace system {

class Task {
public:
	enum class Status {
		waiting,
		canceled,
		running,
		exception,
		done
	};

#ifdef ESL_1_6
	virtual ~Task() = default;

	virtual void cancel() = 0;

	virtual Status getStatus() const = 0;
	virtual object::Context* getContext() const = 0;
	virtual std::exception_ptr getException() const = 0;
#else
	struct Descriptor {
	public:
		std::unique_ptr<object::Procedure> procedure;
		std::unique_ptr<object::Context> context;
		unsigned int priority = 0;
		std::function<void(Status)> onStateChanged;
	};

	class Binding {
	public:
		virtual ~Binding() = default;

		virtual void cancel() = 0;

		virtual Status getStatus() const = 0;
		virtual object::Context* getContext() const = 0;
		virtual std::exception_ptr getException() const = 0;
	};

	Task(std::shared_ptr<Binding> binding);

	void cancel();

	Status getStatus() const;
	object::Context* getContext() const;
	std::exception_ptr getException() const;

private:
	std::shared_ptr<Binding> binding;
#endif
};

} /* namespace system */
} /* inline namespace v1_6 */
} /* namespace esl */

#endif /* ESL_SYSTEM_TASK_H_ */
