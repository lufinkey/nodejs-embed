#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"
build_path="build/mobile"

# clone nodejs
cd "$base_dir" || exit $?
mkdir -p "$build_path" || exit $?
cd "$build_path"
if [ ! -d "node/.git" ] && [ ! -f "node/.git" ]; then
	git clone --recursive "https://github.com/JaneaSystems/nodejs-mobile" "node" || exit $?
fi

# link include files
cd "$base_dir/$build_path" || exit $?
mkdir -p "include" || exit $?
cd "include" || exit $?
rm -rf "node" "v8" "uv" || exit $?
cp -r "../node/src" "node" \
	&& cp -r "../node/deps/v8/include" "v8" \
	&& cp -r "../node/deps/uv/include" "uv"

# clone addon api
cd "$base_dir"
./fetch_addon_api.sh || exit $?
