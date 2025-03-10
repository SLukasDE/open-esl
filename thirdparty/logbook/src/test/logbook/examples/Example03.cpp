#include <logbook/examples/Example03.h>
#include <logbook/appender/OStream.h>
#include <logbook/layout/Default.h>
#include <logbook/Logbook.h>

#include <iostream>

namespace logbook {
inline namespace v0_4 {
namespace examples {

namespace { /* anonymous namespace */

/* As before we need an appender to see your output.
 *
 * BTW: You don't have to initialize the logger always for each class!
 * Later you will make it once, near to your main()-function.
 */
logbook::appender::OStream appenderOstream(std::cout, std::cout, std::cout, std::cout, std::cout);
logbook::layout::Default defaultLayout;

} /* anonymous namespace */

/* Now we are using a logger within a class. This is where it becomes very interesting for professional usage.
 * First we put an static instance into the private section and initialize it with the full qualified name class name.
 */
logbook::Logger Example03::logger = "logbook::examples::Example03";

float Example03::divide(float a, float b) {
	if(b != 0) {
		/* here you can see how to tell the logger where we are. This might be very useful to find the location of an error.
		 * You can leave it empty, then you know already that this output occurred in class "esl::examples::logger::Example03".
		 * But now you have much more details to your location.
		 */
		logger.debug(__func__, __FILE__, __LINE__) << "operation possible\n";
		return a/b;
	}

	/* here we put one more parameter to the logger: "this". This can be useful, to know from which instance the error occurred. */
	logger.error(this, __func__, __FILE__, __LINE__) << "cannot divide by zero!\n";
	return 0;
}






void example03() {
	/* Initialize the logger framework */
	//logbook::Logger::addAppender(appenderOstream);
//    logbook::addAppender(appenderOstream);
	appenderOstream.setLayout(&defaultLayout);

	/* each appender has a layout to specify which columns we want to see.
	 * Let's play a little bit with this feature ... */
	defaultLayout.setShowFile(true);
	defaultLayout.setShowLineNo(true);
	defaultLayout.setShowAddress(true);
	defaultLayout.setShowFunction(true);
	defaultLayout.setShowLevel(true);
	defaultLayout.setShowThreadNo(true);
	defaultLayout.setShowTimestamp(true);
	defaultLayout.setShowTypeName(true);

	/* Let's do some more initialization...
	 * Most time we are not interested into all log messages of your application.
	 * There are 2 dimensions to tell the logger what messages you want to see.
	 *
	 * The first dimension is the log-level.
	 * TRACE is the highest level of the logger and it includes all log messages.
	 * SILENT is the lowest level of the logger and as you can imagine it includes no log messages.
	 * This is the hierarchy of all levels: TRACE > DEBUG > INFO > WARN > ERROR > SILENT
	 *
	 * The second dimension is the "class path" where you want to specify a particular log level.
	 * It is possible to specify for each path and it's sub paths a different log level.
	 * Here we want to make all log messages silent but for class Example03 we want to get all log messages up to "WARN"-level.
	 */
	logbook::setLevel(logbook::Level::silent, "*");
	logbook::setLevel(logbook::Level::warn, "esl::examples::logger::Example03");

	/* now let's test it! */
	Example03 e;
	e.divide(5, 5);
	e.divide(5, 0);
}

} /* namespace examples */
} /* inline namespace v0_4 */
} /* namespace logbook */
