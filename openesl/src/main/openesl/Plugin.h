#ifndef OPENESL_PLUGIN_H_
#define OPENESL_PLUGIN_H_

#include <esl/plugin/Registry.h>

namespace openesl {
inline namespace v1_6 {

class Plugin final {
public:
	Plugin() = delete;
	static void install(esl::plugin::Registry& registry, const char* data);
};

} /* inline namespace v1_6 */
} /* namespace openesl */

#endif /* OPENESL_PLUGIN_H_ */
