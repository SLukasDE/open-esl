/*
 * This file is part of unixODBC4esl.
 * Copyright (C) 2020-2022 Sven Lukas
 *
 * UnixODBC4esl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnixODBC4esl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with mhd4esl.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <unixODBC4esl/database/PreparedStatementBinding.h>
#include <unixODBC4esl/database/BindVariable.h>
#include <unixODBC4esl/database/Driver.h>
#include <unixODBC4esl/database/ResultSetBinding.h>
#include <unixODBC4esl/Logger.h>

#include <esl/system/Stacktrace.h>

#include <sqlext.h>

#include <stdexcept>
#include <memory>

namespace unixODBC4esl {
namespace database {

namespace {
Logger logger("unixODBC4esl::database::PreparedStatementBinding");
}

PreparedStatementBinding::PreparedStatementBinding(const Connection& aConnection, const std::string& aSql, std::size_t defaultBufferSize, std::size_t maximumBufferSize)
: connection(aConnection),
  sql(aSql),
  statementHandle(Driver::getDriver().prepare(connection, sql))
{
	// Get number of result columns from prepared statement
	SQLSMALLINT resultColumnCount = Driver::getDriver().numResultCols(statementHandle);

	logger.trace << "Result columns from SQL \"" << sql << "\" (" << resultColumnCount << "):\n";
	logger.trace << "-----------------------------------------------\n";
	for(SQLSMALLINT i=0; i<resultColumnCount; ++i) {
		std::string resultColumnName;
		esl::database::Column::Type resultColumnType;
		bool resultValueNullable;
		std::size_t resultValueCharacterLength;
		std::size_t resultValueDecimalDigits;
		std::size_t resultValueDisplayLength;

		Driver::getDriver().describeCol(statementHandle, i+1, resultColumnName, resultColumnType, resultValueCharacterLength, resultValueDecimalDigits, resultValueNullable);
		Driver::getDriver().colAttributeDisplaySize(statementHandle, i+1, resultValueDisplayLength);

		if(logger.trace) {
			logger.trace << "Column " << i << ":\n";
			logger.trace << "    Name: \"" << resultColumnName << "\"\n";
			switch(resultColumnType) {
			case esl::database::Column::Type::sqlBoolean:
				logger.trace << "    Type: sqlBoolean\n";
				break;
			case esl::database::Column::Type::sqlInteger:
				logger.trace << "    Type: sqlInteger\n";
				break;
			case esl::database::Column::Type::sqlSmallInt:
				logger.trace << "    Type: sqlSmallInt\n";
				break;
			case esl::database::Column::Type::sqlDouble:
				logger.trace << "    Type: sqlDouble\n";
				break;
			case esl::database::Column::Type::sqlNumeric:
				logger.trace << "    Type: sqlNumeric\n";
				break;
			case esl::database::Column::Type::sqlDecimal:
				logger.trace << "    Type: sqlDecimal\n";
				break;
			case esl::database::Column::Type::sqlFloat:
				logger.trace << "    Type: sqlFloat\n";
				break;
			case esl::database::Column::Type::sqlReal:
				logger.trace << "    Type: sqlReal\n";
				break;
			case esl::database::Column::Type::sqlVarChar:
				logger.trace << "    Type: sqlVarChar\n";
				break;
			case esl::database::Column::Type::sqlChar:
				logger.trace << "    Type: sqlChar\n";
				break;
			case esl::database::Column::Type::sqlDateTime:
				logger.trace << "    Type: sqlDateTime\n";
				break;
			case esl::database::Column::Type::sqlDate:
				logger.trace << "    Type: sqlDate\n";
				break;
			case esl::database::Column::Type::sqlTime:
				logger.trace << "    Type: sqlTime\n";
				break;
			case esl::database::Column::Type::sqlTimestamp:
				logger.trace << "    Type: sqlTimestamp\n";
				break;
			case esl::database::Column::Type::sqlWChar:
				logger.trace << "    Type: sqlWChar\n";
				break;
			case esl::database::Column::Type::sqlWVarChar:
				logger.trace << "    Type: sqlWVarChar\n";
				break;
			case esl::database::Column::Type::sqlWLongVarChar:
				logger.trace << "    Type: sqlWLongVarChar\n";
				break;
			default:
				logger.trace << "    Type: sqlUnknown\n";
				break;
			}

			if(resultValueNullable) {
				logger.trace << "    Nullable: true\n";
			}
			else {
				logger.trace << "    Nullable: false\n";
			}

			logger.trace << "    CharacterLength: " << resultValueCharacterLength << "\n";

			logger.trace << "    DecimalDigits: " << resultValueDecimalDigits << "\n";

			logger.trace << "    DisplayLength: " << resultValueDisplayLength << "\n";
		}

		resultColumns.emplace_back(std::move(resultColumnName), resultColumnType, resultValueNullable, defaultBufferSize, maximumBufferSize, resultValueCharacterLength, resultValueDecimalDigits, resultValueDisplayLength);
    }
	logger.trace << "-----------------------------------------------\n\n";

	// Get number of parameters from prepared statement
	SQLSMALLINT parameterCount = Driver::getDriver().numParams(statementHandle);

	logger.trace << "Parameter columns (" << parameterCount << "):\n";
	logger.trace << "-----------------------------------------------\n";
	for(SQLSMALLINT i=0; i<parameterCount; ++i) {
		esl::database::Column::Type parameterColumnType;
		std::size_t parameterValueCharacterLength;
		std::size_t parameterValueDecimalDigits;
		bool parameterValueNullable;

		Driver::getDriver().describeParam(statementHandle, i+1, parameterColumnType, parameterValueCharacterLength, parameterValueDecimalDigits, parameterValueNullable);

		if(logger.trace) {
			logger.trace << "Column " << i << ":\n";
			switch(parameterColumnType) {
			case esl::database::Column::Type::sqlBoolean:
				logger.trace << "    Type: sqlBoolean\n";
				break;
			case esl::database::Column::Type::sqlInteger:
				logger.trace << "    Type: sqlInteger\n";
				break;
			case esl::database::Column::Type::sqlSmallInt:
				logger.trace << "    Type: sqlSmallInt\n";
				break;
			case esl::database::Column::Type::sqlDouble:
				logger.trace << "    Type: sqlDouble\n";
				break;
			case esl::database::Column::Type::sqlNumeric:
				logger.trace << "    Type: sqlNumeric\n";
				break;
			case esl::database::Column::Type::sqlDecimal:
				logger.trace << "    Type: sqlDecimal\n";
				break;
			case esl::database::Column::Type::sqlFloat:
				logger.trace << "    Type: sqlFloat\n";
				break;
			case esl::database::Column::Type::sqlReal:
				logger.trace << "    Type: sqlReal\n";
				break;
			case esl::database::Column::Type::sqlVarChar:
				logger.trace << "    Type: sqlVarChar\n";
				break;
			case esl::database::Column::Type::sqlChar:
				logger.trace << "    Type: sqlChar\n";
				break;
			case esl::database::Column::Type::sqlDateTime:
				logger.trace << "    Type: sqlDateTime\n";
				break;
			case esl::database::Column::Type::sqlDate:
				logger.trace << "    Type: sqlDate\n";
				break;
			case esl::database::Column::Type::sqlTime:
				logger.trace << "    Type: sqlTime\n";
				break;
			case esl::database::Column::Type::sqlTimestamp:
				logger.trace << "    Type: sqlTimestamp\n";
				break;
			case esl::database::Column::Type::sqlWChar:
				logger.trace << "    Type: sqlWChar\n";
				break;
			case esl::database::Column::Type::sqlWVarChar:
				logger.trace << "    Type: sqlWVarChar\n";
				break;
			case esl::database::Column::Type::sqlWLongVarChar:
				logger.trace << "    Type: sqlWLongVarChar\n";
				break;
			default:
				logger.trace << "    Type: sqlUnknown\n";
				break;
			}

			if(parameterValueNullable) {
				logger.trace << "    Nullable: true\n";
			}
			else {
				logger.trace << "    Nullable: false\n";
			}

			logger.trace << "    CharacterLength: " << parameterValueCharacterLength << "\n";

			logger.trace << "    DecimalDigits: " << parameterValueDecimalDigits << "\n";

			logger.trace << "    DisplayLength: (=parameterValueCharacterLength)\n";
		}

		parameterColumns.emplace_back("", parameterColumnType, parameterValueNullable, defaultBufferSize, maximumBufferSize, parameterValueDecimalDigits, parameterValueCharacterLength, parameterValueCharacterLength);
    }
	logger.trace << "-----------------------------------------------\n\n";
}

const std::vector<esl::database::Column>& PreparedStatementBinding::getParameterColumns() const {
	return parameterColumns;
}

const std::vector<esl::database::Column>& PreparedStatementBinding::getResultColumns() const {
	return resultColumns;
}

esl::database::ResultSet PreparedStatementBinding::execute(const std::vector<esl::database::Field>& parameterValues) {
	if(!statementHandle) {
		logger.trace << "RE-Create statement handle\n";
		statementHandle = StatementHandle(Driver::getDriver().prepare(connection, sql));
	}

	if(parameterColumns.size() != parameterValues.size()) {
	    throw esl::system::Stacktrace::add(std::runtime_error("Wrong number of arguments. Given " + std::to_string(parameterValues.size()) + " parameters but required " + std::to_string(parameterColumns.size()) + " parameters."));
	}

	std::vector<std::unique_ptr<BindVariable>> parameterVariables(parameterValues.size());

	for(std::size_t i=0; i<parameterValues.size(); ++i) {
		parameterVariables[i].reset(new BindVariable(statementHandle, parameterColumns[i], i));
		parameterVariables[i]->getField(parameterValues[i]);
	}

	/* ResultSetBinding makes the "execute" */
	Driver::getDriver().execute(statementHandle);

	esl::database::ResultSet resultSet;

	/* make a fetch, if SQL statement has result set (e.g. no INSERT, UPDATE, DELETE) */
	if(!resultColumns.empty()) {
		std::unique_ptr<esl::database::ResultSet::Binding> resultSetBinding(new ResultSetBinding(std::move(statementHandle), resultColumns));

		/* this makes a fetch */
		resultSet = esl::database::ResultSet(std::unique_ptr<esl::database::ResultSet::Binding>(std::move(resultSetBinding)));
	}

	return resultSet;
}

void* PreparedStatementBinding::getNativeHandle() const {
	if(statementHandle) {
		return statementHandle.getHandle();
	}
	return nullptr;
}

} /* namespace database */
} /* namespace unixODBC4esl */
