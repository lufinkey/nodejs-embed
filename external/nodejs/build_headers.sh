#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"

# fetch nodejs
./fetch_node.sh || exit $?

# link include files
cd "$base_dir" || exit $?
mkdir -p "build/include" || exit $?
cd "build/include" || exit $?
rm -rf "nodejs" || exit $?
ln -s "../node/src" "nodejs" || exit $?
