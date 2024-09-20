cmake_minimum_required(VERSION 3.12)

# Get the project directory name
get_filename_component(PROJECT_NAME ${CMAKE_SOURCE_DIR} NAME)

# Set project name
project(${PROJECT_NAME})

# Defining project name as preprocessor macro
add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

# Silence the C++23 denorm depreciation warning 
# - deprecation of some floating point related features
add_definitions(-D_SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)

# Enable generation of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Print project name
message("Project Name: ${PROJECT_NAME}")

# Set the C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Print compiler version
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

# Find Eigen3
find_package(Eigen3 CONFIG REQUIRED)
message(STATUS "Eigen3 version: ${EIGEN3_VERSION_STRING}")

# Find Boost
find_package(Boost REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")

# Add include directories
include_directories(include ${EIGEN3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Remove main.cpp from SOURCES
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Add main.cpp when generating executable for the main application
add_executable(${PROJECT_NAME} "src/main.cpp" ${SOURCES})

# Set include directories for the main target
target_include_directories(${PROJECT_NAME} PRIVATE include ${EIGEN3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# Link Eigen3 and Boost to the main target
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen ${Boost_LIBRARIES})

# Find Google Test package
find_package(GTest CONFIG REQUIRED)

# Add test source files
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# Generate the test executable
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES} ${SOURCES})

# Set include directories for the test target
target_include_directories(${PROJECT_NAME}_test PRIVATE include ${GTEST_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})

# Link Google Test, Eigen3, and Boost to the test executable
target_link_libraries(${PROJECT_NAME}_test PRIVATE GTest::gtest GTest::gtest_main Eigen3::Eigen ${Boost_LIBRARIES})

# Custom target to run the executable after building
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
