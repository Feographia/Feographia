set(USE_BOOST_LIB OFF CACHE BOOL "USE_BOOST_LIB")
set(BUILD_TESTING ON CACHE BOOL "Build the testing tree.")


# Release/Debug is set in QtCreator.
#set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "CMAKE_BUILD_TYPE")

# Shared is for Debug, static is for Release.
set(BUILD_SHARED_LIBS ON CACHE BOOL "BUILD_SHARED_LIBS")

# Use Link Time Optimization (LTO), for Release.
set(
  CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF
  CACHE BOOL "CMAKE_INTERPROCEDURAL_OPTIMIZATION"
)


set(cmr_BUILD_MULTIPROC ON    CACHE BOOL "cmr_BUILD_MULTIPROC")
set(cmr_BUILD_MULTIPROC_CNT 4 CACHE STRING "cmr_BUILD_MULTIPROC_CNT")

# NOTE: Turn OFF ccache if there are problems with linking and LTO.
set(CMAKE_CXX_COMPILER_LAUNCHER "ccache" CACHE STRING "CMAKE_CXX_COMPILER_LAUNCHER")
set(CMAKE_C_COMPILER_LAUNCHER   "ccache" CACHE STRING "CMAKE_C_COMPILER_LAUNCHER")

option(cmr_PRINT_DEBUG "cmr_PRINT_DEBUG" ON)
set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "CMAKE_VERBOSE_MAKEFILE")
#set(CMAKE_COLOR_MAKEFILE ON CACHE BOOL "CMAKE_COLOR_MAKEFILE")

set(
  cmr_ADD_INSTALL_DIR_PLATFORM_SUFFIX ON
  CACHE BOOL "cmr_ADD_INSTALL_DIR_PLATFORM_SUFFIX"
)
