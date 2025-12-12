# CMakeList.txt : CMake project for Lab2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lab2" LANGUAGES CXX)

add_library(Lab2Lib INTERFACE)

target_include_directories(Lab2Lib
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(Tests tests/test.cpp)

target_link_libraries(Tests
    PRIVATE
        Lab2Lib
        GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(Tests)

add_executable(Main src/main.cpp)

target_link_libraries(Main
    PRIVATE
        Lab2Lib
)
