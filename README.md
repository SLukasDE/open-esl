# OpenESL

OpenESL is a library that bundles ESL together with some open source implementations that realize the APIs of ESL.

## What is ESL

ESL just provides a bunch of comfortable APIs, but it contains (almost) no implementations. Hence, ESL alone is not enough. You will also link your code to an ESL compatible **implementation** for the ESL **features** you want to use. For example, if you want to use the **HTTP-Client** API of ESL, you can use the **curl4esl** implementation. The implementation (or plugin) **curl4esl** maps calls of the **HTTP-Client** API of ESL to calls of the API of the CURL library. Of course you can write your own implementations and you can use this mechanism as well to MOCK some parts of your code to implementing unit tests, component test etc. Once you want to change the implementation, you don't need to change the code ouf your applicaition. You just change the implementation. That's it.

Your code will compile and link even without linking to an implementation. If you call such an API, an exception will be thrown. There are at least 2 features that *work* without an implementation. This is the feature **stacktrace** and **logger**. You can use these two features always, they just don't have an effect if there is no implementation available.

The APIs of ESL are seperated into 4 different main areas:

* System
* Communication
* Database
* Monitoring

### System

This is the area of APIs that are working on a lower level, like creating a *stacktrace*, working with *signals* and creating *processes*.

#### Stacktrace

A Stacktrace shows you the call stack from the main function to your current position. ESL helps you to put this information to exceptions that you want to throw and it helps you to dump this stacktrace wherever you catch an exception.

#### Signal handling

Applications can receive signals. A popular signal is used to terminate your application. If you don't care about signals, there is already a default signal handler installed that calls `std::terminate()`. But sometimes you want to implement your own signal handler, for example because you want to execute some cleanup code before your close your application. ESL helps you to register your own functions and you can run them in a seperatly thread.

#### Processes

Process handling can be very complicated. This API helps you to execute processes, wait for them, redirect their output and input and send signals to processes.

### Monitoring

This is the area of APIs that includes the features *logging* and *metrics*.

#### Logging

* **Available Log-Levels** are `logger.trace`, `logger.debug`, `logger.info`, `logger.warn` and `logger.error`.
* **Class individual logger** are possible and recommended to use.
* **Filter defintions** can be used to drop irrelevant log messages and let you pass only the messages from specific classes and log levels
* **Release-Builds** can automatically include less log messages at compile time to increase the performance.
* **Appenders** can extend the logging framework e.g. to implement a silent *ring buffer* and flush the output if an error occured or to upload the log data to external logging-servers like *Loki* or *Logscale*.

#### Metrics

Metrics are used to collect continously data about your running app and make them available for other tools, like Prometheus. Common used types of metrics are **counters**, **gauges**, **histograms** and **summaries**.

### Communication

This is the area of APIs that are related for any kind of communication.

#### Basic

This is a sub area of the communication area. The *communication-basic* area is used for TCP/IP, named sockets/unix-sockets and other simple byte-stream bases protocols. The area is seperated into two APIs. One for *clients* and one for *servers*.  

#### HTTP

This is a sub area of the communication area. The *communication-http* area is used for HTTP bases communication. The area is seperated into two APIs. One for *clients* and one for *servers*.  

### Database

ESL provides an easy to use API to access all type of SQL or SQL-similar databases

* **Parameterized query** are easy to realize
* **Bulk support** is available
* **Stored Procedures** are supported

## Quick start

Clone and basic build:

```bash
git clone https://github.com/SLukasDE/open-esl.git
cd open-esl
cmake -S . -B build
cmake --build ./build
```

| Available features/implemenations | CMake argument               |
| --------------------------------- | ---------------------------- |
| common4esl                        | -DOPENESL_USE_COMMON4ESL=ON  |
| logbook4esl                       | -DOPENESL_USE_LOGBOOK4ESL=ON |
| zsystem4esl                       | -DOPENESL_USE_ZSYSTEM4ESL=ON |
| opengtx4esl                       | -DOPENESL_USE_OPENGTX4ESL=ON |
| curl4esl                          | -DOPENESL_USE_CURL4ESL=ON    |
| mhd4esl                           | -DOPENESL_USE_MHD4ESL=ON     |
| sqlite4esl                        | -DOPENESL_USE_SQLITE4ESL=ON  |
| odbc4esl                          | -DOPENESL_USE_ODBC4ESL=ON    |

Example: If you want to build OpenESL with the implementations `logbook4esl` and `curl4esl`:

```bash
cmake -S . -B build -DOPENESL_USE_LOGBOOK4ESL=ON -DOPENESL_USE_CURL4ESL=ON
cmake --build ./build
```

## Tutorial

Here I will show you how to make your first steps with ESL.

* We will start with a simple ***Hello World*** app.
* We will extend this app to get an ***Stacktrace*** if an error occures. The restulting app is the base for the next steps.
* We will use the ***HTTP Client*** API of ESL to call a REST services to get the opening price of a stock from Apple Inc.
* We will extend the App to use the ***Logging*** API and we will show some mechanisms to configure the behavior ot the logger.
* We will prepare our app to make implementations exchangeable. We use the concept of a **Context** to initialize our class with the implementation that we specified outside of this class.
* We will learn how to **Register** our own implementations and how we can use **Dependency Injection** to make switch between implementations.
* We will use the ***Database*** API to load and store the results from the REST service.

### Hello World

In general for ESL there is no special initialization required, even if it is possible. Initialization is done lazy. But *de-initialize* is necessary for ESL in some cases, e.g. to flush log messages and so on. This is done by calling `void esl::plugin::Registry::cleanup()`. If something must be done when leaving a block, then RAII will be your friend.

Now let's start with our first code. Go into a new directory, create a folder `src` and inside of this folder create the source file `main.cpp`:

```cpp
// main.cpp
#include <esl/plugin/Registry.h>

#include <iostream>

int main(int argc, const char* argv[]) {
    struct RegistryGuard {
        ~RegistryGuard() {
            esl::plugin::Registry::cleanup();
        }
    } registryGuard;

    std::cout << "Hello World.\n";

    return 0;
}
```

The only thing we use from ESL is to call the cleanup function. But at least, this requires already to use ESL. So let's make our build scripts and see if it works so far. First we creating a `CMakeLists.txt` file in our main directory:

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.23)

project(
    myapp
    VERSION 1.0.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)
```

Now we create another `CMakeLists.txt` file in our `src` directory:

```cmake
# src/CMakeLists.txt
include(FetchContent)
message(STATUS "Try to find OpenESL by FetchContent")
FetchContent_Declare(
    openesl
    GIT_REPOSITORY https://github.com/slukas_de/openesl
    #GIT_TAG        v1.6.0
    GIT_SHALLOW    TRUE
    OVERRIDE_FIND_PACKAGE
)
find_package(openesl REQUIRED)

file(GLOB_RECURSE ${PROJECT_NAME}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC openesl::openesl)
```

Finally we can build this app like this:

```text
> cmake -S . -B build
> cmake --build build
>
```

Let's run the app:

```text
> ./build/src/myapp
Hello World.
>
```

We see `Hello World.` as expected. And we know that we can use ESL already - even if it is just calling the cleanup function.

We will do some more initialization und error handling stuff in our `main.cpp`. To keep the code readable, let's move our business-code *(currently just `std::cout << "Hello World.\n"`)* to a separate class. We create a new sub directory `myapp` withing the `src`-folder and we put 2 source files into this new folder: `Main.hpp` and `Main.cpp`:

```cpp
// myapp/Main.hpp
#pragma once

namespace myapp {

class Main {
public:
    Main(int argc, const char* argv[]);

    void run();
};

} /* namespace myapp */
```

```cpp
// myapp/Main.cpp
#include <myapp/Main.hpp>

#include <iostream>

namespace myapp {

Main::Main(int argc, const char* argv[]) {
}

void Main::run() {
    std::cout << "Hello world.\n";
}

} /* namespace myapp */
```

Let's go back to our `main.cpp` source file. We have to call the method `run` from our `Main`-class there. But it's also a good idea to catch all kind of exceptions there:

```cpp
// main.cpp
#include <myapp/Main.hpp>

#include <openesl/Plugin.h>

#include <esl/plugin/Registry.h>

#include <iostream>
#include <stdexcept>

int main(int argc, const char* argv[]) {
    struct RegistryGuard {
        ~RegistryGuard() {
            esl::plugin::Registry::cleanup();
        }
    } registryGuard;

    int rc = -1;

    try {
        openesl::Plugin::install(esl::plugin::Registry::get(), nullptr);

        myapp::Main myMain(argc, argv);
        myMain.run();
        rc = 0;
    }
    catch(const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << "\n";
    }
    catch(...) {
        std::cerr << "Unknown exception occurred.\n";
    }

    return rc;
}
```

### Stacktrace

As long as we are developing our software we can use a debugger to analyse errors. And of course, we don't want to deploy an incomplete software into the production environment. That's why there are many stages our software has to pass before it goes into production. But the tools to analyse your software change on the way from development stage to production stage, e.g. you cannot use your debugger on further stages. And even if your software passed all the stages and their tests, errors can still happen in production. In this case you are happy about every kind of information that occurred the error. A very helpful information is having a stackstrace. So you will know in which line of code your application crashed and which function called this part.

Another useful hint is to print detailed information of all exceptions. It can be helpful to know the type of exception that has been thrown. But much more useful are the addition information an excection can provide. ESL defines some own exception classes that provide more information than just giving a message by calling `what()`.

So, let's print the Stacktrace (if available) and catch some more excetions in our `main.cpp`:

```cpp
// main.cpp
#include <myapp/Main.hpp>

#include <openesl/Plugin.h>

#include <esl/com/http/client/exception/NetworkError.h>
#include <esl/database/exception/SqlError.h>
#include <esl/plugin/exception/PluginNotFound.h>
#include <esl/plugin/Registry.h>
#include <esl/system/Stacktrace.h>

#include <iostream>
#include <stdexcept>

int main(int argc, const char* argv[]) {
    struct RegistryGuard {
        ~RegistryGuard() {
            esl::plugin::Registry::cleanup();
        }
    } registryGuard;

    int rc = -1;

    try {
        openesl::Plugin::install(esl::plugin::Registry::get(), nullptr);

        myapp::Main myMain(argc, argv);
        myMain.run();
        rc = 0;
    }
    catch(const esl::com::http::client::exception::NetworkError& e) {
        std::cerr << "HTTP client exception occurred: " << e.what() << "\n";
        std::cerr << "- Error code: " << e.getErrorCode() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const esl::database::exception::SqlError& e) {
        std::cerr << "SQL error exception occurred: " << e.what() << "\n";
        std::cerr << "- SQL return code: " << e.getSqlReturnCode() << "\n";
        e.getDiagnostics().dump(std::cerr);
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const esl::plugin::exception::PluginNotFound& e) {
        std::cerr << "Plugin not found exception occurred: " << e.what() << "\n";
        const esl::plugin::Registry::BasePlugins& basePlugins = esl::plugin::Registry::get().getPlugins(e.getTypeIndex());
        if(basePlugins.empty()) {
            std::cerr << "No implementations available.\n";
        }
        else {
            std::cerr << "Implementations available:\n";
            for(const auto& basePlugin : basePlugins) {
                std::cerr << "- " << basePlugin.first << "\n";
            }
        }
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Exception of type 'std::runtime_error' occurred: " << e.what() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Exception of type 'std::exception' occurred: " << e.what() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(...) {
        std::cerr << "Unknown exception occurred.\n";
    }

    return rc;
}
```

Let's throw an runtime error with stacktrace if we call our app with unknown arguments. We will add this code in our main class:

```cpp
// myapp/Main.cpp
#include <myapp/Main.hpp>

#include <esl/system/Stacktrace.h>

#include <iostream>
#include <stdexcept>

namespace myapp {

Main::Main(int argc, const char* argv[]) {
    if(argc > 1) {
        throw esl::system::Stacktrace::add(std::runtime_error("Our application has been called with unknown arguments"));
    }
}

void Main::run() {
    std::cout << "Hello world.\n";
}

} /* namespace myapp */
```

We compile our app same as before. Then we run this app once without arguments and another time with argument `foo`:

```text
> cmake --build build
> ./build/src/myapp
Hello World.
> ./build/src/myapp foo
Exception of type 'std::runtime_error' occurred: Our application has been called with arguments but it doesn't allow arguements
>
```

We can see that our exception has been thrown and it has been catched as well. But where is our stacktrace? ESL is almost just an definition of several APIs. Implementations are not part of ESL. Everyone is invited to develop implementations for some APIs of ESL. But now we don't want to develop these implementations, but we want to use existing implementations.  
OpenESL provides implementations for every API but we have to activate these implementations. All these implementations are working on Linux but some are not working on Windows and MacOS so far. The implementation `zsystem4esl` contains several implementations for ESL-APIs and especially it contains an implementatin for the stacktrace API. But `zsystem4esl` is one on the implementations, that does not work on windows so far. We didn't activate this implementation so far, but we could still use the API. Only it has had no effect.

Anyway, let's say we are on Linux. So let's activate this feature. Now initialization is necessary and we cannot drop the line `openesl::Plugin::install(esl::plugin::Registry::get(), nullptr);` anymore in your `main.cpp`. This call registers all available implementations to the ESL library. To activate the implementation `zsystem4esl` that provides an implementatin for the stracktrace API, we have to recompile our code like this:

```text
> cmake -S . -B build -DOPENESL_USE_ZSYSTEM4ESL=ON
> cmake --build build
>
```

Now we can run our app again and we will see something like this:

```text
> ./build/src/myapp foo
Exception of type 'std::runtime_error' occurred: Our application has been called with arguments but it doesn't allow arguements
./build/src/myapp(+0xad14) [0x5571e83e4d14]
./build/src/myapp(+0x725f) [0x5571e83e125f]
>
```

This is a very basic implementation of the stacktrace API. You can use the tool `addr2line` to get the line-numbers our your code. Later we will see another implementation that shows a real nice stacktrace with filename, line number, etc.

### HTTP Client

Now we understand ESL and OpenESL much better. So let's start with the first real exciting app. We would like to fetch the daily stock opening price of Apple Inc. (AAPL). We will call a REST service to get the data as JSON and we print the result to the console.

We extend out source file `src/myapp/Main.hpp` and  `src/myapp/Main.cpp`:

```cpp
// myapp/Main.hpp
#pragma once

#include <esl/com/http/client/ConnectionFactory.h>

#include <memory>

namespace myapp {

class Main {
public:
    Main(int argc, const char* argv[]);

    void run();

private:
    std::unique_ptr<esl::com::http::client::ConnectionFactory> httpClientConnectionFactory;
};

} /* namespace myapp */
```


```cpp
// myapp/Main.cpp
#include <myapp/Main.hpp>

#include <esl/com/http/client/CURLConnectionFactory.h>
#include <esl/com/http/client/Request.h>
#include <esl/com/http/client/Response.h>
#include <esl/io/input/String.h>
#include <esl/io/Output.h>
#include <esl/system/Stacktrace.h>
#include <esl/utility/HttpMethod.h>
#include <esl/utility/MIME.h>

#include <iostream>
#include <stdexcept>

using namespace esl::com::http::client;
using namespace esl::system;

namespace myapp {

Main::Main(int argc, const char* argv[])
: httpClientConnectionFactory(CURLConnectionFactory::create({{{"url"}, {"https://eodhd.com"}}}))
{
    if(argc > 1) {
        throw Stacktrace::add(std::runtime_error("Our application has been called with arguments but it doesn't allow arguements"));
    }
}

void Main::run() {
    std::cout << "Hello world.\n";

    if(!httpClientConnectionFactory) {
        throw Stacktrace::add(std::runtime_error("HTTP client connection-factory is not initialized"));
    }

    std::unique_ptr<Connection> c = httpClientConnectionFactory->createConnection();
    if(!c) {
        throw Stacktrace::add(std::runtime_error("Could not create connection"));
    }

    esl::io::input::String strInput;
    Request request("api/real-time/AAPL.US?api_token=demo&fmt=json", esl::utility::HttpMethodType::httpGet, esl::utility::MIME());
    Response response = c->send(request, esl::io::Output(), strInput);
    if(response.getStatusCode() < 200 || response.getStatusCode() >= 300) {
        throw Stacktrace::add(std::runtime_error("Request failed with status code " + std::to_string(response.getStatusCode())));
    }

    std::cout << strInput.getString() << '\n';

    /* Here we could make another request to "eodhd.com" with the same connection object 'c'. */
}

} /* namespace myapp */
```

We can see a lot of new stuff that I want to explain. We create an instance of the class `esl::com::http::client::CURLConnectionFactory` by calling it's static method `create`. We store the instance with a unique-pointer to the base class `esl::com::http::client::ConnectionFactory`. Thus, creating the instance was the only part where we have been specific to a special implementation. Later I will show you how to get rid of using `CURLConnectionFactory` as well, so that we can really exchange the implementation without touching the code.

In ESL you will see this **factory pattern** of calling a **create-method** many times. This is an important pattern to make implementations exchangeable. Next we create an instance of `esl::com::http::client::Connection`. As long as you want to make calles to the **same host** within the **same thread**, you can use the same connection instance. So, you don't need to create a new connection if you want to make a second request.

To send a request to the host, we use the method `send` of our connection object. `send` expects 3 arguments:

1. `esl::com::http::client::Request`
2. `esl::io::Output`
3. `esl::io::Input`

1: The `Request` object holds all the data we can put into a **HTTP request header**. That's the `path` of our URL of course, but we have to specify the HTTP method as well. Valid HTTP methods are `GET`, `PUT`, `POST`, `DELETE`, `HEAD`, `PATCH`, `OPTIONS`, ... In theory it can be every string. If we send a payload, e.g. by using HTTP `POST`, then we will have to specify the MIME type of our payload as well. But in our case we are sending a HTTP `GET` request and we don't send a payload. Thus, we use an **empty** (= *default constructed*) MIME type.
Finally there is an optional fourth argument. It's a `std::map<std::string, std::string>` to give arbitrary additional header parameters, like API key and so on.

2: Let's come back to the `send` method. We have prepared already the `Request` object for the first argument of this method. Next, the method need's an `esl::io::Output` object. In general this object is used to provide data for a consumer. If we send a HTTP `POST` request, then we would use this parameter to transmit our **request payload**. But for now, we don't transmit any payload, so we can use an **empty** `esl::io::Output` object.

3: The third argument is an `esl::io::Input` object. This object is used to receive the **response payload** after we sent the request. The classes `esl::io::Input` and `esl::io::Output` have an  default constructor but if you are interested into the input/output-data, then there are 2 other constructors. In case of `esl::io::Input` we use the constructor that takes the argument `esl::io::Writer& writer`. There is already a class available that implements `esl::io::Writer`. It's the class `esl::io::input::String`. This class stores the receiced data into a string. You can get the string by calling its method `getString()`.

Finally the method `send` returns an object of the type `esl::com::http::client::Response`. This object contains the headers of the HTTP response message. This is the HTTP status code (`getStatusCode()`), the MIME type of the response payload (`getContentType()`) and additional headers if available (`getHeaders()`).

Later, if we will parse the response payload and we will check the MIME type of the response payload. Sometimes it's necessary to be liberal with the data from other parties. E.g. if there is no MIME type given, then just try to parse the data as JSON, but catch the excpetion of your parser. On the other side, if the MIME type says, it's an XML format, then don't use your JSON parser.

Now let's compile and run the code:

```text
# On Linux you can use
# cmake -S . -B build -DOPENESL_USE_CURL4ESL=ON -DOPENESL_USE_ZSYSTEM4ESL=ON

> cmake -S . -B build -DOPENESL_USE_CURL4ESL=ON
> cmake --build build
> ./build/src/myapp
Hello World.
{"code":"AAPL.US","timestamp":1729089540,"gmtoffset":0,"open":231.6,"high":232.12,"low":229.84,"close":230.41,"volume":11260071,"previousClose":233.85,"change":-3.44,"change_p":-1.471}
>
```

Be aware, that you activated the implementation `curl4esl` by giving the argument `-DOPENESL_USE_CURL4EL=ON`. Now let's activate all the features we need in our build scripts, so that we don't have to give this extra arguments all the time when compiling your code. We will activate some more implemenations as well, so that we don't need to edit your build scripts anymore for this tutorial.
Edit the file `src/CMakeLists.txt`:

```cmake
include(FetchContent)

if(NOT openesl_FOUND)
    message(STATUS "Try to find OpenESL by FetchContent")
    set(OPENESL_USE_COMMON4ESL ON)
    set(OPENESL_USE_CURL4ESL ON)
    set(OPENESL_USE_SQLITE4ESL ON)
    set(OPENESL_USE_LOGBOOK4ESL ON)
    # on linux:
    # set(OPENESL_USE_ZSYSTEM4ESL ON)
    FetchContent_Declare(
        openesl
        GIT_REPOSITORY https://github.com/slukasde/open-esl
        #GIT_TAG        v1.6.0
        GIT_SHALLOW    TRUE
        OVERRIDE_FIND_PACKAGE # 'find_package(...)' will call 'FetchContent_MakeAvailable(...)'
    )
    find_package(openesl REQUIRED)
endif()

if(NOT nlohmann_json_FOUND)
    message(STATUS "Try to find nlohmann_json by FetchContent")
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.11.3
        GIT_SHALLOW TRUE
        OVERRIDE_FIND_PACKAGE # 'find_package(...)' will call 'FetchContent_MakeAvailable(...)'
    )
    find_package(nlohmann_json REQUIRED)
endif()

file(GLOB_RECURSE ${PROJECT_NAME}_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC openesl::openesl nlohmann_json::nlohmann_json)
```

And now let's update the files `src/myapp/Main.hpp` and `src/myapp/Main.cpp` to parse the JSON data:


```cpp
// myapp/Main.hpp
#pragma once

#include <esl/com/http/client/ConnectionFactory.h>

#include <memory>
#include <tuple>

namespace myapp {

class Main {
public:
    Main(int argc, const char* argv[]);

    void run();

    std::tuple<int, float> fetchOpeningPriceToday();
    
private:
    std::unique_ptr<esl::com::http::client::ConnectionFactory> httpClientConnectionFactory;
};

} /* namespace myapp */
```

```cpp
// myapp/Main.cpp
#include <myapp/Main.hpp>

#include <esl/com/http/client/CURLConnectionFactory.h>
#include <esl/com/http/client/Request.h>
#include <esl/com/http/client/Response.h>
#include <esl/io/input/String.h>
#include <esl/io/Output.h>
#include <esl/system/Stacktrace.h>
#include <esl/utility/HttpMethod.h>
#include <esl/utility/MIME.h>

#include <nlohmann/json.hpp>

#include <ctime>
#include <iostream>
#include <stdexcept>

using namespace esl::com::http::client;
using namespace esl::system;

namespace myapp {

Main::Main(int argc, const char* argv[])
: httpClientConnectionFactory(CURLConnectionFactory::create({{{"url"}, {"https://eodhd.com"}}}))
{
    if(argc > 1) {
        throw Stacktrace::add(
            std::runtime_error(
                "Our application has been called with arguments but it doesn't allow arguements"));
    }
}

void Main::run() {
    std::cout << "Hello world.\n";

    int date;
    float openingPrice;
    std::tie(date, openingPrice) = fetchOpeningPriceToday();
    if(openingPrice < 0) {
        std::cerr << "No service\n";
        return;
    }

    std::cout << "opening price at " << date << ": " << openingPrice << std::endl;
}

std::tuple<int, float> Main::fetchOpeningPriceToday()
{
    if(!httpClientConnectionFactory) {
        throw Stacktrace::add(
            std::runtime_error(
                "HTTP client connection-factory is not initialized"));
    }

    std::unique_ptr<Connection> connection = httpClientConnectionFactory->createConnection();
    if(!connection) {
        throw Stacktrace::add(std::runtime_error("Could not create HTTP client connection"));
    }

    esl::io::input::String strInput;
    Request request("api/real-time/AAPL.US?api_token=demo&fmt=json", esl::utility::HttpMethodType::httpGet, esl::utility::MIME());
    Response response = connection->send(request, esl::io::Output(), strInput);
    if(response.getStatusCode() < 200 || response.getStatusCode() >= 300) {
        throw Stacktrace::add(std::runtime_error("Request failed with status code " + std::to_string(response.getStatusCode())));
    }

    std::cout << "JSON result: " << strInput.getString() << "\n";

    auto js = nlohmann::json::parse(strInput.getString());

    float openingPrice = 0;
    js.at("open").get_to(openingPrice);
    std::cout << "open price is " << openingPrice << std::endl;

    std::tm tm;
    std::time_t timestamp;
    js.at("timestamp").get_to(timestamp);
#ifdef _WIN32
    localtime_s(&tm, &timestamp);
#else
    localtime_r(&timestamp, &tm);
#endif
    int date = ((tm.tm_year + 1900) * 10000) + (tm.tm_mon * 100) + tm.tm_mday;
    std::cout << "date is " << date << std::endl;

    std::cout << "opening price at " << tm.tm_year + 1900 << "/" << tm.tm_mon << "/" << tm.tm_mday << ": " << openingPrice << std::endl;

    return std::make_tuple(date, openingPrice);
}

} /* namespace myapp */
```

Let's compile and run the code:

```text
> cmake -S . -B build
> cmake --build build
> ./build/src/myapp
Hello world.
JSON result: {"code":"AAPL.US","timestamp":1729528620,"gmtoffset":0,"open":234.45,"high":236.61,"low":234.45,"close":236.29,"volume":17480877,"previousClose":235,"change":1.29,"change_p":0.5489}
open price is 234.45
date is 20240921
opening price at 2024/9/21: 234.45
opening price at 20240921: 234.45
>
```

It works and we can observe a lot of output.

### Logging

For analysis and debug reasons, it is helpful to provide a lot of information to the output *(and for **security reasons** it's good NOT to provide much information to the output :-) )*. But sometimes it is too much output in a complex software. So we need some guidence here as well.

First we should seperate our output into different levels. For example some output we want to see always, like **errors** and maybe **warnings** as well. Some other output we only want to see if we need to analyse the running code very deep. These levels are **trace** and **debug**.

Another dimension is to seperate our output into the source where it comes from. This could be the source file. In object oriented programming languages the **class** is an important dimension as well. Personaly I like to structure the code, that I use for each class exactly one `.cpp`-file and one `.hpp`-file that have the same name as the class. And the namespace is mapped by the directory hierarchie. Then, the file name is equal to the full qualified class name.

The ESL logger supports this levels (in this order):
* error
* warning
* info
* debug
* trace

You can tell the logger for example:
* in general (*= for all classes*) you want to see only log messages up to level *warning*
* but for class `myapp::Main` you want to see all log messages (= up to *trace*)
* and for all other classes in the namespace myapp (`myapp::*`) we want to see log messages at least up to *debug*.

And finally we don't want to have log messages higher than *warning* in our code in general, if we make a release build.

First create a new file `src/myapp/Logger.hpp`:

```cpp
#include <esl/monitoring/Streams.h>
#include <esl/monitoring/Logger.h>

namespace myapp {

#ifdef NDEBUG
using Logger = esl::monitoring::Logger<esl::monitoring::Streams::Level::warn>;
#else
using Logger = esl::monitoring::Logger<esl::monitoring::Streams::Level::trace>;
#endif

} /* namespace myapp */
```
We defined a class named `Logger` that we can instantiate. It is very cheap to create an instance of these class. The class contains only 6 public members. The constructor expects an argument `const char* classname` and if forwards this argument to the members. The construction of these members is cheap as well. They just store their constructor arguments into private members, that are two pointer and one enum.  the constructor stores this pointer to the class name as class member - that's it. If you call methods or if you use members of this instance, then it will have some costs. But again, it doesn't cost to create an instance. So, don't be afraid of put one logger instance into every `.cpp`-file.

Now let's use this class in our file `src/myapp/Main.cpp`:


```cpp
// myapp/Main.cpp
#include <myapp/Logger.hpp>
#include <myapp/Main.hpp>

#include <esl/com/http/client/CURLConnectionFactory.h>
#include <esl/com/http/client/Request.h>
#include <esl/com/http/client/Response.h>
#include <esl/io/input/String.h>
#include <esl/io/Output.h>
#include <esl/system/Stacktrace.h>
#include <esl/utility/HttpMethod.h>
#include <esl/utility/MIME.h>

#include <nlohmann/json.hpp>

#include <ctime>
#include <iostream>
#include <stdexcept>

using namespace esl::com::http::client;
using namespace esl::system;

namespace myapp {

namespace {
Logger logger("myapp::Main");
}

Main::Main(int argc, const char* argv[])
: httpClientConnectionFactory(CURLConnectionFactory::create({{{"url"}, {"https://eodhd.com"}}}))
{
    if(argc > 1) {
        throw Stacktrace::add(
            std::runtime_error(
                "Our application has been called with arguments but it doesn't allow arguements"));
    }
}

void Main::run() {
    logger.info << "Hello world.\n";

    int date;
    float openingPrice;
    std::tie(date, openingPrice) = fetchOpeningPriceToday();
    if(openingPrice < 0) {
        logger.warn << "No service\n";
        return;
    }

    logger.debug << "opening price at " << date << ": " << openingPrice << std::endl;
}

std::tuple<int, float> Main::fetchOpeningPriceToday()
{
    if(!httpClientConnectionFactory) {
        throw Stacktrace::add(
            std::runtime_error(
                "HTTP client connection-factory is not initialized"));
    }

    std::unique_ptr<Connection> connection = httpClientConnectionFactory->createConnection();
    if(!connection) {
        throw Stacktrace::add(
            std::runtime_error(
                "Could not create HTTP client connection"));
    }

    esl::io::input::String strInput;
    Request request(
        "api/real-time/AAPL.US?api_token=demo&fmt=json",
        esl::utility::HttpMethodType::httpGet,
        esl::utility::MIME());
    Response response = connection->send(request, esl::io::Output(), strInput);
    if(response.getStatusCode() < 200 || response.getStatusCode() >= 300) {
        throw Stacktrace::add(
            std::runtime_error(
                "Request failed with status code " + std::to_string(response.getStatusCode())));
    }

    logger.debug << "JSON result: " << strInput.getString() << "\n";

    auto js = nlohmann::json::parse(strInput.getString());

    float openingPrice = 0;
    js.at("open").get_to(openingPrice);
    logger.trace << "open price is " << openingPrice << std::endl;

    std::tm tm;
    std::time_t timestamp;
    js.at("timestamp").get_to(timestamp);
#ifdef _WIN32
    localtime_s(&tm, &timestamp);
#else
    localtime_r(&timestamp, &tm);
#endif
    int date = ((tm.tm_year + 1900) * 10000) + (tm.tm_mon * 100) + tm.tm_mday;
    logger.trace << "date is " << date << std::endl;

    logger.trace << "opening price at "
        << tm.tm_year + 1900 << "/" << tm.tm_mon << "/" << tm.tm_mday
        << ": " << openingPrice << std::endl;

    return std::make_tuple(date, openingPrice);
}

} /* namespace myapp */
```

We created an instance of `myapp::Logger` as variable `logger` and gave the C string `"myapp::Main"` into the constructor. Then we replaced `std::cout` with `logger.warn`, `logger.info`, `logger.debug` and `logger.trace`. That's how the use the logger already. Easy, right?

Let's go back to the new header file. You might ask why there are two different definition depending on the definition of `NDEBUG`. The first defintion creates a logger class that does not care about the text you are sending to the streams **after** `warn`. Only the text sent to the stream `error` and `warn` is processed. The streams `info`, `debug` and `trace` have an empty implementation. Thus, the optimizer will remove this code.

> Of course the compiler cannot drop function calls like this:
> ```cpp
> logger.info << "Random number: " << rand() << std::endl;
> ```
> In general the compiler has to call the function `rand()`, even if you don't care about the return value. If you want to call the the funktion `rand()` only if the `info` stream is getting processed, then you can check if the stream is getting processed:  
> ```cpp
> if(logger.info)
> {
>    logger.info  << "Random number: " << rand() << std::endl;
> }
> ```

Now we know how to ***use*** the logger and you can compile this code already. But you will see no output. This is because we haven't ***initialized*** the logger so far. This we will do in our `src/main.cpp` file. Add these additional includes:

```cpp
#include <esl/monitoring/LogbookLogging.h>
#include <esl/monitoring/OStreamAppender.h>
#include <esl/monitoring/SimpleLayout.h>
#include <esl/monitoring/Streams.h>
```

Then go into the `try` block and write this:

```cpp
    try {
        esl::plugin::Registry& registry(esl::plugin::Registry::get());
        openesl::Plugin::install(registry, nullptr);

        auto logging = esl::monitoring::LogbookLogging::createNative();

        logging->setLevel(esl::monitoring::Streams::Level::warn, "*");
        logging->setLevel(esl::monitoring::Streams::Level::trace, "myapp::Main");
        logging->setLevel(esl::monitoring::Streams::Level::debug, "myapp::*");

        {
            auto layout = esl::monitoring::SimpleLayout::create({});
            logging->addLayout("default-layout", std::move(layout));
        }

        {
            auto appender = esl::monitoring::OStreamAppender::create({
                {"trace", "out"},
                {"debug", "out"},
                {"info", "out"},
                {"warn", "out"},
                {"error", "err"}});
            logging->addAppender("", "default-layout", std::move(appender));
        }

		registry.setObject(std::move(logging));

        myapp::Main myMain(argc, argv);
        myMain.run();
        rc = 0;
    }
```

We create an instance of `esl::monitoring::LogbookLogging` and we store it into a variable `logging` with thetype `std::unique_ptr<esl::monitoring::Logging>`. At the end of this initialization process we transfer the ownership of this instance to the **ESL registry** by calling `registry.setObject(std::move(logging));`. After that the logger with use this instance form the ESL registry. But first we have to setup this object.

Let's remeber our requirements:

> You can tell the logger for example:
> * in general (*= for all classes*) you want to see only log messages up to level *warning*
> * but for class `myapp::Main` you want to see all log messages (*= up to trace*)
> * and for all other classes in the namespace myapp (`myapp::*`) we want to see log messages at least up to *debug*.

I think it is very clear that this is exaclty doing this code:

```cpp
logging->setLevel(esl::monitoring::Streams::Level::warn, "*");
logging->setLevel(esl::monitoring::Streams::Level::trace, "myapp::Main");
logging->setLevel(esl::monitoring::Streams::Level::debug, "myapp::*");
```

There are different ways to format a log message. E.g. you could print as preamble to each line the log-level of the message, the class name, the thread id, line number, file etc. This is specified by a **Layout**. So, we create an instance of `esl::monitoring::SimpleLayout` and we add it with the name `"default-layout"` to the logging object. Now this layout just exists and it can be found by the name `"default-layout"`, but it is not used so far.

Now we come to the final part. The layout just defines **how** the log messages should look like. But with an **Appender** we define **where** to print a log message. We create an instance of `esl::monitoring::OStreamAppender` to print log messages with level `trace`,  `debug`,  `info` and  `warn` to `std::out`, but log messages with log level `error` are printed to `std::cerr`.

Finally we add this appender to our logging object with and empty name `""`, but the connect it the layout `"default-layout"`.

This was a lot of code. But we don't want to recompile our app everytime if we change our logging defintion. I wanted to show you this stepts to understand how to make your configuration by yourself, if needed. The implemetation `logbook4esl` provides already an option to load your configuration from a file. First let us update and shrink our `main.cpp`:

```cpp
// main.cpp
#include <myapp/Main.hpp>

#include <openesl/Plugin.h>

#include <esl/com/http/client/exception/NetworkError.h>
#include <esl/database/exception/SqlError.h>
#include <esl/monitoring/LogbookLogging.h>
#include <esl/plugin/exception/PluginNotFound.h>
#include <esl/plugin/Registry.h>
#include <esl/system/Stacktrace.h>

#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, const char* argv[]) {
    struct RegistryGuard {
        ~RegistryGuard() {
            esl::plugin::Registry::cleanup();
        }
    } registryGuard;

    int rc = -1;

    try {
		esl::plugin::Registry& registry(esl::plugin::Registry::get());
        openesl::Plugin::install(registry, nullptr);

		auto logging = esl::monitoring::LogbookLogging::createNative();
        std::ifstream f("logger.xml");
        if(f.good())
        {
            logging->addFile("logger.xml");
        }
        f.close();
		registry.setObject(std::move(logging));

        myapp::Main myMain(argc, argv);
        myMain.run();
        rc = 0;
    }
    ...
}
```

After creating the logging object we try to open a file `logger.xml`. If this file exists, then we will tell the logging object to load this file, that contains our logger configuration. Let's take a look to the file `logger.xml`:

```xml
<esl-logger>
    <setting scope="*" level="warn"/>
    <setting scope="myapp::Main" level="trace"/>
    <setting scope="myapp::*" level="debug"/>

    <layout id="default-layout" implementation="esl/monitoring/SimpleLayout">
        <parameter key="show-timestamp" value="true"/>
        <parameter key="show-level"     value="true"/>
        <parameter key="show-typename"  value="true"/>
        <parameter key="show-address"   value="false"/>
        <parameter key="show-file"      value="false"/>
        <parameter key="show-function"  value="false"/>
        <parameter key="show-line-no"   value="false"/>
        <parameter key="show-thread-no" value="false"/>
    </layout>

    <appender implementation="esl/monitoring/OStreamAppender" record="selected" layout="default-layout">
        <parameter key="trace" value="out"/>
        <parameter key="debug" value="out"/>
        <parameter key="info"  value="out"/>
        <parameter key="warn"  value="out"/>
        <parameter key="error" value="err"/>
    </appender>

    <appender implementation="esl/monitoring/MemBufferAppender" record="all" layout="default-layout">
        <parameter key="max-lines" value="100"/>
    </appender>
</esl-logger>
```

Let's run our app again:

```text
> cmake -S . -B build
> cmake --build build
> ./build/src/myapp
> $ 2024-10-22 15:17:24 [INFO ] (myapp::Main         ): Hello world.
> $ 2024-10-22 15:17:25 [DEBUG] (myapp::Main         ): JSON result: {"code":"AAPL.US","timestamp":1729602120,"gmtoffset":0,"open":0,"high":0,"low":0,"close":234.12,"volume":1629662,"previousClose":236.48,"change":-2.36,"change_p":-0.998}
> $ 2024-10-22 15:17:25 [TRACE] (myapp::Main         ): open price is 0
> $ 2024-10-22 15:17:25 [TRACE] (myapp::Main         ): date is 20240922
> $ 2024-10-22 15:17:25 [TRACE] (myapp::Main         ): opening price at 2024/9/22: 0
> $ 2024-10-22 15:17:25 [DEBUG] (myapp::Main         ): opening price at 20240922: 0
>
```

### Context

A fundamental concept of ESL is to decouple your code from specific implementations as much as possible. Our code is already almost independent of specific implementations. But there are 3 lines of code that are outside to pure metal ESL:

1. in `src/main.cpp` there is the line `openesl::Plugin::install(registry, nullptr);`
2. in `src/main.cpp` there is the line `auto logging = esl::monitoring::LogbookLogging::createNative();`
3. in `src/myapp/Main.cpp` there is the line `: httpClientConnectionFactory(CURLConnectionFactory::create(...))`

The first two locations in `src/main.cpp` are not problematic. You can always use `openesl` in addition to another imlementations. If you want to change the implementation of the logger, then you will have to change only one line of code in `src/main.cpp`.

The third location in `src/myapp/Main.cpp` has much more impact. Imagine you are writing a huge, complex software. Then you are communicating with more than one host. Thus, you need more then one instance of `esl::com::http::client::ConnectionFactory`. And you will use one ore more databases. Thus, you need at least one insacne of `esl::database::ConnectionFactory` as well. It's obvious to create these instances at one central location. So, let's do it as well in `src/main.cpp`. Now, you have to give your `Main` all the reference to these object. As long there is only one object, you can add an additional argument of type `esl::com::http::client::ConnectionFactory &` to the constructor of `myapp::Main`. If you need to create many connection factories, then the number of arguments are increasing. Instead, we will pack all of these instances into another object and then we make this object available to `myapp::Main`. We call this object a **context** and the base class is `esl::object::Context`. You can imagine this class as a `std::map` with  `std::string` as key and the instance as value. Let's create the context that contains the instance. First we have to include an additional header file in `main.cpp`:

```cpp
#include <esl/object/SimpleContext.h>
```

The class `esl::object::SimpleContext` is an implementation of the base class `esl::object::Context`.  
In our `try` block we add this code:

```cpp
esl::object::SimpleContext context;
std::unique_ptr<esl::com::http::client::ConnectionFactory> httpClientConnectionFactory =
    esl::com::http::client::CURLConnectionFactory::create({
            {"url", "https://eodhd.com"}
        });
context.addObject("stock-api-host", std::move(httpClientConnectionFactory));
```

Now we have to make the instance of `context` available to the class `myapp::Main`. As discussed before, we could add an additional argument of type `esl::object::Context &` to the constructor of `myapp::Main`. This will work for our simple app. I want to show you another concept. Before, let's take a look to the previous code, when we initialized the logger manually. We created a layout like this:

```cpp
auto layout = esl::monitoring::SimpleLayout::create({
    });
```

And we created an appender like this:

```cpp
auto appender = esl::monitoring::OStreamAppender::create({
        {"trace", "out"},
        {"debug", "out"},
        {"info", "out"},
        {"warn", "out"},
        {"error", "err"}
    });
```

These two static methods have been registered in the **ESL registry** with an string identifier by calling `openesl::Plugin::install(registry, nullptr);`. Later, we initialized the logger by loading an external configuration file. This configuration file is not limited to only one layout and appender. You can specify any layout and appender as long this identifier string is available in the registry.  
Finally let's take a look how we created the connection factory:

```cpp
httpClientConnectionFactory = CURLConnectionFactory::create({
        {"url", "https://eodhd.com"}
    });
```

All of these classes have an uniform interface to create an instance:
```cpp
static std::unique_ptr<BaseOfT> T::create(const std::vector<std::pair<std::string, std::string>>& settings);
```

I would like to implement this method for our class `myapp::Main` as well. Even though this pattern may turn out to be very useful, it initially prevents us from passing the context to the class when it is created. So, we need another method to forward the context to our instance we have already created. This method is called `initialContext` and it is declared in a base class `esl::object::InitializeContext`. Finally, I would like to define another method as well, even though we are not using it yet. The method is called `procedureRun` and it is declared in a base class `esl::object::Procedure`. Let's see the new code:

```cpp
// myapp/Main.hpp
#include <esl/com/http/client/ConnectionFactory.h>
#include <esl/object/Context.h>
#include <esl/object/InitializeContext.h>
#include <esl/object/Procedure.h>

#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace myapp {

class Main
: public esl::object::InitializeContext,
  public esl::object::Procedure
{
public:
    Main(const std::vector<std::pair<std::string, std::string>>& settings);
    Main(const Main&) = delete;
    Main(Main&&) = delete;

    static std::unique_ptr<Main> create(
        const std::vector<std::pair<std::string, std::string>>& settings);
	void initializeContext(esl::object::Context& context) override;
    void procedureRun(esl::object::Context& context) override;
    
    void run();

    std::tuple<int, float> fetchOpeningPriceToday();

private:
    esl::com::http::client::ConnectionFactory& getHTTPClientConnectionFactory();
    esl::com::http::client::ConnectionFactory* httpClientConnectionFactory = nullptr;
    std::string httpClientConnectionFactoryId;
};

} /* namespace myapp */
```

```cpp
// myapp/Main.cpp
#include <myapp/Logger.hpp>
#include <myapp/Main.hpp>

#include <esl/com/http/client/Request.h>
#include <esl/com/http/client/Response.h>
#include <esl/io/input/String.h>
#include <esl/io/Output.h>
#include <esl/system/Stacktrace.h>
#include <esl/utility/HttpMethod.h>
#include <esl/utility/MIME.h>

#include <nlohmann/json.hpp>

#include <ctime>
#include <iostream>
#include <stdexcept>

using namespace esl::com::http::client;
using namespace esl::system;

namespace myapp {

namespace {
Logger logger("myapp::Main");
}

Main::Main(const std::vector<std::pair<std::string, std::string>>& settings)
{
    for(const auto& setting : settings) {
        if(setting.first == "http-connection-id") {
            if(setting.second.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Invalid empty value given for key 'http-connection-id'."));
            }
            if(!httpClientConnectionFactoryId.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Multiple defintion of key 'http-connection-id'."));
            }
            httpClientConnectionFactoryId = setting.second;
        }
        else {
            throw Stacktrace::add(
                std::runtime_error(
                    "Unknown key '" + setting.first + "' specified to create class myapp::Main"));
        }
    }

    if(httpClientConnectionFactoryId.empty()) {
        throw Stacktrace::add(
            std::runtime_error(
                "Defintion of key 'http-connection-id' is missing."));
    }
}

std::unique_ptr<Main> Main::create(
    const std::vector<std::pair<std::string, std::string>>& settings) {
    return std::unique_ptr<Main>(new Main(settings));
}

void Main::initializeContext(esl::object::Context& context) {
    httpClientConnectionFactory =
      &context.getObject<esl::com::http::client::ConnectionFactory>(httpClientConnectionFactoryId);
    getHTTPClientConnectionFactory();
}

void Main::procedureRun(esl::object::Context& context) {
    run();
}

void Main::run() {
    logger.info << "Hello world.\n";

    int date;
    float openingPrice;
    std::tie(date, openingPrice) = fetchOpeningPriceToday();
    if(openingPrice < 0) {
        logger.warn << "No service\n";
        return;
    }

    logger.debug << "opening price at " << date << ": " << openingPrice << std::endl;
}

std::tuple<int, float> Main::fetchOpeningPriceToday()
{
    auto connection = getHTTPClientConnectionFactory().createConnection();
    if(!connection) {
        throw Stacktrace::add(std::runtime_error("Could not create HTTP client connection"));
    }

    esl::io::input::String strInput;
    Request request("api/real-time/AAPL.US?api_token=demo&fmt=json", esl::utility::HttpMethodType::httpGet, esl::utility::MIME());
    Response response = connection->send(request, esl::io::Output(), strInput);
    if(response.getStatusCode() < 200 || response.getStatusCode() >= 300) {
        throw Stacktrace::add(std::runtime_error("Request failed with status code " + std::to_string(response.getStatusCode())));
    }

    logger.debug << "JSON result: " << strInput.getString() << "\n";

    auto js = nlohmann::json::parse(strInput.getString());

    float openingPrice = 0;
    js.at("open").get_to(openingPrice);
    logger.trace << "open price is " << openingPrice << std::endl;

    std::tm tm;
    std::time_t timestamp;
    js.at("timestamp").get_to(timestamp);
#ifdef _WIN32
    localtime_s(&tm, &timestamp);
#else
    localtime_r(&timestamp, &tm);
#endif
    int date = ((tm.tm_year + 1900) * 10000) + (tm.tm_mon * 100) + tm.tm_mday;
    logger.trace << "date is " << date << std::endl;

    logger.trace << "opening price at " << tm.tm_year + 1900 << "/" << tm.tm_mon << "/" << tm.tm_mday << ": " << openingPrice << std::endl;

    return std::make_tuple(date, openingPrice);
}

esl::com::http::client::ConnectionFactory& Main::getHTTPClientConnectionFactory() {
    if(!httpClientConnectionFactory) {
        throw Stacktrace::add(
            std::runtime_error(
                "HTTP client connection-factory is not initialized"));
    }
    return *httpClientConnectionFactory;
}

} /* namespace myapp */
```

```cpp
// main.cpp
#include <myapp/Main.hpp>

#include <openesl/Plugin.h>

#include <esl/com/http/client/CURLConnectionFactory.h>
#include <esl/com/http/client/exception/NetworkError.h>
#include <esl/database/exception/SqlError.h>
#include <esl/monitoring/LogbookLogging.h>
#include <esl/object/SimpleContext.h>
#include <esl/plugin/exception/PluginNotFound.h>
#include <esl/plugin/Registry.h>
#include <esl/system/Stacktrace.h>

#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc, const char* argv[]) {
    struct RegistryGuard {
        ~RegistryGuard() {
            esl::plugin::Registry::cleanup();
        }
    } registryGuard;

    int rc = -1;

    try {
		esl::plugin::Registry& registry(esl::plugin::Registry::get());
        openesl::Plugin::install(registry, nullptr);

		auto logging = esl::monitoring::LogbookLogging::createNative();
        std::ifstream f("logger.xml");
        if(f.good())
        {
            logging->addFile("logger.xml");
        }
        f.close();
		registry.setObject(std::move(logging));

        esl::object::SimpleContext context;

        std::unique_ptr<esl::com::http::client::ConnectionFactory> httpClientConnectionFactory =
            esl::com::http::client::CURLConnectionFactory::create({
                {"url", "https://eodhd.com"}
            });
        context.addObject("stock-api-host", std::move(httpClientConnectionFactory));

        auto myMain = myapp::Main::create(
            std::vector<std::pair<std::string, std::string>>({
                {"http-connection-id", "stock-api-host"}
            }));
        
        myMain->initializeContext(context);
        myMain->run();
        rc = 0;
    }
    catch(const esl::com::http::client::exception::NetworkError& e) {
        std::cerr << "HTTP client exception occurred: " << e.what() << "\n";
        std::cerr << "- Error code: " << e.getErrorCode() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const esl::database::exception::SqlError& e) {
        std::cerr << "SQL error exception occurred: " << e.what() << "\n";
        std::cerr << "- SQL return code: " << e.getSqlReturnCode() << "\n";
        e.getDiagnostics().dump(std::cerr);
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const esl::plugin::exception::PluginNotFound& e) {
        std::cerr << "Plugin not found exception occurred: " << e.what() << "\n";
        const esl::plugin::Registry::BasePlugins& basePlugins = esl::plugin::Registry::get().getPlugins(e.getTypeIndex());
        if(basePlugins.empty()) {
            std::cerr << "No implementations available.\n";
        }
        else {
            std::cerr << "Implementations available:\n";
            for(const auto& basePlugin : basePlugins) {
                std::cerr << "- " << basePlugin.first << "\n";
            }
        }
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Exception of type 'std::runtime_error' occurred: " << e.what() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Exception of type 'std::exception' occurred: " << e.what() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(...) {
        std::cerr << "Unknown exception occurred.\n";
    }

    return rc;
}
```

If you compile and run this code, it will do exaclty the same as before. Now the whole initialization is done in `main.cpp`, even creating the instance of our `esl::com::http::client::ConnectionFactory`. After that, we create the instance of `myapp::Main`. It doesn't matter in which order we create these two classes. `myapp::Main` does not need the other connection factory at this stage. We just tell our object the key string to find the connection in a context. Later, we call the method `initializeContext`. At this point, the instance of the connection factory must exist and it must be registered in the context with the same identifier that we told our other class to find this object.  

### Registry & Dependency Injection

This programming technique is called **dependency injection (DI)**. The main idea of dependency injections is to seperate the creation of objects and using them. You don't need to use the the interface `esl::object::Context` to realize dependency injection. Our first idea was to call the constructor of `myapp::Main` with a reference to the connection-object as an additional argument. As soon as there are a large number of references to be passed, you could also bundle them into a `struct` instead of passing them as individual parameters. If you do it this way, it is called **compile-time dependency injection**. This kind of dependency injection has the big advantage to see some kind of error's already at compile-time. The disadvantage is that you must recompile your code, if you want to create another object or if you want to change the parameters to create the object.

The oppsit of *compile-time dependency injection* is **runtime dependency injection**. Then you don't need to recompile your code if you want to change the created objects or to change the parameters to create the objects. This could be realized by reading this information at runtime form the arguments, environment variables, files, etc. Of course, if you have the possibility to change the behavor at runtime, then you will get an error at runtime, if the parameters are invalid.

In the previous chapter, we talked already about the concept to create instances by using a general interface, realized my a static method called `create`. In total we used already three funktions like this:

> Example 1:
> ```cpp
> auto layout = esl::monitoring::SimpleLayout::create({
>     });
> ```
> 
> Example 2:
> ```cpp
> auto appender = esl::monitoring::OStreamAppender::create({
>         {"trace", "out"},
>         {"debug", "out"},
>         {"info", "out"},
>         {"warn", "out"},
>         {"error", "err"}
>     });
> ```
> 
> Example 3:
> ```cpp
> auto httpClientConnectionFactory = CURLConnectionFactory::create({
>         {"url", "https://eodhd.com"}
>     });
> ```

Then we simplified the initialization of the logger by loading an external configuration file. This file still defined to create a layout of type `esl::monitoring::SimpleLayout` and an appender of type `esl::monitoring::OStreamAppender`. This is possible, because the create-functions have been registered in the **ESL registry** and they can be found with an string identifier. This is how you get the ESL registry:

```cpp
esl::plugin::Registry& registry = esl::plugin::Registry::get();
```

YOu can create the instances via the registry as well:

> Example 1:
> ```cpp
> auto layout = registry.create<esl::monitoring::Layout>(
>     "esl::monitoring::SimpleLayout",
>     { });
> ```
> 
> Example 2:
> ```cpp
> auto appender = registry.create<esl::monitoring::Appender>(
>     "esl::monitoring::OStreamAppender",
>     {
>         {"trace", "out"},
>         {"debug", "out"},
>         {"info", "out"},
>         {"warn", "out"},
>         {"error", "err"}
>     });
> ```
> 
> Example 3:
> ```cpp
> auto httpClientConnectionFactory = registry.create<esl::com::http::client::ConnectionFactory>(
>     "esl::com::http::client::CURLConnectionFactory",
>     {
>         {"url", "https://eodhd.com"}
>     });
> ```

This mechanism allows us to set the implementation to be used, including the parameters for creating the object, to our program from the outside. We used this mechanism already for initializion of our logger. Now let's do it for the other objects we need in our code. Right now this is the HTTP client connection factory only, but later it will be easy to add more objects.

We change our code in `src/main.cpp` from this:

```cpp
esl::object::SimpleContext context;
std::unique_ptr<esl::com::http::client::ConnectionFactory> httpClientConnectionFactory =
    esl::com::http::client::CURLConnectionFactory::create({
        {"url", "https://eodhd.com"}
    });
context.addObject("stock-api-host", std::move(httpClientConnectionFactory));
```

to this:

```cpp
esl::object::SimpleProcessingContext context({});
esl::object::Value<std::string> filename("context.xml");
context.runCommand("load-xml-file", &filename);
```

Instead of including the header file `esl/object/SimpleContext.h`, we need some other includes:

```cpp
#include <esl/object/SimpleProcessingContext.h>
#include <esl/object/Procedure.h>
#include <esl/object/Value.h>
#include <string>
```

Now the initialization is outside of our C++ code. The initialization is done by loading the file `context.xml`:

```xml
<!-- context.xml -->
<esl-context>
    <http-client id="stock-api-host" implementation="esl/com/http/client/CURLConnectionFactory">
        <parameter key="url" value="https://eodhd.com"/>
    </http-client>
</esl-context>
```

Some final word's to the registry: We have seen how to create an instance of an object by calling the method `create<T>(...)` from the registry. The method expects the name of the implementation and the parameters. These to arguments can be set dynamically. But there is a Type `<T>` that you have to specify at compile type. If you want to create an instance of type `T`, then the registered create-function must return `std::unique_ptr<T>`. If the registered create-function returns another type, e.g. `std::unique_ptr<U>`, then `esl::plugin::Register::create<T>(...)` will fail.  
There are 2 methods to register your own creat-function. Let's register the function `myapp::Main::create` with name `"myapp/Main"` in our registry:

```cpp
registry.addPlugin("myapp/Main", myapp::Main::create);
```

Now you are able to create an instance of this class by calling this:

```cpp
auto myMain = registry.create<myapp::Main>(
    "myapp/Main",
    {
        {"http-connection-id", "stock-api-host"}
    });
```

In general you shouldn't register your class for the type that is your class. If you are using **dependency injection**, then you want to work with interfaces (or *base classes*) and you have many imlementaions for this base class that you want to make exchangeable. So, in real live you want to register your create-function for at least one of your base classes, but not for the type `myapp::Main`. Our class implements two interfaces / base-classes:

1. `esl::object::InitializeContext`
2. `esl::object::Procedure`

You have to implement a seperate create-function for each type you want to register. It is senseless to register your class for the first interface. Thus, you only need one create-function with return-type `std::unique_ptr<esl::object::Procedure>`. You could change your existing create-function to this type and then you are done. If you have more types you want to register, then it makes sense to keep the create-function as it is and you can register the create-function for another base-type like this:

```cpp
registry.addPlugin<esl::object::Procedure, myapp::Main, myapp::Main::create>("myapp/Main");
```

This template function creates another function for you that calls `myapp::Main::create` and converts the return type from `std::unique_ptr<myapp::Main>` to `std::unique_ptr<esl::object::Procedure>`.

With this final words to the registry, let's register our main function as well. We use the second pattern, so we don't need to touch the files `src/myapp/Main.cpp` and `src/myapp/Main.hpp`. We switched already from `esl::object::Context` to `esl::object::ProcessingContext`. This allows us to call `void esl::object::ProcessingContext::run(int argc, const char* argv[])`, This will execute an anonymous *(=empty 'id')* instance of `esl::object::Procedure`. And there is a create-function with name `myapp/Main` registered that returns an object of type `esl::object::Procedure`. So, let's put this in our configuration as well.

This is the new configuration file `context.xml`:

```xml
<!-- context.xml -->
<esl-context>
    <http-client id="stock-api-host" implementation="esl/com/http/client/CURLConnectionFactory">
        <parameter key="url" value="https://eodhd.com"/>
    </http-client>

    <procedure implementation="myapp/Main">
        <parameter key="http-connection-id" value="stock-api-host"/>
    </procedure>
</esl-context>
```

This is the full content of `src/main.cpp`:

```cpp
// main.cpp
#include <myapp/Main.hpp>

#include <openesl/Plugin.h>

#include <esl/com/http/client/CURLConnectionFactory.h>
#include <esl/com/http/client/exception/NetworkError.h>
#include <esl/database/exception/SqlError.h>
#include <esl/monitoring/LogbookLogging.h>
#include <esl/object/SimpleProcessingContext.h>
#include <esl/object/Procedure.h>
#include <esl/object/Value.h>
#include <esl/plugin/exception/PluginNotFound.h>
#include <esl/plugin/Registry.h>
#include <esl/system/Stacktrace.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, const char* argv[]) {
    struct RegistryGuard {
        ~RegistryGuard() {
            esl::plugin::Registry::cleanup();
        }
    } registryGuard;

    int rc = -1;

    try {
		esl::plugin::Registry& registry(esl::plugin::Registry::get());
        openesl::Plugin::install(registry, nullptr);
        registry.addPlugin<esl::object::Procedure, myapp::Main, myapp::Main::create>("myapp/Main");

		auto logging = esl::monitoring::LogbookLogging::createNative();
        std::ifstream f("logger.xml");
        if(f.good())
        {
            logging->addFile("logger.xml");
        }
        f.close();
		registry.setObject(std::move(logging));

        esl::object::SimpleProcessingContext context({});
        esl::object::Value<std::string> filename("context.xml");
        context.runCommand("load-xml-file", &filename);
        context.run(argc, argv);

        rc = 0;
    }
    catch(const esl::com::http::client::exception::NetworkError& e) {
        std::cerr << "HTTP client exception occurred: " << e.what() << "\n";
        std::cerr << "- Error code: " << e.getErrorCode() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const esl::database::exception::SqlError& e) {
        std::cerr << "SQL error exception occurred: " << e.what() << "\n";
        std::cerr << "- SQL return code: " << e.getSqlReturnCode() << "\n";
        e.getDiagnostics().dump(std::cerr);
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const esl::plugin::exception::PluginNotFound& e) {
        std::cerr << "Plugin not found exception occurred: " << e.what() << "\n";
        const esl::plugin::Registry::BasePlugins& basePlugins = esl::plugin::Registry::get().getPlugins(e.getTypeIndex());
        if(basePlugins.empty()) {
            std::cerr << "No implementations available.\n";
        }
        else {
            std::cerr << "Implementations available:\n";
            for(const auto& basePlugin : basePlugins) {
                std::cerr << "- " << basePlugin.first << "\n";
            }
        }
    }
    catch(const std::runtime_error& e) {
        std::cerr << "Exception of type 'std::runtime_error' occurred: " << e.what() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Exception of type 'std::exception' occurred: " << e.what() << "\n";
        if(const auto* s = esl::system::Stacktrace::get(e)) {
            s->dump(std::cerr);
        }
    }
    catch(...) {
        std::cerr << "Unknown exception occurred.\n";
    }
    return rc;
}
```

### Database

We reached the point where we can make new connection factories available to our class without touching the file `src/main.cpp` anymore. We could create an instance of a database connection factory for an SQLite database like this:

```cpp
#include <esl/database/ConnectionFactory.h>
#include <esl/database/SQLiteConnectionFactory.h>
...
std::unique_ptr<esl::database::ConnectionFactory> dbConnectionFactory =
    SQLiteConnectionFactory::create({{"URI", "file:prices.db?mode=rwc"}});
```

But now we make the connection factory available in the context with name `db-connection-factory`. So, we update the file `context.xml` to this:

```cpp
<!-- context.xml -->
<esl-context>
    <database id="db-connection-factory" implementation="esl/database/SQLiteConnectionFactory">
        <parameter key="URI" value="file:prices.db?mode=rwc"/>
    </database>

    <http-client id="stock-api-host" implementation="esl/com/http/client/CURLConnectionFactory">
        <parameter key="url" value="https://eodhd.com"/>
    </http-client>

    <procedure implementation="myapp/Main">
        <parameter key="api-token" value="demo"/>
        <parameter key="db-id" value="db-connection-factory"/>
        <parameter key="http-connection-id" value="stock-api-host"/>
    </procedure>
</esl-context>
```

Now let's update our files `src/myapp/Main.hpp` and `src/myapp/Main.cpp`:

```cpp
// myapp/Main.hpp
#include <esl/com/http/client/ConnectionFactory.h>
#include <esl/database/ConnectionFactory.h>
#include <esl/object/Context.h>
#include <esl/object/InitializeContext.h>
#include <esl/object/Procedure.h>

#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace myapp {

class Main
: public esl::object::InitializeContext,
  public esl::object::Procedure
{
public:
    Main(const std::vector<std::pair<std::string, std::string>>& settings);

    static std::unique_ptr<Main> create(
        const std::vector<std::pair<std::string, std::string>>& settings);
	void initializeContext(esl::object::Context& context) override;
    void procedureRun(esl::object::Context& context) override;
    
private:
    float getOpeningPriceByDate(int date);
    std::tuple<int, float> fetchOpeningPriceToday();
    float loadOpeningPrice(int date);
    void saveOpeningPrice(int date, float price);

    std::string apiToken;

    esl::database::ConnectionFactory& getDBClientConnectionFactory();
    esl::database::ConnectionFactory* dbConnectionFactory = nullptr;
    std::string dbConnectionFactoryId;

    esl::com::http::client::ConnectionFactory& getHTTPClientConnectionFactory();
    esl::com::http::client::ConnectionFactory* httpClientConnectionFactory = nullptr;
    std::string httpClientConnectionFactoryId;
};

} /* namespace myapp */
```

```cpp
// myapp/Main.cpp
#include <myapp/Logger.hpp>
#include <myapp/Main.hpp>

#include <esl/com/http/client/Request.h>
#include <esl/com/http/client/Response.h>
#include <esl/database/Connection.h>
#include <esl/database/PreparedStatement.h>
#include <esl/database/ResultSet.h>
#include <esl/io/input/String.h>
#include <esl/io/Output.h>
#include <esl/system/Stacktrace.h>
#include <esl/utility/HttpMethod.h>
#include <esl/utility/MIME.h>

#include <nlohmann/json.hpp>

#include <ctime>
#include <iostream>
#include <stdexcept>

using namespace esl::com::http::client;
using namespace esl::system;

namespace myapp {

namespace {
Logger logger("myapp::Main");
}

Main::Main(const std::vector<std::pair<std::string, std::string>>& settings)
{
    /*
    std::string apiToken;

    esl::database::ConnectionFactory& getDBClientConnectionFactory();
    esl::database::ConnectionFactory* dbConnectionFactory = nullptr;
    std::string dbConnectionFactoryId;
    */
    for(const auto& setting : settings) {
        if(setting.first == "api-token") {
            if(setting.second.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Invalid empty value given for key 'api-token'."));
            }
            if(!apiToken.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Multiple defintion of key 'api-token'."));
            }
            apiToken = setting.second;
        }
        else if(setting.first == "db-id") {
            if(setting.second.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Invalid empty value given for key 'http-connection-id'."));
            }
            if(!dbConnectionFactoryId.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Multiple defintion of key 'db-id'."));
            }
            dbConnectionFactoryId = setting.second;
        }
        else if(setting.first == "http-connection-id") {
            if(setting.second.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Invalid empty value given for key 'http-connection-id'."));
            }
            if(!httpClientConnectionFactoryId.empty()) {
                throw Stacktrace::add(
                    std::runtime_error(
                        "Multiple defintion of key 'http-connection-id'."));
            }
            httpClientConnectionFactoryId = setting.second;
        }
        else {
            throw Stacktrace::add(
                std::runtime_error(
                    "Unknown key '" + setting.first + "' specified to create class myapp::Main"));
        }
    }

    if(apiToken.empty()) {
        apiToken = "demo";
    }

    if(dbConnectionFactoryId.empty()) {
        throw Stacktrace::add(
            std::runtime_error(
                "Defintion of key 'db-id' is missing."));
    }

    if(httpClientConnectionFactoryId.empty()) {
        throw Stacktrace::add(
            std::runtime_error(
                "Defintion of key 'http-connection-id' is missing."));
    }
}

std::unique_ptr<Main> Main::create(
    const std::vector<std::pair<std::string, std::string>>& settings) {
    return std::unique_ptr<Main>(new Main(settings));
}

void Main::initializeContext(esl::object::Context& context) {
    dbConnectionFactory =
        &context.getObject<esl::database::ConnectionFactory>(dbConnectionFactoryId);
    std::unique_ptr<esl::database::Connection> connection = getDBClientConnectionFactory().createConnection();
    if(!connection) {
        throw Stacktrace::add(std::runtime_error("Could not create database connection"));
    }
    esl::database::PreparedStatement preparedStatement = connection->prepare("CREATE TABLE IF NOT EXISTS prices (date INTEGER PRIMARY KEY, price REAL);");
    preparedStatement.execute();

    httpClientConnectionFactory =
        &context.getObject<esl::com::http::client::ConnectionFactory>(httpClientConnectionFactoryId);
    getHTTPClientConnectionFactory();
}

void Main::procedureRun(esl::object::Context& context) {
    logger.info << "Hello world.\n";

    std::tm tm;
    std::time_t timestamp = std::time(nullptr);
#ifdef _WIN32
    localtime_s(&tm, &timestamp);
#else
    localtime_r(&timestamp, &tm);
#endif
    int currentDate = ((tm.tm_year + 1900) * 10000) + (tm.tm_mon * 100) + tm.tm_mday;

    logger.info << "Load opening price for Apple at date " << currentDate << "...\n";
    float price = getOpeningPriceByDate(currentDate);
    if(price < 0) {
        logger.warn << "Couldn't load price.\n";
    }
    else {
        logger.info << "Price is " << price << " USD.\n";
    }
}

float Main::getOpeningPriceByDate(int date)
{
    float openingPrice = loadOpeningPrice(date);

    if(openingPrice < 0)
    {
        int fetchDate;
        std::tie(fetchDate, openingPrice) = fetchOpeningPriceToday();
        if(openingPrice < 0 ||fetchDate != date) {
            return -1.0f;
        }

        saveOpeningPrice(date, openingPrice);
    }

    return openingPrice;
}

std::tuple<int, float> Main::fetchOpeningPriceToday()
{
    auto connection = getHTTPClientConnectionFactory().createConnection();
    if(!connection) {
        throw Stacktrace::add(std::runtime_error("Could not create HTTP client connection"));
    }

    esl::io::input::String strInput;
    Request request("api/real-time/AAPL.US?api_token=" + apiToken + "&fmt=json", esl::utility::HttpMethodType::httpGet, esl::utility::MIME());
    Response response = connection->send(request, esl::io::Output(), strInput);
    if(response.getStatusCode() < 200 || response.getStatusCode() >= 300) {
        throw Stacktrace::add(std::runtime_error("Request failed with status code " + std::to_string(response.getStatusCode())));
    }

    logger.debug << "JSON result: " << strInput.getString() << "\n";

    auto js = nlohmann::json::parse(strInput.getString());

    float openingPrice = 0;
    js.at("open").get_to(openingPrice);
    logger.trace << "open price is " << openingPrice << std::endl;

    std::tm tm;
    std::time_t timestamp;
    js.at("timestamp").get_to(timestamp);
#ifdef _WIN32
    localtime_s(&tm, &timestamp);
#else
    localtime_r(&timestamp, &tm);
#endif
    int date = ((tm.tm_year + 1900) * 10000) + (tm.tm_mon * 100) + tm.tm_mday;
    logger.trace << "date is " << date << std::endl;

    logger.trace << "opening price at " << tm.tm_year + 1900 << "/" << tm.tm_mon << "/" << tm.tm_mday << ": " << openingPrice << std::endl;

    return std::make_tuple(date, openingPrice);
}

float Main::loadOpeningPrice(int date)
{
    float openingPrice = -1.0f;

    std::unique_ptr<esl::database::Connection> connection = getDBClientConnectionFactory().createConnection();
    if(!connection) {
        throw Stacktrace::add(std::runtime_error("Could not create database connection"));
    }

    esl::database::PreparedStatement preparedStatement = connection->prepare("SELECT price FROM prices WHERE date = ?");
    for(esl::database::ResultSet resultSet = preparedStatement.execute(date); resultSet; resultSet.next())
    {
        const auto& priceField = resultSet["price"];
        if(!priceField.isNull()) {
            openingPrice = static_cast<float>(priceField.asDouble()); // dirty cast !!!
        }
        break;
    }

    return openingPrice;
}

void Main::saveOpeningPrice(int date, float price) {
    std::unique_ptr<esl::database::Connection> connection = getDBClientConnectionFactory().createConnection();
    if(!connection) {
        throw Stacktrace::add(std::runtime_error("Could not create database connection"));
    }

    esl::database::PreparedStatement preparedStatement = connection->prepare("INSERT INTO prices (date,price) VALUES (?,?)");
    preparedStatement.execute(date, price);
}

esl::database::ConnectionFactory& Main::getDBClientConnectionFactory() {
    if(!dbConnectionFactory) {
        throw Stacktrace::add(
            std::runtime_error(
                "Database connection-factory is not initialized"));
    }
    return *dbConnectionFactory;
}

esl::com::http::client::ConnectionFactory& Main::getHTTPClientConnectionFactory() {
    if(!httpClientConnectionFactory) {
        throw Stacktrace::add(
            std::runtime_error(
                "HTTP client connection-factory is not initialized"));
    }
    return *httpClientConnectionFactory;
}

} /* namespace myapp */
```
