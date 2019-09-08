#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"

# clone nodejs
mkdir -p "build" || exit $?
cd "build"
if [ ! -d "node/.git" ] && [ ! -f "node/.git" ]; then
	git clone --recursive "https://github.com/nodejs/node" || exit $?
fi
