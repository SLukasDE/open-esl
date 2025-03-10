/*
 * This file is part of mhd4esl.
 * Copyright (C) 2019-2023 Sven Lukas
 *
 * Mhd4esl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Mhd4esl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with mhd4esl.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <mhd4esl/com/http/server/Connection.h>

#include <esl/io/Reader.h>
#include <esl/Logger.h>
#include <esl/system/Stacktrace.h>

#include <microhttpd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace mhd4esl {
inline namespace v1_6 {
namespace com {
namespace http {
namespace server {

namespace {
esl::Logger logger("mhd4esl::com::http::Connection");
}

Connection::Connection(MHD_Connection& mhdConnection)
: mhdConnection(mhdConnection)
{ }

Connection::~Connection() {
	for(auto& response : responseQueue) {
		MHD_destroy_response(std::get<1>(response));
	}
}

bool Connection::sendQueue() noexcept {
	bool rv = true;

	for(auto& response : responseQueue) {
		rv &= std::get<0>(response)();
		if(rv) {
			responseSent = true;
		}
	}

	return rv;
}

bool Connection::isResponseQueueEmpty() noexcept {
	return responseQueue.empty();
}

bool Connection::hasResponseSent() noexcept {
	return responseSent;
}

bool Connection::send(const esl::com::http::server::Response& response, const void* data, std::size_t size) noexcept {
    MHD_Response* mhdResponse = MHD_create_response_from_buffer(size, const_cast<void*>(data), MHD_RESPMEM_PERSISTENT);

    return sendResponse(response, mhdResponse);
}

bool Connection::send(const esl::com::http::server::Response& response, esl::io::Output output) {
	esl::io::Output* outputPtr = new esl::io::Output(std::move(output));
	MHD_Response* mhdResponse = MHD_create_response_from_callback(-1, 8192, contentReaderCallback, outputPtr, contentReaderFreeCallback);

	return sendResponse(response, mhdResponse);
}

bool Connection::sendFile(const esl::com::http::server::Response& response, const std::string& path) {
    int fd = open(path.c_str(), O_RDONLY);
    if(fd < 0) {
        return false;
    }
    size_t size = static_cast<size_t>(lseek(fd, 0, SEEK_END));
    lseek(fd, 0, SEEK_SET);

    MHD_Response* mhdResponse = MHD_create_response_from_fd(size, fd);

    return sendResponse(response, mhdResponse);
}

bool Connection::sendResponse(const esl::com::http::server::Response& response, MHD_Response* mhdResponse) noexcept {
	if(mhdResponse == nullptr) {
		logger.warn << "- mhdResponse == nullptr\n";
		return false;
	}

	for(const auto& header : response.getHeaders()) {
		MHD_add_response_header(mhdResponse, header.first.c_str(), header.second.c_str());
	}

	std::function<bool()> sendFunc;
	unsigned short httpStatusCode = response.getStatusCode();

	sendFunc = [this, httpStatusCode, mhdResponse]() {
	    return MHD_queue_response(&mhdConnection, httpStatusCode, mhdResponse) == MHD_YES;
	};

	responseQueue.push_back(std::make_tuple(sendFunc, mhdResponse));

	return true;
}

ssize_t Connection::contentReaderCallback(void* cls, uint64_t bytesTransmitted, char* buffer, size_t bufferSize) {
    esl::io::Output* outputPtr = static_cast<esl::io::Output*>(cls);
    if(outputPtr == nullptr) {
        return MHD_CONTENT_READER_END_OF_STREAM;
    }

    std::unique_ptr<esl::system::Stacktrace> stacktrace = nullptr;
    try {
        std::size_t size = outputPtr->getReader().read(buffer, bufferSize);
    	if(size == esl::io::Reader::npos) {
            return MHD_CONTENT_READER_END_OF_STREAM;
        }

        return size;
    }
    catch (std::exception& e) {
    	logger.error << e.what() << std::endl;

    	const esl::system::Stacktrace* stacktracePtr = esl::system::Stacktrace::get(e);
    	if(stacktracePtr) {
            stacktrace = stacktracePtr->clone();
    	}
    }
    catch (...) {
    	logger.error << "unknown exception" << std::endl;
    }

    if(stacktrace) {
        stacktrace->dump(logger.error);
    }
    return MHD_CONTENT_READER_END_WITH_ERROR;
}

void Connection::contentReaderFreeCallback(void* cls) {
    esl::io::Output* outputPtr = static_cast<esl::io::Output*>(cls);

    if(outputPtr) {
        delete outputPtr;
    }
}

} /* namespace server */
} /* namespace http */
} /* namespace com */
} /* inline namespace v1_6 */
} /* namespace mhd4esl */
