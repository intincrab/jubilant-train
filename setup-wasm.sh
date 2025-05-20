#!/bin/bash

# Exit on error
set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}Setting up Emscripten SDK and building WebAssembly version...${NC}"

# Check if Emscripten is already installed
if command -v emcc &> /dev/null; then
  echo -e "${GREEN}Emscripten already installed.${NC}"
else
  echo -e "${YELLOW}Installing Emscripten SDK...${NC}"
  
  # Install dependencies
  echo -e "${YELLOW}Installing dependencies...${NC}"
  sudo apt-get update
  sudo apt-get install -y git python3 cmake build-essential
  
  # Clone emsdk repo
  if [ ! -d "emsdk" ]; then
    git clone https://github.com/emscripten-core/emsdk.git
  fi
  
  # Enter that directory and fetch + pull latest
  cd emsdk
  git pull
  
  # Download and install the latest SDK tools
  ./emsdk install latest
  
  # Make the "latest" SDK "active" for the current user
  ./emsdk activate latest
  
  # Activate path variables
  source ./emsdk_env.sh
  
  cd ..
  
  echo -e "${GREEN}Emscripten SDK installed successfully.${NC}"
fi

# Activate Emscripten environment
echo -e "${YELLOW}Activating Emscripten environment...${NC}"
source ./emsdk/emsdk_env.sh

# Build the WebAssembly version
echo -e "${YELLOW}Building WebAssembly version...${NC}"
make wasm

echo -e "${GREEN}WebAssembly build complete!${NC}"
echo -e "${GREEN}The compiled files are in the 'public' directory.${NC}"
echo -e "${GREEN}You can serve this directory with a web server to test the compiler.${NC}"
echo -e "${YELLOW}For example:${NC}"
echo -e "cd public && python3 -m http.server" 