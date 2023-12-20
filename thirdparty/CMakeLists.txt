cmake_policy(SET CMP0077 NEW)

set(THIRD_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

set(EXT_INSTALL_DIR ${CMAKE_BINARY_DIR}/ext-install)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/ext-src")

include(GNUInstallDirs)

if (WITH_TESTS)
  add_subdirectory(gtest)
endif()