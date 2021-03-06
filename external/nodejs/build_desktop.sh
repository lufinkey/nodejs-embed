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
	git clone --recursive "https://github.com/nodejs/node.git" "node" || exit $?
	cd "node"
	git checkout "v12.10.0"
fi

# link include files
cd "$base_dir/$build_path" || exit $?
mkdir -p "include" || exit $?
cd "include" || exit $?
rm -rf "nodejs" "v8" "uv" || exit $?
cp -r "../node/src" "nodejs" \
	&& cp -r "../node/deps/v8/include" "v8" \
	&& cp -r "../node/deps/uv/include" "uv" \
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
	rm -rf "torque-generated"
	cp -r -f *.a obj/gen/*.cc obj/gen/*.h \
		"obj/gen/torque-generated" "obj/gen/torque-output-root/torque-generated" \
		"obj/gen/generate-bytecode-output-root/builtins-generated" \
		"$base_dir/$build_path/$build_type"
fi

# clone addon api
cd "$base_dir"
./fetch_addon_api.sh || exit $?
