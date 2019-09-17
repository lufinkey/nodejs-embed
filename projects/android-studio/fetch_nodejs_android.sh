#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"
build_path="libnodejsembed/build"
source_path="libnodejsembed/src/main"

# ensure we don't rebuild if we don't need to
if [ ! -f "$base_dir/$build_path/libnode/bin/x86/libnode.so" ]; then
	cd "libnodejsembed/build" || exit $?
	rm -rf "libnode" && mkdir "libnode" && cd "libnode" || exit $?
	nodejs_mobile_url="https://github.com/JaneaSystems/nodejs-mobile/releases/download/nodejs-mobile-v0.2.1/nodejs-mobile-v0.2.1-android.zip"
	if [ -n "$(which curl)" ]; then
		curl -J -L "$nodejs_mobile_url" -o "nodejs-mobile-android.zip" || exit $?
	else
		wget --content-disposition "$nodejs_mobile_url" -O "nodejs-mobile-android.zip" || exit $?
	fi
	unzip "nodejs-mobile-android.zip" || exit $?
	rm -rf "nodejs-mobile-android.zip"
fi
if [ ! -f "$base_dir/$source_path/jniLibs/x86/libnode.so" ]; then
	cd "$base_dir/$source_path" || exit $?
	mkdir -p "jniLibs" || exit $?
	cd "$base_dir/$build_path/libnode/bin" || exit $?
	cp -r -f * "$base_dir/$source_path/jniLibs" || exit $?
fi
