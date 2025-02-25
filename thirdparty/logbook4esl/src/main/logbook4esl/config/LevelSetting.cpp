#include <logbook4esl/config/LevelSetting.h>

#include <common4esl/config/FilePosition.h>

#include <esl/utility/String.h>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

LevelSetting::LevelSetting(const std::string& fileName, const tinyxml2::XMLElement& element)
: Config(fileName, element)
{
	if(element.GetUserData() != nullptr) {
		throw common4esl::config::FilePosition::add(*this, "Element has user data but it should be empty");
	}

	bool levelDefined = false;

	for(const tinyxml2::XMLAttribute* attribute = element.FirstAttribute(); attribute != nullptr; attribute = attribute->Next()) {
		if(std::string(attribute->Name()) == "scope") {
			if(scope != "") {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'scope'");
			}
			scope = attribute->Value();
			if(scope == "") {
				throw common4esl::config::FilePosition::add(*this, "Value \"\" of attribute 'scope' is invalid.");
			}
		}
		else if(std::string(attribute->Name()) == "level") {
			if(levelDefined) {
				throw common4esl::config::FilePosition::add(*this, "Multiple definition of attribute 'level'");
			}
			std::string levelStr = esl::utility::String::toUpper(attribute->Value());

			if(levelStr == "SILENT") {
				level = esl::monitoring::Streams::Level::silent;
			}
			else if(levelStr == "ERROR") {
				level = esl::monitoring::Streams::Level::error;
			}
			else if(levelStr == "WARN") {
				level = esl::monitoring::Streams::Level::warn;
			}
			else if(levelStr == "INFO") {
				level = esl::monitoring::Streams::Level::info;
			}
			else if(levelStr == "DEBUG") {
				level = esl::monitoring::Streams::Level::debug;
			}
			else if(levelStr == "TRACE") {
				level = esl::monitoring::Streams::Level::trace;
			}
			else {
				throw common4esl::config::FilePosition::add(*this, "Value \"" + levelStr + "\" of attribute 'level' is invalid. "
						"Valid values are \"SILENT\", \"ERROR\", \"WARN\", \"INFO\", \"DEBUG\" and \"TRACE\"");
			}
			levelDefined = true;
		}
		else {
			throw common4esl::config::FilePosition::add(*this, "Unknown attribute '" + std::string(attribute->Name()) + "'");
		}
	}

	if(scope == "") {
		throw common4esl::config::FilePosition::add(*this, "Missing attribute 'scope'");
	}
	if(!levelDefined) {
		throw common4esl::config::FilePosition::add(*this, "Missing attribute 'level'");
	}
}

esl::monitoring::Streams::Level LevelSetting::getLevel() const {
	return level;
}

const std::string& LevelSetting::getScope() const {
	return scope;
}

void LevelSetting::save(std::ostream& oStream, std::size_t spaces) const {
	switch(level) {
	case esl::monitoring::Streams::Level::silent:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"silent\"/>\n";
		break;
	case esl::monitoring::Streams::Level::error:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"error\"/>\n";
		break;
	case esl::monitoring::Streams::Level::warn:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"warn\"/>\n";
		break;
	case esl::monitoring::Streams::Level::info:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"info\"/>\n";
		break;
	case esl::monitoring::Streams::Level::debug:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"debug\"/>\n";
		break;
	case esl::monitoring::Streams::Level::trace:
		oStream << makeSpaces(spaces) << "<setting scope=\"" << scope << "\" level=\"trace\"/>\n";
		break;
	}
}

} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */
