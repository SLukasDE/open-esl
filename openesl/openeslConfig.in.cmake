include(CMakeFindDependencyMacro)

find_dependency(esa)
find_dependency(esl)
@FIND_DEPENDENCY_COMMON4ESL@
@FIND_DEPENDENCY_CURL4ESL@
@FIND_DEPENDENCY_LOGBOOK4ESL@
@FIND_DEPENDENCY_MHD4ESL@
@FIND_DEPENDENCY_SQLITE4ESL@
@FIND_DEPENDENCY_ODBC4ESL@
@FIND_DEPENDENCY_ZSYSTEM4ESL@

include("${CMAKE_CURRENT_LIST_DIR}/openeslTargets.cmake")
#get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
#include(${SELF_DIR}/${CMAKE_BUILD_TYPE}/my_library.cmake)