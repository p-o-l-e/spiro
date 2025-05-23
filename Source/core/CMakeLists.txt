cmake_minimum_required(VERSION 3.10)
project(SpiroHeadless)

set(CMAKE_CXX_STANDARD 23) 
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG_MODE")

add_compile_options(-g -Wfatal-errors)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)


file(GLOB SOURCES   ${CMAKE_SOURCE_DIR}/*.cpp 
                    ${CMAKE_SOURCE_DIR}/utility/*.cpp 
                    ${CMAKE_SOURCE_DIR}/setup/*.cpp 
                    ${CMAKE_SOURCE_DIR}/graphics/*.cpp 
                    ${CMAKE_SOURCE_DIR}/modules/*.cpp 
                    ${CMAKE_SOURCE_DIR}/modules/interface/*.cpp ) 

add_library(spiro STATIC ${SOURCES})


#add_executable(core_test ${CMAKE_SOURCE_DIR}/core-test/core_test.cpp)
add_executable(ui_test   ${CMAKE_SOURCE_DIR}/ui-test/ui_test.cpp)


include_directories(${CMAKE_SOURCE_DIR}/ 
                    ${CMAKE_SOURCE_DIR}/utility/ 
                    ${CMAKE_SOURCE_DIR}/setup/ 
                    ${CMAKE_SOURCE_DIR}/graphics/ 
                    ${CMAKE_SOURCE_DIR}/modules/ 
                    ${CMAKE_SOURCE_DIR}/modules/interface/ )

                #target_link_libraries(core_test PRIVATE spiro)
target_link_libraries(ui_test   PRIVATE raylib spiro)


#set_target_properties(core_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set_target_properties(ui_test   PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

#add_custom_command(TARGET core_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/core_test ${CMAKE_SOURCE_DIR}/bin )
add_custom_command(TARGET   ui_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/ui_test   ${CMAKE_SOURCE_DIR}/bin )


