cmake_minimum_required(VERSION 3.9.2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DARQUBE_BUILD_DIR})

# Build tests

add_executable(json_test json_test.cpp)
target_include_directories(json_test PUBLIC "${DARQUBE_ROOT}/src/json")
target_link_libraries(json_test gtest_main moderator)

add_executable(filter_test filter_test.cpp)
target_include_directories(filter_test PUBLIC "${DARQUBE_ROOT}/src/filter")
target_link_libraries(filter_test gtest_main moderator)

add_executable(user_test user_test.cpp)
target_include_directories(user_test PUBLIC "${DARQUBE_ROOT}/src/user")
target_link_libraries(user_test gtest_main moderator)

add_executable(moderator_test moderator_test.cpp)
target_include_directories(moderator_test PUBLIC "${DARQUBE_ROOT}/src")
target_link_libraries(moderator_test gtest_main moderator)

# Discover tests in binaries
include(GoogleTest)
gtest_discover_tests(json_test
	WORKING_DIRECTORY ${DARQUBE_BUILD_DIR})

gtest_discover_tests(filter_test
	WORKING_DIRECTORY ${DARQUBE_BUILD_DIR})

gtest_discover_tests(user_test
	WORKING_DIRECTORY ${DARQUBE_BUILD_DIR})

gtest_discover_tests(moderator_test
	WORKING_DIRECTORY ${DARQUBE_BUILD_DIR})