#!/bin/bash

# Get the directory name (project name)
dir_name=$(basename "$(pwd)")

# Set paths
exe_path="build/Debug/${dir_name}.exe"
test_exe_path="build/Debug/${dir_name}_test.exe"

# Function to build the project
build() {
    mkdir -p build
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

# Function to clean the project
clean() {
    if [ -d "build" ]; then
        echo "Removing existing build directory..."
        rm -rf build
    fi
    echo "Creating new build directory..."
    mkdir build
    echo "Clean completed successfully."
}

# Function to run tests
run_tests() {
    clean
    build
    if [ -f "$test_exe_path" ]; then
        echo "Running tests..."
        "$test_exe_path"
    else
        echo "Test executable not found."
        exit 1
    fi
}

# Function to run the application
run_app() {
    if [ ! -f "$exe_path" ]; then
        echo "Executable not found. Building the project..."
        build
    fi
    echo "Running executable with arguments: $*"
    "$exe_path" "$@"
}

# Main logic
actions=()
app_args=()
parsing_actions=true

# Parse arguments
for arg in "$@"; do
    if $parsing_actions; then
        case $arg in
            build|clean|test|run)
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
        build)
            build
            ;;
        clean)
            clean
            ;;
        test)
            run_tests
            ;;
        run)
            run_app "${app_args[@]}"
            ;;
    esac
done
