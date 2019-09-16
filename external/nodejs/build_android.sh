#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"
build_path="build/mobile"

# build headers and clone repo
./build_mobile_headers.sh || exit $?

# clone NDK if needed
if [ -z "$ANDROID_HOME" ] || [ ! -d "$ANDROID_HOME" ] || [ ! -d "$ANDROID_HOME/ndk-bundle" ]; then
	cd "$base_dir/$build_path"
	ndk_url="https://dl.google.com/android/repository/android-ndk-r18b-darwin-x86_64.zip"
	if [ -n "$(which curl)" ]; then
		curl "$ndk_url" -o "android_ndk.zip" || exit $?
	else
		wget "$ndk_url" -O "android_ndk.zip" || exit $?
	fi
	unzip "android_ndk.zip" || exit $?
	NDK_DIR="$PWD/android-ndk-r18b"
else
	NDK_DIR="$ANDROID_HOME/ndk-bundle"
fi

# build node for android
cd "$base_dir/$build_path/node" || exit $?
./tools/android_build.sh "$NDK_DIR" || exit $?
