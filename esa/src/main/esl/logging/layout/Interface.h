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

#ifndef ESL_LOGGING_LAYOUT_INTERFACE_H_
#define ESL_LOGGING_LAYOUT_INTERFACE_H_

#include <esl/module/Interface.h>
#include <esl/Module.h>
#include <esl/object/Settings.h>
#include <esl/logging/Location.h>
#include <string>
#include <memory>

namespace esl {
namespace logging {
namespace layout {

struct Interface : esl::module::Interface {

	/* ******************************************** *
	 * type definitions required for this interface *
	 * ******************************************** */

	class Layout : public esl::object::Settings {
	public:
		virtual std::string toString(const Location& location) const = 0;
	};

	using CreateLayout = std::unique_ptr<Layout> (*)();

	/* ************************************ *
	 * standard API definition of interface *
	 * ************************************ */

	static inline const char* getType() {
		return "esl-logging-layout";
	}

	static inline const std::string& getApiVersion() {
		return esl::getModule().getApiVersion();
	}

	/* ************************************ *
	 * extended API definition of interface *
	 * ************************************ */

	Interface(std::string module, std::string implementation, CreateLayout aCreateLayout)
	: esl::module::Interface(std::move(module), getType(), std::move(implementation), getApiVersion()),
	  createLayout(aCreateLayout)
	{ }

	CreateLayout createLayout;
};

} /* namespace layout */
} /* namespace logging */
} /* namespace esl */

#endif /* ESL_LOGGING_LAYOUT_INTERFACE_H_ */
