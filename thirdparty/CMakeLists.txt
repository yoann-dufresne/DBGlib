cmake_policy(SET CMP0077 NEW)

set(THIRD_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

set(EXT_INSTALL_DIR ${CMAKE_BINARY_DIR}/ext-install)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/ext-src")

include(GNUInstallDirs)




find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/thirdparty/kseqpp/CMakeLists.txt")
    message(FATAL_ERROR "Kseqpp submodule not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()




if (WITH_TESTS)
  add_subdirectory(gtest)
endif()

add_subdirectory(zlib)

add_library(kseqpp INTERFACE)
include(ExternalProject)

ExternalProject_Add(
  KSEQPP
  PREFIX KSEQPP
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/kseqpp
  INSTALL_COMMAND ""
  LOG_CONFIGURE ON
  LOG_BUILD ON
)

target_include_directories(kseqpp INTERFACE ${THIRD_DIR}/kseqpp/include ${THIRD_DIR}/kseqpp/include/kseq++)
