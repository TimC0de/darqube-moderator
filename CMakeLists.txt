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

# Etc files copying to build directory
file(GLOB etc_files RELATIVE_PATH "etc/*")
file(COPY ${etc_files} DESTINATION "${CMAKE_BINARY_DIR}/etc")

# Building moderator
add_library(moderator SHARED ${moderator_cpps})

target_include_directories(moderator PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(moderator PUBLIC "${PROJECT_SOURCE_DIR}/src")

## Importing jsoncpp library
add_library(jsoncpp SHARED IMPORTED)
set_target_properties(jsoncpp PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib")

target_link_libraries(moderator ${jsoncpp})

install(TARGETS moderator
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib
)

install(DIRECTORY lib DESTINATION .)

# Installing headers
install(DIRECTORY include/json DESTINATION include)

# Enabling CTest
if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
	enable_testing()

	set(DARQUBE_ROOT ${PROJECT_SOURCE_DIR})
	set(DARQUBE_BUILD_DIR ${CMAKE_BINARY_DIR})
	add_subdirectory(tests)
endif()