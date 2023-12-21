
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
# add_library(${DBGLIB_LIB} ${DBGLIB_SOURCES})
add_executable(qf-tests ${TEST_SOURCES})

target_link_libraries(qf-tests PUBLIC gtest pthread dbglib)

if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0.0)
  target_link_libraries(qf-tests PUBLIC stdc++fs)
endif()

add_test(
  NAME datastruct:qf
  COMMAND qf-tests
)