INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_ASK_DECODE ask_decode)

FIND_PATH(
    ASK_DECODE_INCLUDE_DIRS
    NAMES ask_decode/api.h
    HINTS $ENV{ASK_DECODE_DIR}/include
        ${PC_ASK_DECODE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    ASK_DECODE_LIBRARIES
    NAMES gnuradio-ask_decode
    HINTS $ENV{ASK_DECODE_DIR}/lib
        ${PC_ASK_DECODE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASK_DECODE DEFAULT_MSG ASK_DECODE_LIBRARIES ASK_DECODE_INCLUDE_DIRS)
MARK_AS_ADVANCED(ASK_DECODE_LIBRARIES ASK_DECODE_INCLUDE_DIRS)

