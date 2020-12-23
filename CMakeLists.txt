cmake_minimum_required(VERSION 3.9)

##############################
###        googletest      ###
##############################
SET(BUILD_GMOCK ON CACHE INTERNAL "")
SET(BUILD_GTEST ON CACHE INTERNAL "")
SET(INSTALL_GMOCK OFF CACHE INTERNAL "")
SET(INSTALL_GTEST OFF CACHE INTERNAL "")
SET(gmock_build_tests OFF CACHE INTERNAL "")
SET(gtest_build_samples OFF CACHE INTERNAL "")
SET(gtest_build_tests OFF CACHE INTERNAL "")
SET(gtest_disable_pthreads OFF CACHE INTERNAL "")
SET(gtest_hide_internal_symbols OFF CACHE INTERNAL "")
SET(gtest_force_shared_crt OFF CACHE INTERNAL "")

add_subdirectory(googletest)

##############################
###        Project         ###
##############################

project(AbomimableIntelligence VERSION 1.0 LANGUAGES CXX)
enable_testing()
include("CPack")
include(CTest)
set(CMAKE_DEBUG_POSTFIX "-d")

##############################
###        Globals         ###
##############################
set_tests_properties(${noArgsTests}   PROPERTIES TIMEOUT 10)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

find_package(Torch REQUIRED)

##############################
###       Subfolders       ###
##############################
include(AIMacros.cmake)
include(extra.cmake)
add_subdirectory(tool)
add_subdirectory(lib)


