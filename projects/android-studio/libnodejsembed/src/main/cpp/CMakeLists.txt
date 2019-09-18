# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 17 )

execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../fetch_nodejs_android.sh")
execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build_mobile_headers.sh")
execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../src/embed/nodejs/js/build.sh")

include_directories(
		"${CMAKE_CURRENT_SOURCE_DIR}"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../src"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../src/embed/nodejs/js/build"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build/mobile/include"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build/mobile/include/nodejs"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build/mobile/include/chakrashim"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build/mobile/include/chakracore"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build/mobile/include/uv"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build/addon-api")

file(GLOB_RECURSE NODEJS_SOURCES ../../../../../../src/embed/*.cpp)

add_library(NodeJSEmbed STATIC
		NodeJSEmbed_android.cpp
		${NODEJS_SOURCES})

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
