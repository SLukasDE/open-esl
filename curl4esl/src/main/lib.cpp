#include <curl4esl/Module.h>

#include <esl/Module.h>

extern "C" void esl__module__library__install(esl::module::Module* module) {
	if(module != nullptr) {
		curl4esl::Module::install(*module);
	}
}
