# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 17 )

set( NODEJSEMBED_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.." )

execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../fetch_nodejs_android.sh")
execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build_mobile_headers.sh")
execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../src/embed/nodejs/js/build.sh")

include_directories(
		"${CMAKE_CURRENT_SOURCE_DIR}"
		"${NODEJSEMBED_ROOT}/src"
		"${NODEJSEMBED_ROOT}/src/embed/nodejs/js/build"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include/nodejs"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include/chakrashim"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include/chakracore"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include/uv"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/addon-api")

file(GLOB_RECURSE NODEJSEMBED_SOURCES ../../../../../../src/embed/*.cpp)

add_library(NodeJSEmbed STATIC
		NodeJSEmbed_android.cpp
		${NODEJSEMBED_SOURCES})

add_library(node SHARED IMPORTED)
set_target_properties(node PROPERTIES IMPORTED_LOCATION
		"${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libnode.so")

find_library(
		log-lib
		log)

target_link_libraries(
		NodeJSEmbed

		node
		${log-lib})
