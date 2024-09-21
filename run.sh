#!/bin/bash

# Get the directory name
# $(pwd) gets the current working directory,
# dirname extracts the path
# basename extracts the final componet
dir_name=$(basename "$(pwd)")

# Define paths for executables
# ${var} is a variable expansion, similar to %var% in batch
exe_path="build/Debug/${dir_name}.exe"
test_exe_path="build/Debug/${dir_name}_test.exe"

# Function to clean the build directory
clean(){
  echo "Cleaning build directory..."
  # Checking if build directory exists
  if [-d "build" ]; then
    echo "Removing existing build directory..."
    # -rf: recursive and force options for rm
    rm -rf build
    # $? is a special variable that holds the exit status of the last command
    if [ $? -ne 0 ]; then
      echo "Failed to remove build directory."
      exit 1
    fi
  fi
  echo "Creating new build directory..."
  mkdir build
  if [ $? -ne 0 ]; then
    echo "Failed to create build directory."
    exit 1
  fi
  echo "Clean completed successfully."
}

# Function to build the project
build(){
  # -p action for mkdir creates parent directories if they don't exist
  mkdir -p build
  echo "Building the project..."
  cd build
  cmake -DCMAKE_TOOLCHAIN_FILE=/c/Users/samtn/vcpkg/scripts/buildsystems/vcpkg.cmake ..
  if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
  fi
  cmake --build .
  if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
  fi
  cd ..
  echo "Build completed successfully."
}


# Function to run the executable
run() {
  # -f test checks if a file exists and is a regular file
  if [ ! -f "$exe_path" ]; then
    echo "Executable not found. Building the project..."
    build
    if [ $? -ne 0 ]; then
      exit 1
    fi
  fi
  echo "Running executable..."
  # Execute the program
  "$exe_path"
}

# Function to run tests in a new window
test() {
  echo "Performing build and running tests..."
  build
  if [ $? -ne 0 ]; then
    exit 1
  fi
  echo "Running tests..."
  "$test_exe_path"
}

# Function to run tests in a new window
run_tests_new_window() {
  # start opens a new window in GitBash
  # The command in quotes will be executed in the new window
  start bash -c "'$0' test; read -p 'Press Enter to continue...'"
}


# Main script logic
# $# gives the number of command-line arguments
if [ $# -eq 0 ]; then
  run
else
  # Loop through all command-line-arguments
  while [ $# -gt 0 ]; do
    # case statement for argument parsing
    case "$1" in
      build)
        build
        ;;
      run)
        run
        ;;
      test)
        test
        ;;
      clean)
        clean
        ;;
      run_tests_new_window)
        run_tests_new_window
        ;;
      *)
        echo "Invalid action: $1"
        echo "Valid actions are: clean, build, run test, run_tests_new_window"
        exit 1
        ;;
    esac # end case
    # shift removes the first argument, moving to the next one
    shift
  done
fi

