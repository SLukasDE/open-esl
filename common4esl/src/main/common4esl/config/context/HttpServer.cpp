#include <common4esl/config/context/HttpServer.h>
#include <common4esl/config/FilePosition.h>

#include <esl/com/http/server/Socket.h>
#include <esl/plugin/exception/PluginNotFound.h>
#include <esl/plugin/Registry.h>

#include <utility>

namespace common4esl {
inline namespace v1_6 {
namespace config {
namespace context {

HttpServer::HttpServer(const std::string& fileName, const tinyxml2::XMLElement& element)
: Config(fileName, element)
{
	if(element.GetUserData() != nullptr) {
		throw FilePosition::add(*this, "Element has user data but it should be empty");
	}

	for(const tinyxml2::XMLAttribute* attribute = element.FirstAttribute(); attribute != nullptr; attribute = attribute->Next()) {
		std::string attributeName = attribute->Name();

		if(attributeName == "id") {
			if(!id.empty()) {
				throw FilePosition::add(*this, "Multiple definitions of attribute 'id'.");
			}
			id = attribute->Value();
			if(id.empty()) {
				throw FilePosition::add(*this, "Invalid value \"\" for attribute 'id'");
			}
		}
		else if(attributeName == "implementation") {
			if(!implementation.empty()) {
				throw std::runtime_error("Multiple definition of attribute 'implementation'.");
			}
			implementation = attribute->Value();
			if(implementation.empty()) {
				throw FilePosition::add(*this, "Invalid value \"\" for attribute 'implementation'");
			}
			if(!refId.empty()) {
				throw FilePosition::add(*this, "Attribute 'implementation' is not allowed together with attribute 'ref-id'.");
			}
		}
		else if(attributeName == "ref-id") {
			if(!refId.empty()) {
				throw FilePosition::add(*this, "Multiple definition of attribute 'ref-id'");
			}
			refId = attribute->Value();
			if(refId.empty()) {
				throw FilePosition::add(*this, "Invalid value \"\" for attribute 'ref-id'");
			}
			if(!implementation.empty()) {
				throw FilePosition::add(*this, "Attribute 'ref-id' is not allowed together with attribute 'implementation'.");
			}
		}
		else {
			throw FilePosition::add(*this, "Unknown attribute '" + attributeName + "'");
		}
	}

	if(id.empty()) {
		throw FilePosition::add(*this, "Missing attribute 'id'");
	}
	if(refId.empty() && implementation.empty()) {
		throw FilePosition::add(*this, "Attribute 'implementation' is missing.");
	}

	for(const tinyxml2::XMLNode* node = element.FirstChild(); node != nullptr; node = node->NextSibling()) {
		const tinyxml2::XMLElement* innerElement = node->ToElement();

		if(innerElement == nullptr) {
			continue;
		}

		auto oldXmlFile = setXMLFile(getFileName(), *innerElement);
		if(!refId.empty()) {
			throw FilePosition::add(*this, "Definition of elements is not allowed together with attribute 'ref-id'.");
		}

		parseInnerElement(*innerElement);
		setXMLFile(oldXmlFile);
	}
}

void HttpServer::save(std::ostream& oStream, std::size_t spaces) const {
	oStream << makeSpaces(spaces) << "<http-server";

	if(!id.empty()) {
		oStream << " id=\"" << id << "\"";
	}

	if(!implementation.empty()) {
		oStream << " implementation=\"" << implementation << "\"";
	}

	if(!refId.empty()) {
		oStream << " ref-id=\"" << refId << "\"";
	}

	if(settings.empty()) {
		oStream << "/>\n";
	}
	else {
		oStream << ">\n";

		for(const auto& setting : settings) {
			setting.saveParameter(oStream, spaces+2);
		}

		oStream << makeSpaces(spaces) << "</http-server>\n";
	}
}


void HttpServer::install(object::ProcessingContext& context) const {
	if(refId.empty()) {
		context.addObject(id, create());
	}
	else {
		context.addAlias(id, refId);

		if(context.findObject<esl::com::http::server::Socket>(refId) == nullptr) {
			if(id.empty()) {
				throw FilePosition::add(*this, "Could not add http server reference, because referenced object with id '" + refId + "' is not a http server socket.");
			}
			else {
				throw FilePosition::add(*this, "Could not add http server reference with id '" + id + "', because referenced object with id '" + refId + "' is not a http server socket.");
			}
		}
	}
}

std::unique_ptr<esl::object::Object> HttpServer::create() const {
	std::vector<std::pair<std::string, std::string>> eslSettings;
	for(const auto& setting : settings) {
		eslSettings.push_back(std::make_pair(setting.key, evaluate(setting.value, setting.language)));
	}

	std::unique_ptr<esl::com::http::server::Socket> socket;
	try {
		socket = esl::plugin::Registry::get().create<esl::com::http::server::Socket>(implementation, eslSettings);
	}
	catch(const esl::plugin::exception::PluginNotFound& e) {
		throw FilePosition::add(*this, e);
	}
	catch(const std::runtime_error& e) {
		throw FilePosition::add(*this, e);
	}
	catch(const std::exception& e) {
		throw FilePosition::add(*this, e);
	}
	catch(...) {
		throw FilePosition::add(*this, "Could not create a http server socket with id '" + id + "' for implementation '" + implementation + "' because an unknown exception occurred.");
	}

	if(!socket) {
		throw FilePosition::add(*this, "Could not create a http server socket with id '" + id + "' for implementation '" + implementation + "' because interface method createSocket() returns nullptr.");
	}

	return std::unique_ptr<esl::object::Object>(socket.release());
}

void HttpServer::parseInnerElement(const tinyxml2::XMLElement& element) {
	if(element.Name() == nullptr) {
		throw FilePosition::add(*this, "Element name is empty");
	}

	std::string elementName(element.Name());

	if(elementName == "parameter") {
		settings.push_back(Setting(getFileName(), element, true));
	}
	else {
		throw FilePosition::add(*this, "Unknown element name \"" + elementName + "\"");
	}
}

} /* namespace context */
} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace common4esl */
