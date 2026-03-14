cmake_minimum_required(VERSION 3.24)
project(No-Name)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

add_executable(No-Name "No-Name.cpp")

target_link_libraries(No-Name sfml-graphics sfml-window sfml-system sfml-audio sfml-network)