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

#ifndef ESL_OBJECT_PARAMETER_INTERFACE_H_
#define ESL_OBJECT_PARAMETER_INTERFACE_H_

#include <esl/module/Interface.h>
#include <esl/Module.h>
#include <esl/Object.h>

#include <string>
#include <memory>
//#include <vector>
//#include <utility>

namespace esl {
namespace object {
namespace parameter {

struct Interface : esl::module::Interface {
	/* ******************************************** *
	 * type definitions required for this interface *
	 * ******************************************** */

	class Object : public esl::Object {
	public:
		virtual ~Object() = default;
		virtual void setParameter(const std::string& key, const std::string& value) = 0;
		// virtual std::vector<std::pair<std::string, std::string>> getParameters() = 0;
	};


	using CreateObject = std::unique_ptr<Object>(*)();

	/* ************************************ *
	 * standard API definition of interface *
	 * ************************************ */

	static inline const char* getId() {
		return "esl-object-parameter";
	}

	static inline const std::string& getApiVersion() {
		return esl::getModule().getApiVersion();
	}

	/* ************************************ *
	 * extended API definition of interface *
	 * ************************************ */

	Interface(std::string module, std::string implementation,
			CreateObject aCreateObject)
	: esl::module::Interface(std::move(module), getId(), std::move(implementation), getApiVersion()),
	  createObject(aCreateObject)
	{ }

	CreateObject createObject;
};

} /* namespace parameter */
} /* namespace object */
} /* namespace esl */

#endif /* ESL_OBJECT_PARAMETER_INTERFACE_H_ */
