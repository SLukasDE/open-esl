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

#include <odbc4esl/database/Connection.h>
#include <odbc4esl/database/Driver.h>
#include <odbc4esl/database/PreparedBulkStatementBinding.h>
#include <odbc4esl/database/PreparedStatementBinding.h>

#include <esl/Logger.h>

#include <esl/database/Diagnostic.h>
#include <esl/database/exception/SqlError.h>
#include <esl/database/PreparedStatement.h>
#include <esl/monitoring/Streams.h>
#include <esl/system/Stacktrace.h>

#include <memory>
#include <stdexcept>

namespace odbc4esl {
inline namespace v1_6 {
namespace database {

namespace {
esl::Logger logger("odbc4esl::database::Connection");

std::set<std::string> implementations{{"unixODBC"}};
}

Connection::Connection(const ConnectionFactory& connectionFactory)
: handle(Driver::getDriver().allocHandleConnection(connectionFactory)),
  defaultBufferSize(connectionFactory.getSettings().defaultBufferSize),
  maximumBufferSize(connectionFactory.getSettings().maximumBufferSize)
{
	ESL__LOGGER_TRACE_THIS("create connection\n");

    // disable autocommit
    Driver::getDriver().setConnectAttr(*this, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);

	Driver::getDriver().driverConnect(*this, connectionFactory.getSettings().connectionString);
}

Connection::~Connection() {
	esl::monitoring::Streams::Location location;
	location.function = __func__;
	location.file = __FILE__;

	try {
		if(!isClosed()) {
		    rollback();
			Driver::getDriver().disconnect(*this);
			handle = SQL_NULL_HDBC;
		}
	}
	catch (const esl::database::exception::SqlError& e) {
		ESL__LOGGER_WARN_THIS("esl::database::exception::SqlError exception occured\n");
		ESL__LOGGER_WARN_THIS(e.what(), "\n");

		location.line = __LINE__;
    	e.getDiagnostics().dump(logger.warn, location);

		const esl::system::Stacktrace* stacktrace = esl::system::Stacktrace::get(e);
    	if(stacktrace) {
    		location.line = __LINE__;
    		stacktrace->dump(logger.warn, location);
    	}
    	else {
    		ESL__LOGGER_WARN_THIS("no stacktrace\n");
    	}
	}
	catch(const std::exception& e) {
		ESL__LOGGER_WARN_THIS("std::exception exception occured\n");
		ESL__LOGGER_WARN_THIS(e.what(), "\n");

		const esl::system::Stacktrace* stacktrace = esl::system::Stacktrace::get(e);
    	if(stacktrace) {
    		location.line = __LINE__;
    		stacktrace->dump(logger.warn, location);
    	}
    	else {
    		ESL__LOGGER_WARN_THIS("no stacktrace\n");
    	}
	}
	catch (...) {
		ESL__LOGGER_ERROR_THIS("unkown exception occured\n");
	}
}

SQLHANDLE Connection::getHandle() const {
	return handle;
}

esl::database::PreparedStatement Connection::prepare(const std::string& sql) const {
	return esl::database::PreparedStatement(std::unique_ptr<esl::database::PreparedStatement::Binding>(new PreparedStatementBinding(*this, sql, defaultBufferSize, maximumBufferSize)));
}

esl::database::PreparedBulkStatement Connection::prepareBulk(const std::string& sql) const {
	return esl::database::PreparedBulkStatement(std::unique_ptr<esl::database::PreparedBulkStatement::Binding>(new PreparedBulkStatementBinding(*this, sql, defaultBufferSize, maximumBufferSize)));
}

void Connection::commit() const {
	if(!isClosed()) {
		ESL__LOGGER_TRACE_THIS("Do commit\n");
		Driver::getDriver().endTran(*this, SQL_COMMIT);
	}
	else {
		ESL__LOGGER_TRACE_THIS("NO commit, connection already closed\n");
	}
}

void Connection::rollback() const {
	if(!isClosed()) {
		Driver::getDriver().endTran(*this, SQL_ROLLBACK);
	}
}

bool Connection::isClosed() const {
	return handle == SQL_NULL_HDBC;
}

void* Connection::getNativeHandle() const {
	return const_cast<void*>(handle);
}

const std::set<std::string>& Connection::getImplementations() const {
	return implementations;
}


} /* namespace database */
} /* inline namespace v1_6 */
} /* namespace odbc4esl */
