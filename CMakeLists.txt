cmake_minimum_required(VERSION 3.6.3)
project(kvsdcep C)

include(dcepFilePaths.cmake)

add_library(kvsdcep ${DCEP_SOURCES})

target_include_directories(kvsdcep PUBLIC
                           ${DCEP_INCLUDE_PUBLIC_DIRS})

# install header files
install(
    FILES ${DCEP_INCLUDE_PUBLIC_FILES}
    DESTINATION include/kvssdp)

# install DCEP library
install(
    TARGETS kvsdcep
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
