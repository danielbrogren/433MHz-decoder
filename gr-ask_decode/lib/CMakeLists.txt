# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND ask_decode_sources
    clock_detector_impl.cc
)

set(ask_decode_sources "${ask_decode_sources}" PARENT_SCOPE)
if(NOT ask_decode_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT ask_decode_sources)

add_library(gnuradio-ask_decode SHARED ${ask_decode_sources})
target_link_libraries(gnuradio-ask_decode ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-ask_decode PROPERTIES DEFINE_SYMBOL "gnuradio_ask_decode_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-ask_decode PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-ask_decode
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_ask_decode_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_ask_decode.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_ask_decode.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_clock_detector.cc
)

add_executable(test-ask_decode ${test_ask_decode_sources})

target_link_libraries(
  test-ask_decode
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-ask_decode
)

GR_ADD_TEST(test_ask_decode test-ask_decode)
