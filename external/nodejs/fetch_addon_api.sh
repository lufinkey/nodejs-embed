#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"
build_path="build/desktop"

# clone addon API
if [ ! -d "build/addon-api/.git" ] && [ ! -f "build/addon-api/.git" ]; then
	git clone --recursive 'https://github.com/nodejs/node-addon-api' 'build/addon-api'
fi
