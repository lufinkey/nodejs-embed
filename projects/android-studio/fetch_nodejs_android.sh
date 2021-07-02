#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"

if [ ! -f "$base_dir/libnode/bin/x86/libnode.so" ]; then
	# Download NodeJS Mobile distribution]
	rm -rf "libnode" && mkdir "libnode" && cd "libnode" || exit $?
	nodejs_mobile_url="https://github.com/JaneaSystems/nodejs-mobile/releases/download/nodejs-mobile-v0.3.2/nodejs-mobile-v0.3.2-android.zip"
	if [ -n "$(which curl)" ]; then
		curl -J -L "$nodejs_mobile_url" -o "nodejs-mobile-android.zip" || exit $?
	else
		wget --content-disposition "$nodejs_mobile_url" -O "nodejs-mobile-android.zip" || exit $?
	fi
	unzip "nodejs-mobile-android.zip" || exit $?
	rm -rf "nodejs-mobile-android.zip"
fi
