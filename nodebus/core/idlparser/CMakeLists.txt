#
# Copyright (C) 2012-2014 Emeric Verschuur <emericv@mbedsys.org>
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#

# ### FILES ###
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DUSE_NODEBUS_EXCEPTION)
file(GLOB project_HDRS *.h)
file(GLOB project_SRCS *.cpp)

# ### FLEX & BISON ###
find_package(BISON)
find_package(FLEX)
if ("${BISON_VERSION}" MATCHES "(^2\\..*)")
	add_definitions(-DGNU_BISON_V2)
bison_target(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
else ()
bison_target(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc COMPILE_FLAGS -Wno-deprecated)
endif ()
flex_target(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
add_flex_bison_dependency(Scanner Parser)

# ### TARGET ###
add_library(idlparser STATIC ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS} ${project_SRCS} ${project_HDRS})
