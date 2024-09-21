cmake_minimum_required(VERSION 3.21)

# Gets project directory name
get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)

# Sets project name
project(${PROJECT_NAME})

# Defining project name as preprocesssor macro
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

# Enables generation of compile_commands.json 
# Works fine on linux. Needs ninja on windows.
# This project uses compile_flags.txt though
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prints project name
message("Project Name: ${PROJECT_NAME}")


# Sets the C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prints the compiler version
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")


# Finds Eigen3
find_package(Eigen3 CONFIG REQUIRED)
message(STATUS "Eigen3 version: ${EIGEN3_VERSION_STRING}")

# Finds Boost
find_package(Boost REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")

# Adds dependency include directories
include_directories(include ${EIGEN3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})


# Adds source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Removes main.cpp from SOURCES
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Adds back main.cpp when generating executable for main app
add_executable(${PROJECT_NAME} "src/main.cpp" ${SOURCES})

# Sets include directories for the main target
target_include_directories(${PROJECT_NAME} PRIVATE include ${EIGEN3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# Link any necessary libraries here, if needed
# Linking Eigen3 and Boost to main target
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen ${Boost_LIBRARIES})


# Finds Google Test package
find_package(GTest CONFIG REQUIRED)

# Adds test source files
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Generates the test executable
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES} ${SOURCES})

# Sets include directory for the test target
target_include_directories(${PROJECT_NAME}_test PRIVATE include ${GTEST_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# Link Google Test, Eigen3 and Boost to the test executable
target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest GTest::gtest_main Eigen3::Eigen ${Boost_LIBRARIES})


# Custom target to run the executable after building
add_custom_target( run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
