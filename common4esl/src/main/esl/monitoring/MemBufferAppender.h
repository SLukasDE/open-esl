#ifndef ESL_MONITORING_MEMBUFFERAPPENDER_H_
#define ESL_MONITORING_MEMBUFFERAPPENDER_H_

#include <esl/monitoring/Appender.h>
#include <esl/monitoring/Layout.h>
#include <esl/monitoring/Streams.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace esl {
inline namespace v1_6 {
namespace monitoring {

class MemBufferAppender : public Appender {
public:
	struct Settings {
		Settings() = default;
		Settings(const std::vector<std::pair<std::string, std::string>>& settings);

		std::size_t maxLines = 0;
		std::size_t maxColumns = 0;
	};

	MemBufferAppender(const Settings& settings);

	static std::unique_ptr<Appender> create(const std::vector<std::pair<std::string, std::string>>& settings);

	void setLayout(const Layout* aLayout) override;
	const Layout* getLayout() const override;

	/* both methods are NOT thread-safe */
	void setRecordLevel(RecordLevel aRecordLevel = RecordLevel::SELECTED) override;
	RecordLevel getRecordLevel() const override;

	void flush(std::ostream* oStream) override;

	void write(const Streams::Location& location, const char* ptr, std::size_t size) override;

private:
	std::unique_ptr<Appender> appender;
};

} /* namespace monitoring */
} /* inline namespace v1_6 */
} /* namespace esl */


#endif /* ESL_MONITORING_MEMBUFFERAPPENDER_H_ */
