#include <logbook/examples/Example02.h>
#include <logbook/Logbook.h>
#include <logbook/Logger.h>
#include <logbook/appender/OStream.h>

#include <iostream>


namespace logbook {
inline namespace v0_4 {
namespace examples {

namespace { /* anonymous namespace */

/* There are several appenders available and of course you write your own fancy appender.
 *
 * Here we use the most common appender: "esl::logging::appender::OStream"
 * It writes the output of each log level (trace, debug, warn, info, error) to a given "std::ostream".
 * Here we use std::cout for all log levels, but you can use std::fstream for example as well.
 */
logbook::appender::OStream appenderOstream(std::cout, std::cout, std::cout, std::cout, std::cout);

void simpleInitialize() {
	/* Now we want to add this appender instance to the logging framework */
	//logbook::addAppender(appenderOstream);

	/* you can add multiple appenders and each appender has settings, but that's enough for now. */
}

void testFunction() {
	/* this function is similar to example01(). But we make a little bit more.
	 * First, we make our logger instance static. It is not necessary to make a new instance every time we call a function.
	 * Second, we give this instance a name.
	 *   Typically you specify your full qualifies class name if you put a logger instance inside your class
	 *   or your full qualified namespace if you use a global available logger for a namespace.
	 */
	/* creating a logger class */
	static logbook::Logger logger("exmaple02");

	/* Now you will see your output */
	logger.info << "Hello again!" << std::endl;

	/* the output might be much more specific as you expected. The details depend on your appender you are using and how you initialized it */
}

} /* anonymous namespace */


void example02() {
	simpleInitialize();

	testFunction();
}

} /* namespace examples */
} /* inline namespace v0_4 */
} /* namespace logbook */
