#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"

# get arguments
build_type="$1"
if [ -z "$build_type" ]; then
	build_type="Release"
fi
configure_args=()
if [ "$build_type" == "Debug" ]; then
	configure_args+=("--debug")
fi

# fetch nodejs
./fetch_node.sh || exit $?

# build header files
./build_headers.sh || exit $?

# build nodejs
cd "$base_dir/build/node" || exit $?
./configure --fully-static --enable-static "${configure_args[@]}" || exit $?
make -j4 || exit $?

# copy output files
cd "$base_dir/build" || exit $?
mkdir -p "$build_type" || exit $?
cd "$base_dir/build/node/out/$build_type" || exit $?
cp *.a "$base_dir/build/$build_type" || exit $?
