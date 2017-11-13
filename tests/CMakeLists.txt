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
# set target
set (TARGET main)
#

#
# set target include directories
set (INCLUDE_DIRECTORIES ${libpipe_SOURCE_DIR}/include)
#

#
# create executable for target
add_executable (${TARGET} ${TARGET}.cc)
#

#
# specify target include directories
target_include_directories (${TARGET} PUBLIC ${INCLUDE_DIRECTORIES})
#

#
# add test to CTest
add_test (NAME ${TARGET} COMMAND ${TARGET})
#

