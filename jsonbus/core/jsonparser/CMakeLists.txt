#
#    Copyright 2012-2014 Emeric Verschuur <emericv@openihs.org>
# 
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
# 
#        http://www.apache.org/licenses/LICENSE-2.0
# 
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

# ### FILES ###
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DUSE_JSONBUS_EXCEPTION)
file(GLOB project_HDRS *.h)
file(GLOB project_SRCS *.cpp)

# ### FLEX & BISON ###
find_package(BISON)
find_package(FLEX)
bison_target(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
add_flex_bison_dependency(Scanner Parser)

# ### TARGET ###
add_library(jsonparser STATIC ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS} ${project_SRCS} ${project_HDRS})
# target_link_libraries(jsonparser )
