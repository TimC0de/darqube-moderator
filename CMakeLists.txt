cmake_minimum_required(VERSION 3.9.2)

project(darqube-moderator)

set(PACKAGE_NAME "darqube-moderator" CACHE STRING "The packaging name of this library.")
set(MODERATOR_VERSION "1.0.0" CACHE STRING "The version of the library.")
SET(CMAKE_BUILD_TYPE "DEBUG" CACHE STRING "Cmake build type.")

set(CMAKE_CXX_STANDARD 17)

message("Cmake build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	# Configuring Google Test
	include(FetchContent)
	FetchContent_Declare(
	  googletest
	  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
	)

	FetchContent_MakeAvailable(googletest)
endif()

# Locaing all library's source files
file(GLOB_RECURSE moderator_cpps RELATIVE_PATH "src/**/*.cpp" "src/*.cpp")

# Place header files into 'include' directory
set(header_paths filter json user)
foreach(header_path in ${header_paths})
	file(GLOB moderator_path_hpps RELATIVE_PATH "src/${header_path}/*.h")
	install(FILES ${moderator_path_hpps} DESTINATION include/darqube/moderator/${header_path})
endforeach()

file(GLOB moderator_hpps RELATIVE_PATH "src/*.h")
install(FILES ${moderator_hpps} DESTINATION include/darqube/moderator)

if (WIN32)
    # DLLs in Windows appear to have not been fully thought through
    set(DEFAULT_LIBRARY_TYPE "STATIC")
else()
    set(DEFAULT_LIBRARY_TYPE "SHARED")
endif()

file(GLOB etc_files RELATIVE_PATH "etc/*")
file(COPY ${etc_files} DESTINATION "${CMAKE_BINARY_DIR}/etc")

# Building moderator
add_library(
        moderator
        ${moderator_cpps}
        )

target_include_directories(moderator PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(moderator PUBLIC "${PROJECT_SOURCE_DIR}/src")

find_library(jsoncpp NAMES jsoncpp PATHS lib)
target_link_libraries(moderator ${jsoncpp})

install(TARGETS moderator
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
)

# Enabling CTest
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	enable_testing()

	set(DARQUBE_ROOT ${PROJECT_SOURCE_DIR})
	set(DARQUBE_BUILD_DIR ${CMAKE_BINARY_DIR})
	add_subdirectory(tests)
endif()