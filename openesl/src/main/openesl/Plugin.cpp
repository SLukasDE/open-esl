#include <openesl/Config.hpp>
#include <openesl/Plugin.h>

#include <esl/plugin/Registry.h>
#include <esl/object/Object.h>
#include <esl/object/ProcessingContext.h>
#include <esl/system/TaskFactory.h>

#if ESL_USE_COMMON4ESL == 1
// std4esl
#include <esl/object/IntValue.h>
#include <esl/object/MapStringStringValue.h>
#include <esl/object/SetIntValue.h>
#include <esl/object/SetStringValue.h>
#include <esl/object/StringValue.h>
#include <esl/object/VectorIntValue.h>
#include <esl/object/VectorPairStringStringValue.h>
#include <esl/object/VectorStringValue.h>

// common4esl
#include <esl/monitoring/MemBufferAppender.h>
#include <esl/monitoring/OStreamAppender.h>
#include <esl/monitoring/SimpleLayout.h>
#include <esl/object/ExceptionHandlerProcedure.h>
#include <esl/object/SimpleContext.h>
#include <esl/object/SimpleProcessingContext.h>
#include <esl/system/DefaultTaskFactory.h>
#endif

#if ESL_USE_CURL4ESL == 1
// curl4esl
#include <esl/com/http/client/CURLConnectionFactory.h>
#endif

// logbook4esl
#if ESL_USE_LOGBOOK4ESL == 1
#include <esl/monitoring/LogbookLogging.h>
#endif

// mhd4esl
#if ESL_USE_MHD4ESL == 1
#include <esl/com/http/server/MHDSocket.h>
#endif

// sqlite4esl
#if ESL_USE_SQLITE4ESL == 1
#include <esl/database/SQLiteConnectionFactory.h>
#endif

// odbc4esl
#if ESL_USE_ODBC4ESL == 1
#include <esl/database/ODBCConnectionFactory.h>
#endif

// zsystem4esl
#if ESL_USE_ZSYSTEM4ESL == 1
#include <esl/system/ZSProcess.h>
#include <esl/system/ZSSignalManager.h>
#include <esl/system/ZSStacktraceFactory.h>
#endif

// opengtx4esl
#if ESL_USE_OPENGTX4ESL == 1
#include <esl/crypto/GTXKeyStore.h>
#endif

#include <esa/object/ProcessingContext.h>

#include <memory>

namespace openesl {
inline namespace v1_6 {

namespace {
template <class BaseReturnValue, class ReturnValue, std::unique_ptr<ReturnValue> (*createFunction)(const std::vector<std::pair<std::string, std::string>>&)>
std::unique_ptr<BaseReturnValue> createBase(const std::vector<std::pair<std::string, std::string>>& settings) {
	return createFunction(settings);
}
}

void Plugin::install(esl::plugin::Registry& registry, const char* data) {
	esl::plugin::Registry::set(registry);

#if ESL_USE_COMMON4ESL == 1
	// std4esl
	registry.addPlugin("esl/object/int", esl::object::IntValue::create);
	registry.addPlugin("esl/object/map<string,string>", esl::object::MapStringStringValue::create);
	registry.addPlugin("esl/object/set<int>", esl::object::SetIntValue::create);
	registry.addPlugin("esl/object/set<string>", esl::object::SetStringValue::create);
	registry.addPlugin("esl/object/string", esl::object::StringValue::create);
	registry.addPlugin("esl/object/vector<int>", esl::object::VectorIntValue::create);
	registry.addPlugin("esl/object/vector<pair<string,string>>", esl::object::VectorPairStringStringValue::create);
	registry.addPlugin("esl/object/vector<string>", esl::object::VectorStringValue::create);


	// common4esl
	registry.addPlugin("esl/monitoring/MemBufferAppender", esl::monitoring::MemBufferAppender::create);
	registry.addPlugin("esl/monitoring/OStreamAppender", esl::monitoring::OStreamAppender::create);
	registry.addPlugin("esl/monitoring/SimpleLayout", esl::monitoring::SimpleLayout::create);
	registry.addPlugin("esl/object/ExceptionHandlerProcedure", esl::object::ExceptionHandlerProcedure::create);
	registry.addPlugin("esl/object/SimpleContext", esl::object::SimpleContext::create);
	registry.addPlugin("esl/object/SimpleProcessingContext", esl::object::SimpleProcessingContext::create);
	registry.addPlugin<esa::object::ProcessingContext, esl::object::ProcessingContext, esl::object::SimpleProcessingContext::create>("esl/object/SimpleProcessingContext");
	registry.addPlugin("esl/system/DefaultTaskFactory", esl::system::DefaultTaskFactory::create);
	registry.addPlugin<esl::object::Object, esl::system::TaskFactory, esl::system::DefaultTaskFactory::create>("esl/system/DefaultTaskFactory");
#endif

	// curl4esl
#if ESL_USE_CURL4ESL == 1
	registry.addPlugin("esl/com/http/client/CURLConnectionFactory", esl::com::http::client::CURLConnectionFactory::create);
#endif

	// logbook4esl
#if ESL_USE_LOGBOOK4ESL == 1
	registry.addPlugin("esl/monitoring/LogbookLogging", esl::monitoring::LogbookLogging::create);
#endif


	// mhd4esl
#if ESL_USE_MHD4ESL == 1
	registry.addPlugin("esl/com/http/server/MHDSocket", esl::com::http::server::MHDSocket::create);
#endif


	// sqlite4esl
#if ESL_USE_SQLITE4ESL == 1
	registry.addPlugin("esl/database/SQLiteConnectionFactory", esl::database::SQLiteConnectionFactory::create);
#endif


	// odbc4esl
#if ESL_USE_ODBC4ESL == 1
	registry.addPlugin("esl/database/ODBCConnectionFactory", esl::database::ODBCConnectionFactory::create);
#endif


	// zsystem4esl
#if ESL_USE_ZSYSTEM4ESL == 1
	registry.addPlugin("esl/system/ZSProcess", esl::system::ZSProcess::create);
	registry.addPlugin("esl/system/ZSSignalManager", esl::system::ZSSignalManager::create);
	registry.addPlugin("esl/system/ZSStacktraceFactory", esl::system::ZSStacktraceFactory::create);

	registry.setObject(esl::system::ZSStacktraceFactory::createNative());
#endif


	// opengtx4esl
#if ESL_USE_OPENGTX4ESL == 1
	registry.addPlugin("esl/crypto/GTXKeyStore", esl::crypto::GTXKeyStore::create);
#endif
}

} /* inline namespace v1_6 */
} /* namespace openesl */
