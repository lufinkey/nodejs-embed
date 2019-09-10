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
rm -rf "nodejs" "chakrashim" "chakracore" || exit $?
cp -r "../node/src" "nodejs" \
	&& cp -r "../node/deps/chakrashim/include" "chakrashim" \
	&& cp -r "../node/deps/chakrashim/core/lib/Jsrt" "chakracore" \
	&& cp -r "../node/deps/uv/include" "uv" \
	|| exit $?
