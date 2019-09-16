#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"
build_path="build/mobile"

# build headers and clone repo
./build_mobile_headers.sh || exit $?

# ensure we don't rebuild if we don't need to
cd "$base_dir/$build_path"
mkdir -p "nodejs-mobile-android" || exit $?
cd "nodejs-mobile-android" || exit $?
if [ ! -f "x86/libnode.so" ]; then
	nodejs_mobile_url="https://github.com/JaneaSystems/nodejs-mobile/releases/download/nodejs-mobile-v0.2.1/nodejs-mobile-v0.2.1-android.zip"
	if [ -n "$(which curl)" ]; then
		curl -J -L "$nodejs_mobile_url" -o "nodejs-mobile-android.zip" || exit $?
	else
		wget --content-disposition "$nodejs_mobile_url" -O "nodejs-mobile-android.zip" || exit $?
	fi
	unzip "nodejs-mobile-android.zip" || exit $?
fi
