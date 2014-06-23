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
file(GLOB project_HDRS *.h)
file(GLOB project_SRCS *.cpp)

# ### QT4 ###
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
qt4_wrap_ui(project_UIS_H)
qt4_wrap_cpp(project_MOC_SRCS container.h)

# ### TARGET ###
add_executable(nodebus_container ${project_SRCS} ${project_MOC_SRCS} ${project_HDRS})
target_link_libraries(nodebus_container ${QT_LIBRARIES} nodebus nodebusosgi)
