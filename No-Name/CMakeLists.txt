# CMakeList.txt : CMake project for No-Name, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (No-Name "No-Name.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET No-Name PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
