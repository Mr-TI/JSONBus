#
#  Copyright (c) 2012, Emeric Verschuur <emericv@openihs.org>
#  All rights reserved.
# 
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#      * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#      * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#      * Neither the name of the OpenIHS.org nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.
# 
#  THIS SOFTWARE IS PROVIDED BY Emeric Verschuur <emericv@openihs.org> ''AS IS'' AND ANY
#  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
#  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur <emericv@openihs.org> BE LIABLE FOR ANY
#  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
#  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
#  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
#  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

# ### FILES ###
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
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
