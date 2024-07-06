# CMakeList.txt : CMake project for TRAHT-Manual-Controller, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#project ("Air-Hockey-Robot-Manual-Controller")

set(SFML_DIR "libraries/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)


add_subdirectory("libraries/serial")
include_directories("libraries/serial/include")

# Add source to this project's executable.
add_executable (Air-Hockey-Robot-Manual-Controller "Air-Hockey-Robot-Manual-Controller.cpp" "libraries/serial/include/serial/serial.h" "libraries/serial/include/serial/v8stdint.h" "libraries/serial/include/serial/impl/unix.h" "libraries/serial/include/serial/impl/win.h")
target_link_libraries(Air-Hockey-Robot-Manual-Controller sfml-graphics sfml-window sfml-system serial)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Air-Hockey-Robot-Manual-Controller PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
