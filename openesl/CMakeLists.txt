include(Packages)

add_subdirectory(src/main)

option(COMPILE_UNITTESTS "Weather to compile unittests" ON)
if(COMPILE_UNITTESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/test/main.cpp")
    add_subdirectory(src/test)
endif()

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME})

configure_file(${PROJECT_NAME}Config.in.cmake ${CMAKE_BINARY_DIR}/configured_files/${PROJECT_NAME}Config.cmake ESCAPE_QUOTES @ONLY)
install(FILES "${CMAKE_BINARY_DIR}/configured_files/${PROJECT_NAME}Config.cmake" DESTINATION lib/cmake/${PROJECT_NAME})
#install(FILES "${PROJECT_NAME}Config.cmake" DESTINATION lib/cmake/${PROJECT_NAME})
#install(FILES "${PROJECT_NAME}Config.cmake" "${PROJECT_NAME}ConfigVersion.cmake" DESTINATION lib/cmake/${PROJECT_NAME})
