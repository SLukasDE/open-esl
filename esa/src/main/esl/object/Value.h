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

#ifndef ESL_OBJECT_VALUE_H_
#define ESL_OBJECT_VALUE_H_

#include <esl/object/IObject.h>
#include <esl/object/ICloneable.h>

#include <memory>

namespace esl {
namespace object {

template <typename T>
class Value : public ICloneable {
public:
	Value(const T& aValue)
	: value(aValue)
	{ }

	Value(T&& aValue)
	: value(std::move(aValue))
	{ }

	std::unique_ptr<IObject> clone() const override {
		return std::unique_ptr<IObject>(new Value<T>(value));
	}

	T& operator=(const T& aValue) {
		value = aValue;
		return value;
	}

	T& operator=(T&& aValue) {
		value = std::move(aValue);
		return value;
	}

	const T& operator*() const noexcept {
		return value;
	}

	T& operator*() noexcept {
		return value;
	}

	const T& operator->() const noexcept {
		return value;
	}

	T& operator->() noexcept {
		return value;
	}

	const T& get() const noexcept {
		return value;
	}

	T& get() noexcept {
		return value;
	}

private:
	T value;
};

} /* namespace object */
} /* namespace esl */

#endif /* ESL_OBJECT_VALUE_H_ */
