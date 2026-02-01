project(No-Name)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/SFML-3.0.0+32-Bit/include")

link_directories("C:/SFML-3.0.0+32-Bit/lib")

add_executable (
	No-Name "No-Name.cpp")

target_link_libraries(No-Name sfml-graphics sfml-window sfml-system sfml-audio sfml-network)