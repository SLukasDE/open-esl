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

#ifndef ESL_DATABASE_TABLE_INTERFACE_H_
#define ESL_DATABASE_TABLE_INTERFACE_H_

#include <esl/module/Interface.h>
#include <esl/object/Interface.h>
#include <esl/database/Interface.h>
#include <esl/Module.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
namespace database {
namespace table {

struct Interface : module::Interface {
	/* ******************************************** *
	 * type definitions required for this interface *
	 * ******************************************** */

	class Tables : public virtual object::Interface::Object {
	public:
	};

	using CreateTables = std::unique_ptr<Tables> (*)(const std::vector<std::pair<std::string, std::string>>& settings);

	/* ************************************ *
	 * standard API definition of interface *
	 * ************************************ */

	static inline const char* getType() {
		return "esl-database-table";
	}

	/* ************************************ *
	 * extended API definition of interface *
	 * ************************************ */

	static std::unique_ptr<const module::Interface> createInterface(const char* implementation, CreateTables createTables) {
		return std::unique_ptr<const module::Interface>(new Interface(implementation, createTables));
	}

	Interface(const char* implementation, CreateTables aCreateTables)
	: module::Interface(getModule().getId(), getType(), implementation, getModule().getApiVersion()),
	  createTables(aCreateTables)
	{ }

	CreateTables createTables;
};

} /* namespace table */
} /* namespace database */
} /* namespace esl */

#endif /* ESL_DATABASE_TABLE_INTERFACE_H_ */
