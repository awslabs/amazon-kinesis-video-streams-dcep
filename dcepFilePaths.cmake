# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# DCEP library source files.
set( DCEP_SOURCES
     "${CMAKE_CURRENT_LIST_DIR}/source/dcep_api.c"
     "${CMAKE_CURRENT_LIST_DIR}/source/dcep_endianness.c" )

# DCEP library public include directories.
set( DCEP_INCLUDE_PUBLIC_DIRS
     "${CMAKE_CURRENT_LIST_DIR}/source/include" )

# DCEP library public include header files.
set( DCEP_INCLUDE_PUBLIC_FILES
     "${CMAKE_CURRENT_LIST_DIR}/source/include/dcep_api.h" )
