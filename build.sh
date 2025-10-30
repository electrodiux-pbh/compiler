#!/bin/bash

# Build script for the compiler project

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if clang is installed
if ! command -v clang &> /dev/null; then
    print_error "Clang compiler not found. Please install clang and clang++."
    exit 1
fi

if ! command -v clang++ &> /dev/null; then
    print_error "Clang++ compiler not found. Please install clang++."
    exit 1
fi

# Create build directory
BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ]; then
    print_status "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Navigate to build directory
cd "$BUILD_DIR"

# Configure with CMake
print_status "Configuring project with CMake..."
cmake -DCMAKE_BUILD_TYPE=Debug ..

if [ $? -ne 0 ]; then
    print_error "CMake configuration failed!"
    exit 1
fi

# Build the project
print_status "Building project..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    print_status "Build successful!"
    if [ -f "compiler" ]; then
        print_status "Executable created: ./build/compiler"
    fi
else
    print_error "Build failed!"
    exit 1
fi