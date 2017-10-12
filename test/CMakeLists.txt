cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(sqlite_test LANGUAGES C CXX)
include(CTest)

get_filename_component(TOP_DIR "${CMAKE_CURRENT_SOURCE_DIR}" PATH)

enable_testing()

if (BUILD_TESTING)
  find_package(Threads REQUIRED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Wextra -Werror -Wunused")
  set(INCLUDE_DIRS
    "${TOP_DIR}/include"
    "${TOP_DIR}/test/contrib/catch/include"
    )
    
  set(SQLITE_TEST_SOURCES
    "src/sqlite_test.cpp"
    )
  add_executable(sqlite_test ${SQLITE_TEST_SOURCES})
  target_compile_definitions(sqlite_test PRIVATE "CATCH_CONFIG_MAIN" "CATCH_CONFIG_COLOUR_NONE")
  target_include_directories(sqlite_test PRIVATE ${INCLUDE_DIRS})
  target_link_libraries(sqlite_test sqlite3 ${CMAKE_THREAD_LIBS_INIT})
  add_test(NAME RunSqliteTests WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" COMMAND sqlite_test)

  set(TP_NATIVE_TEST_SOURCES
    "src/tp_native_test.cpp"
    )

endif()
