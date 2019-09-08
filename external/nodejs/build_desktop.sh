#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"
build_path="build/desktop"

# get arguments
build_type="$1"
if [ -z "$build_type" ]; then
	build_type="Release"
fi
configure_args=()
if [ "$build_type" == "Debug" ]; then
	configure_args+=("--debug")
fi

# clone nodejs
cd "$base_dir" || exit $?
mkdir -p "$build_path" || exit $?
cd "$build_path"
if [ ! -d "node/.git" ] && [ ! -f "node/.git" ]; then
	git clone --recursive "https://github.com/nodejs/node" || exit $?
fi

# link include files
cd "$base_dir/$build_path" || exit $?
mkdir -p "include" || exit $?
cd "include" || exit $?
rm -rf "nodejs" "v8" || exit $?
cp -r "../node/src" "nodejs" \
	&& cp -r "../node/deps/v8/include" "v8" \
	|| exit $?

# build nodejs if needed
if [ ! -f "$base_dir/$build_path/$build_type/libnode.a" ]; then
	# build nodejs
	cd "$base_dir/$build_path/node" || exit $?
	./configure --fully-static --enable-static "${configure_args[@]}" || exit $?
	make -j4 || exit $?

	# copy output files
	cd "$base_dir/$build_path" || exit $?
	mkdir -p "$build_type" || exit $?
	cd "$base_dir/$build_path/node/out/$build_type" || exit $?
	cp -f *.a "$base_dir/$build_path/$build_type" || exit $?
fi
