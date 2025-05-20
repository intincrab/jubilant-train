#!/bin/bash
set -e

# Create a directory for Emscripten if it doesn't exist
mkdir -p ~/emsdk

# Clone emsdk repo if not already cloned
if [ ! -d ~/emsdk/.git ]; then
  echo "Cloning Emscripten SDK..."
  cd ~/emsdk
  git clone https://github.com/emscripten-core/emsdk.git .
else
  echo "Emscripten SDK already cloned, updating..."
  cd ~/emsdk
  git pull
fi

# Install and activate latest Emscripten SDK
cd ~/emsdk
./emsdk install latest
./emsdk activate latest

# Source the environment variables
source ./emsdk_env.sh

# Return to the tiny-compiler directory
cd /home/ajay/tiny-compiler

# Build the WebAssembly version
echo "Building WebAssembly version..."
make wasm

echo "WebAssembly build complete!"
echo "The compiled files are in the 'public' directory."
echo "You can access the WebAssembly version at: http://localhost:8000/"

# If the web server is not running, start it
if ! pgrep -f "python3 -m http.server 8000" > /dev/null; then
  echo "Starting web server..."
  cd public
  python3 -m http.server 8000 &
  echo "Web server started at http://localhost:8000/"
else
  echo "Web server already running at http://localhost:8000/"
fi 