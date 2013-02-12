#
#    Copyright 2012-2013 Emeric Verschuur <emericv@openihs.org>
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

# ### PROJECT ###
project(jsonbus)
cmake_minimum_required(VERSION 2.8.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if(NOT WIN32)
set(CMAKE_CXX_FLAGS "-Wall -std=gnu++0x -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG  "-g3")
endif(NOT WIN32)

# ### SUBDIRECTORIES ###
add_subdirectory(jsonbus)
