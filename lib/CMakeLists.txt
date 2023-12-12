configure_file("${CMAKE_SOURCE_DIR}/lib/config.hpp.in" "${CMAKE_BINARY_DIR}/include/dbglib/config.hpp")

set(DBGLIB_LIB "dbglib")
# if (NOT APPLE)
#   set(CMAKE_INSTALL_RPATH $ORIGIN)
# endif()


file(GLOB_RECURSE DBGLIB_SOURCES "src/*.cpp")
add_library(${DBGLIB_LIB} ${DBGLIB_SOURCES})

set_target_properties(${DBGLIB_LIB} PROPERTIES OUTPUT_NAME "dbg")
target_compile_definitions(${DBGLIB_LIB} PUBLIC DBGLIB_PUBLIC)

add_library(${DBGLIB_LIB}::${DBGLIB_LIB} ALIAS ${DBGLIB_LIB})

target_compile_features(${DBGLIB_LIB} PUBLIC cxx_std_20)

target_link_libraries(${DBGLIB_LIB} PUBLIC
  $<BUILD_INTERFACE:build_type_flags>
  $<BUILD_INTERFACE:warning_flags>
)

# add_dependencies(${KMINDEX_LIB} deps)

target_include_directories(${DBGLIB_LIB} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/include>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(${DBGLIB_LIB} PROPERTIES
  SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
  VERSION ${CMAKE_PROJECT_VERSION}
)