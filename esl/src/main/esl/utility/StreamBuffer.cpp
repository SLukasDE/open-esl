#include <esl/utility/StreamBuffer.h>

namespace esl {
inline namespace v1_6 {
namespace utility {

StreamBuffer::StreamBuffer(std::size_t bufferSize, std::function<void(const char*, std::size_t)> writeHandler)
: std::streambuf(),
  bufferSize(bufferSize),
  buffer(bufferSize+1, 0),
  writeHandler(writeHandler)
{
    /* Calling "setp(buffer, buffer)" set space to zero and this results in calling "int_type overflow(int_type c)" */
    setp(&buffer[0], &buffer[0]);
}

StreamBuffer::int_type StreamBuffer::overflow(int_type c) {
    if (c == traits_type::eof()) {
        return traits_type::eof();
    }

    if(epptr() == &buffer[0]) {
        setp(&buffer[0], &buffer[0] + bufferSize - 1);
    }

    *pptr() = c;
    pbump(1);

    if(sync() == -1) {
        return traits_type::eof();
    }

    return c;
}

int StreamBuffer::sync() {
    char* beginStr = pbase();
    char* endStr = pptr();

    if(beginStr == endStr) {
        return 0;
    }

    char* currentStr = beginStr;
    do {
        char currentChar = *currentStr;
        ++currentStr;

        if (currentChar == '\n') {
            std::ptrdiff_t n = currentStr - beginStr;
            writeHandler(beginStr, n);
            beginStr = currentStr;
        }
    } while(currentStr != endStr);

    if (beginStr != currentStr) {
        std::ptrdiff_t n = currentStr - beginStr;
        writeHandler(beginStr, n);
    }

    setp(&buffer[0], &buffer[0] + bufferSize - 1);
    return 0;
}

} /* namespace utility */
} /* inline namespace v1_6 */
} /* namespace esl */
