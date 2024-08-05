cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)
set(CMAKE_CXX_COMPILER "zig;c++" CACHE STRING "Select your CXX compiler")
set(CMAKE_SKIP_RPATH True)
set(artifactLocation "${CMAKE_BINARY_DIR}/artifact")

project(whoWantsToBeAMillionaire LANGUAGES CXX)
add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "a.out")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${artifactLocation})

file(GLOB sources CONFIGURE_DEPENDS "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE hdr)
target_link_directories(${PROJECT_NAME} PRIVATE lib)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3 SDL3_image SDL3_mixer mpg123 SDL3_ttf freetype harfbuzz)

file(COPY assets DESTINATION ${artifactLocation})