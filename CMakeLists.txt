cmake_minimum_required(VERSION 3.23)

project(
    openesl
    VERSION 1.6.0
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
include(Packages)
#find_package_RapidJSON()


option(ALL_IN_ONE_ESL              "Weather to compile a single library instead of many small librariers"  ON)
option(BUILD_SHARED_LIBS           "Weather to compile a shared library instead of a static library"       OFF)
option(COMPILE_UNITTESTS           "Weather to compile unittests"                                          ON)
option(ESL_DEPENDENCIES_USE_CONAN  "Weather to use Conan as package manager to fetch dependencies"         OFF)
option(ESL_DEPENDENCIES_USE_VCPKG  "Weather to use Vcpkg as package manager to fetch dependencies"         OFF)

option(OPENESL_USE_COMMON4ESL   "Weather to include common4esl"   OFF) #
option(OPENESL_USE_LOGBOOK4ESL  "Weather to include logbook4esl"  OFF) #
option(OPENESL_USE_ZSYSTEM4ESL  "Weather to include zsystem4esl"  OFF)
option(OPENESL_USE_OPENGTX4ESL  "Weather to include opengtx4esl"  OFF) #
option(OPENESL_USE_CURL4ESL     "Weather to include curl4esl"     OFF) #
option(OPENESL_USE_MHD4ESL      "Weather to include mhd4esl"      OFF) #
option(OPENESL_USE_SQLITE4ESL   "Weather to include sqlite4esl"   OFF) #
option(OPENESL_USE_ODBC4ESL     "Weather to include odbc4esl"     OFF)

####################
# fix dependencies #
####################

# dependencies of logbook4esl:
# - common4esl
if(OPENESL_USE_LOGBOOK4ESL AND NOT OPENESL_USE_COMMON4ESL)
    message(STATUS "common4esl has been included because of its dependency in logbook4esl")
    set(OPENESL_USE_COMMON4ESL ON)
endif()

# dependencies of mhd4esl:
# - common4esl
# - opengtx4esl
if(OPENESL_USE_MHD4ESL AND NOT OPENESL_USE_COMMON4ESL)
    message(STATUS "common4esl has been included because of its dependency in mhd4esl")
    set(OPENESL_USE_COMMON4ESL ON)
endif()
if(OPENESL_USE_MHD4ESL AND NOT OPENESL_USE_OPENGTX4ESL)
    message(STATUS "opengtx4esl has been included because of its dependency in mhd4esl")
    set(OPENESL_USE_OPENGTX4ESL ON)
endif()

function(add_esl_feature feature dependency)
    #add_subdirectory(thirdparty/${feature})

    if(ALL_IN_ONE_ESL)
        get_property(TMP_MAIN_SRC     GLOBAL PROPERTY ${feature}_MAIN_SRC)
        get_property(TMP_MAIN_INC     GLOBAL PROPERTY ${feature}_MAIN_INC)
        get_property(TMP_MAIN_INC_DIR GLOBAL PROPERTY ${feature}_MAIN_INC_DIR)

        list(APPEND ALL_ESL_MAIN_SRC ${TMP_MAIN_SRC})
        list(FILTER ALL_ESL_MAIN_SRC EXCLUDE REGEX "${TMP_MAIN_INC_DIR}/lib.cpp" )
        set(ALL_ESL_MAIN_SRC ${ALL_ESL_MAIN_SRC} PARENT_SCOPE)

        list(APPEND ALL_ESL_MAIN_INC ${TMP_MAIN_INC})
        set(ALL_ESL_MAIN_INC ${ALL_ESL_MAIN_INC} PARENT_SCOPE)

        list(APPEND ALL_ESL_MAIN_INC_DIRS ${TMP_MAIN_INC_DIR})
        set(ALL_ESL_MAIN_INC_DIRS ${ALL_ESL_MAIN_INC_DIRS} PARENT_SCOPE)

        if(NOT dependency STREQUAL "")
            list(APPEND ALL_ESL_MAIN_LIBS "${dependency}")
            set(ALL_ESL_MAIN_LIBS ${ALL_ESL_MAIN_LIBS} PARENT_SCOPE)
        endif()

    else(ALL_IN_ONE_ESL)
        if(feature STREQUAL "esa" OR feature STREQUAL "esl")
        else()
            string(TOUPPER ${feature} featureUpperCase)
            set(FIND_DEPENDENCY_${featureUpperCase} "find_dependency(${feature})" PARENT_SCOPE)
        endif()

        if(dependency STREQUAL "")
            list(APPEND ALL_ESL_MAIN_LIBS "${feature}::${feature}")
        else()
            list(APPEND ALL_ESL_MAIN_LIBS "${dependency}" "${feature}::${feature}")
        endif()
        set(ALL_ESL_MAIN_LIBS ${ALL_ESL_MAIN_LIBS} PARENT_SCOPE)
        
    endif(ALL_IN_ONE_ESL)
endfunction()


find_package_esa()
add_esl_feature(esa "")

find_package_esl()
add_esl_feature(esl "")

if(OPENESL_USE_COMMON4ESL)
    find_package_common4esl()
    add_esl_feature(common4esl tinyxml2::tinyxml2)
    find_package_TinyXML2()
    set(DEFINE_USE_COMMON4ESL "1")
else()
    set(DEFINE_USE_COMMON4ESL "0")
endif()

if(OPENESL_USE_LOGBOOK4ESL)
    find_package_logbook4esl()
    add_esl_feature(logbook4esl "tinyxml2::tinyxml2;logbook::logbook")
    find_package_logbook()
    set(DEFINE_USE_LOGBOOK4ESL "1")
else()
    set(DEFINE_USE_LOGBOOK4ESL "0")
endif()

if(OPENESL_USE_ZSYSTEM4ESL)
    find_package_zsystem4esl()
    add_esl_feature(zsystem4esl zsystem::zsystem)
    find_package_zsystem()
    set(DEFINE_USE_ZSYSTEM4ESL "1")
else()
    set(DEFINE_USE_ZSYSTEM4ESL "0")
endif()

if(OPENESL_USE_OPENGTX4ESL)
    find_package_opengtx4esl()
    add_esl_feature(opengtx4esl GnuTLS::GnuTLS)
    find_package_GnuTLS()
    set(DEFINE_USE_OPENGTX4ESL "1")
else()
    set(DEFINE_USE_OPENGTX4ESL "0")
endif()

if(OPENESL_USE_CURL4ESL)
    find_package_curl4esl()
    add_esl_feature(curl4esl CURL::libcurl)
    find_package_CURL()
    set(DEFINE_USE_CURL4ESL "1")
else()
    set(DEFINE_USE_CURL4ESL "0")
endif()

if(OPENESL_USE_MHD4ESL)
    find_package_mhd4esl()
    add_esl_feature(mhd4esl libmicrohttpd::libmicrohttpd)
    find_package_libmicrohttpd()
    set(DEFINE_USE_MHD4ESL "1")
else()
    set(DEFINE_USE_MHD4ESL "0")
endif()

if(OPENESL_USE_SQLITE4ESL)
    find_package_sqlite4esl()
    add_esl_feature(sqlite4esl SQLite::SQLite3)
    find_package_SQLite3()
    set(DEFINE_USE_SQLITE4ESL "1")
else()
    set(DEFINE_USE_SQLITE4ESL "0")
endif()

if(OPENESL_USE_ODBC4ESL)
    find_package_odbc4esl()
    add_esl_feature(odbc4esl ODBC::ODBC)
    find_package_ODBC()
    set(DEFINE_USE_ODBC4ESL "1")
else()
    set(DEFINE_USE_ODBC4ESL "0")
endif()

add_subdirectory(openesl)
