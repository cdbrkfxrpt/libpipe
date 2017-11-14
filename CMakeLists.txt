#
# Copyright 2017- Florian Eich <florian.eich@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# require CMake version >= 3.8 to make sure it knows C++17
cmake_minimum_required (VERSION 3.8)
#

#
# set project name and version
project (libpipe CXX)
#
set (libpipe_VERSION_MAJOR 1)
set (libpipe_VERSION_MINOR 0)
set (libpipe_VERSION_PATCH 0)
#

#
# die if on Windows
if (NOT UNIX)
  message (FATAL_ERROR "I don't build on non-UNIX because I'm untested there.")
endif ()
#

#
# detect compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.2.0)
    message (FATAL_ERROR "g++ 7.2.0 or later required.")
  endif ()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0.0)
    message (FATAL_ERROR "clang 5.0.0 or later required.")
  endif()
  add_compile_options (-stdlib=libc++ -fuse-ld=lld -flto=thin)
else ()
  message (WARNING "Compiler and standard library must implement C++17 for this to compile.")
endif ()
#

#
# set C++ standard and global compiler options
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)
#
add_compile_options (-Wall -Wextra -Wpedantic -Werror)
#

#
# compile commands for youcompleteme (vim plugin)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
#

#
# set output directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#

#
# enable testing and add unit tests
enable_testing ()
add_subdirectory (${libpipe_SOURCE_DIR}/tests)
#

