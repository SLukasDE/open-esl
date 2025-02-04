/*
 * This file is part of odbc4esl.
 * Copyright (C) 2020-2023 Sven Lukas
 *
 * Odbc4esl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Odbc4esl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with mhd4esl.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <odbc4esl/database/Diagnostics.h>
#include <odbc4esl/database/Driver.h>

#include <esl/Logger.h>

#include <algorithm>

namespace odbc4esl {
inline namespace v1_6 {
namespace database {

namespace {
esl::Logger logger("odbc4esl::database::Diagnostics");
}

Diagnostics::Diagnostics(SQLSMALLINT type, SQLHANDLE handle)
: esl::database::Diagnostics()
{
    if(type != SQL_HANDLE_ENV || handle != SQL_NULL_HENV) {
    	for(SQLSMALLINT index = 0; true; ++index) {
    		esl::database::Diagnostic diagnostic;
    		if(Driver::getDriver().getDiagRec(diagnostic, type, handle, index+1) == false) {
    			break;
    		}
    		diagnostics.emplace_back(std::move(diagnostic));
    	}
    }
}

#if 0
const esl::database::Diagnostic* Diagnostics::getSQLCode() const {
	const Diagnostic* rv = nullptr;

	for(std::vector<esl::database::Diagnostic>::size_type i = 0; i < diagnostics.size(); ++i) {
		const Diagnostic& diagnostic = diagnostics[i];
        if(diagnostic.code == -99999 && (diagnostic.state == "xxxxx" || diagnostic.state == "01S02")) {
            continue;
        }
		if(rv == nullptr || rv->state < diagnostic.state) {
			rv = &diagnostic;
		}
	}
	return rv;
}
#endif

} /* namespace database */
} /* inline namespace v1_6 */
} /* namespace odbc4esl */
