#ifndef ESA_EXAMPLES_LOGGER_H_
#define ESA_EXAMPLES_LOGGER_H_

#include <esa/logging/Logger.h>

namespace esa {
inline namespace v1_6 {
namespace examples {

extern esl::monitoring::Logger<> logger;
void loggerInitialize();
void loggerReplay();

} /* namespace examples */
} /* inline namespace v1_6 */
} /* namespace esa */

#endif /* ESA_EXAMPLES_LOGGER_H_ */
