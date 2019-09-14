#!/bin/bash

# enter script directory
base_dir=$(dirname "${BASH_SOURCE[0]}")
cd "$base_dir"
base_dir="$PWD"

npm run build || exit $?
