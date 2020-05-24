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

#include <esl/database/PreparedStatement.h>

namespace esl {
namespace database {

namespace {
std::vector<Column> emptyColumns;
}

PreparedStatement::PreparedStatement(PreparedStatement&& other)
: binding(std::move(other.binding))
{
}

PreparedStatement::PreparedStatement(std::unique_ptr<Binding> aBinding)
: binding(std::move(aBinding))
{ }

PreparedStatement::operator bool() const noexcept {
	return binding ? true : false;
}

PreparedStatement& PreparedStatement::operator=(PreparedStatement&& other) {
	binding = std::move(other.binding);
	return *this;
}

const std::vector<Column>& PreparedStatement::getResultColumns() const {
	if(binding) {
		return binding->getResultColumns();
	}
	return emptyColumns;
}

ResultSet PreparedStatement::execute() {
    return execute(std::vector<Field>());
}

ResultSet PreparedStatement::execute(const std::vector<Field>& fields) {
	if(binding) {
		return binding->execute(fields);
	}
	return ResultSet();
}

void PreparedStatement::executeBulk() {
    executeBulk(std::vector<std::vector<Field>>());
}

void PreparedStatement::executeBulk(const std::vector<std::vector<Field>>& fieldArrays) {
	if(binding) {
		binding->executeBulk(fieldArrays);
	}
}

} /* namespace database */
} /* namespace esl */