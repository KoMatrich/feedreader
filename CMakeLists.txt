# CMakeList.txt : CMake project for hinfosvc

cmake_minimum_required (VERSION 3.16)

# Set the project name and version
project ("feedreader" VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)

# Locate packages
find_package(OpenSSL REQUIRED)
find_package(LibXml2 REQUIRED)

# Define sources and headers locations
file(GLOB_RECURSE srcs "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE hdrs "${PROJECT_SOURCE_DIR}/include/*.h")

# Include needed libs
include_directories(${PROJECT_SOURCE_DIR} ${LIBXML2_INCLUDE_DIR})

# Add source to this project's executable.
add_executable ("${PROJECT_NAME}" ${srcs} ${hdrs} "src/parsedLink.cpp" "src/pasedResponse.cpp" "include/pasedResponse.h"  "include/util.h" )

# Link libs
target_link_libraries("${PROJECT_NAME}" PRIVATE ${LIBXML2_LIBRARIES})
target_link_libraries("${PROJECT_NAME}" PRIVATE ${OPENSSL_LIBRARIES})
target_link_libraries("${PROJECT_NAME}" PRIVATE ${PCAP_LIBRARY})

# Add the header file directory
target_include_directories("${PROJECT_NAME}" PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories("${PROJECT_NAME}" PUBLIC ${PCAP_ROOT_DIR}/Include)