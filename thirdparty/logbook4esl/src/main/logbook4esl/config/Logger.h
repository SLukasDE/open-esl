#ifndef LOGBOOK4ESL_CONFIG_LOGGER_H_
#define LOGBOOK4ESL_CONFIG_LOGGER_H_

#include <logbook4esl/config/Appender.h>
#include <logbook4esl/config/Layout.h>
#include <logbook4esl/config/LevelSetting.h>
#include <logbook4esl/monitoring/Logging.h>

#include <common4esl/config/Config.h>

#include <tinyxml2.h>

#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace logbook4esl {
inline namespace v1_6 {
namespace config {

class Logger : public common4esl::config::Config {
public:
	Logger(bool isFile, const std::string& value);
	Logger(const std::string& filename, const tinyxml2::XMLElement& element);

	void save(std::ostream& oStream) const;
	void install(monitoring::Logging& aLogging) const;

private:
	std::map<std::string, Layout> layouts;
	std::vector<Appender> appenders;
	std::vector<LevelSetting> settings;

	void loadXML(const tinyxml2::XMLElement& element);

	void parseInnerElement(const tinyxml2::XMLElement& element);
};

} /* namespace config */
} /* inline namespace v1_6 */
} /* namespace logbook4esl */

#endif /* LOGBOOK4ESL_CONFIG_LOGGER_H_ */
