cmake_minimum_required(VERSION 3.23.3)
cmake_policy(SET CMP0135 NEW)

# Configure Complier
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)

project(http_server)

add_executable(${PROJECT_NAME} src/main.cpp src/HTTPHandler.cpp src/HTTPRequest.cpp src/HTTPResponse.cpp)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.81 REQUIRED COMPONENTS program_options)
message(STATUS "Boost version: ${Boost_VERSION}")
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
