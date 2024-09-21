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
  # We use this instead of cd as we wount change directory if pushd fails
  pushd build > /dev/null
  cmake -DCMAKE_TOOLCHAIN_FILE=/c/Users/samtn/vcpkg/scripts/buildsystems/vcpkg.cmake ..
  if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    popd > /dev/null
    exit 1
  fi
  cmake --build .
  if [ $? -ne 0 ]; then
    echo "Build failed."
    popd > /dev/null
    exit 1
  fi
  popd > /dev/null
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
  echo "Running executable with arguments: $*"
  # Execute the program
  "$exe_path" "$@"
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
actions=()
app_args=()
parsing_actions=true

# Parse arguments
for arg in "$@"; do
  if $parsing_actions; then
    case $arg in
      clean|build|run|test)
        actions+=("$arg")
        ;;
      *)
        parsing_actions=false
        app_args+=("$arg")
        ;;
    esac
  else
    app_args+=("$arg")
  fi
done

# If no actions specified, default to "run"
if [ ${#actions[@]} -eq 0 ]; then
  actions+=("run")
fi

# Execute actions
for action in "${actions[@]}"; do
  case $action in
    clean)
      clean
      ;;
    build)
      build
      ;;
    run)
      run "${app_args[@]}"
      ;;
    test)
      test
      ;;
  esac
done
