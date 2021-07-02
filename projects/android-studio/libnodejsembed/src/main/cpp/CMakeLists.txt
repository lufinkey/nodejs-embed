
project(NodeJSEmbed)
cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 17 )

set( NODEJSEMBED_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.." )

execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../fetch_nodejs_android.sh")
execute_process(COMMAND "${NODEJSEMBED_ROOT}/external/nodejs/build_mobile_headers.sh")
execute_process(COMMAND "${NODEJSEMBED_ROOT}/src/embed/nodejs/js/build.sh")

file(GLOB_RECURSE NODEJSEMBED_SOURCES "${NODEJSEMBED_ROOT}/src/embed/*.cpp")

add_library( NodeJSEmbed SHARED
		${NODEJSEMBED_SOURCES}
		_unused.cpp )

target_include_directories(NodeJSEmbed PUBLIC
		"${NODEJSEMBED_ROOT}/src")

target_include_directories(NodeJSEmbed PRIVATE
		"${NODEJSEMBED_ROOT}/src/embed/nodejs/js/build"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include/nodejs"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include/v8"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/mobile/include/uv"
		"${NODEJSEMBED_ROOT}/external/nodejs/build/addon-api")

add_library(node SHARED IMPORTED)
set_target_properties(node PROPERTIES IMPORTED_LOCATION
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../libnode/bin/${ANDROID_ABI}/libnode.so")

find_library(log-lib log)

target_link_libraries(
		NodeJSEmbed

		node
		${log-lib})
