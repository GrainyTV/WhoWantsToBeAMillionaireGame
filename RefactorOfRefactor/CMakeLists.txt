cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_CXX_COMPILER "zig;c++" CACHE STRING "Select your CXX compiler")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Wpedantic -Werror -Wno-missing-field-initializers -Wno-return-type")
set(CMAKE_SKIP_RPATH True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(whoWantsToBeAMillionaire LANGUAGES CXX)
add_executable(${PROJECT_NAME})
set(artifactLocation "${CMAKE_BINARY_DIR}/artifact")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${artifactLocation})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "a.out")

file(GLOB sources CONFIGURE_DEPENDS "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE hdr)
target_link_directories(${PROJECT_NAME} PRIVATE lib)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3 SDL3_image SDL3_mixer mpg123 SDL3_ttf freetype harfbuzz)

file(COPY assets DESTINATION ${artifactLocation})