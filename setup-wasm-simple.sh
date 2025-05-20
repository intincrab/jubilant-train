#!/bin/bash

# Exit on error
set -e

echo "Building the native version of the compiler..."
make

echo "Checking if we can compile the example file..."
./build/tiny-compiler examples/example.tiny

echo "Starting a web server to serve the static files..."
cd public
python3 -m http.server 8000 