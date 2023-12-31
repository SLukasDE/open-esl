#include "logbook4esl/Example02.h"
#include "logbook4esl/Logger.h"

#include <logbook4esl/monitoring/Logging.h>

#include <common4esl/monitoring/DefaultLayout.h>
#include <common4esl/monitoring/OStreamAppender.h>

#include <esl/monitoring/Appender.h>
#include <esl/monitoring/Layout.h>
#include <esl/monitoring/Logging.h>

namespace logbook4esl {
inline namespace v1_6 {

namespace {
/* creating a logger class */
Logger logger("logbook4esl::Example02");
}

void Example02::run() {
	std::unique_ptr<esl::monitoring::Logging> aLogging(new logbook4esl::monitoring::Logging({}));
	esl::monitoring::Logging::init(std::move(aLogging));

	{
		std::unique_ptr<esl::monitoring::Layout> defaultLayout(new common4esl::monitoring::DefaultLayout({}));
		esl::monitoring::Logging::get()->addLayout("defaultLayout", std::move(defaultLayout));
	}

	{
		std::unique_ptr<esl::monitoring::Appender> oStreamAppender(new common4esl::monitoring::OStreamAppender({
			{"trace", "out"},
			{"debug", "out"},
			{"info", "out"},
			{"warn", "out"},
			{"error", "out"}
		}));
		esl::monitoring::Logging::get()->addAppender("", "defaultLayout", std::move(oStreamAppender));
	}



	/* this function is similar to example01(). But we make a little bit more.
	 * First, we make our logger instance static. It is not necessary to make a new instance every time we call a function.
	 * Second, we give this instance a name.
	 *   Typically you specify your full qualifies class name if you put a logger instance inside your class
	 *   or your full qualified namespace if you use a global available logger for a namespace.
	 */


	/* Now you will see your output */
	logger.info << "Hello again!" << std::endl;

	/* the output might be much more specific as you expected. The details depend on your appender you are using and how you initialized it */
}

} /* inline namespace v1_6 */
} /* namespace logbook4esl */
