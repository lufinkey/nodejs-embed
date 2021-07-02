
project(TestApp)
cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 17 )

set( NODEJSEMBED_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.." )

add_subdirectory(
		"${NODEJSEMBED_ROOT}/projects/android-studio/libnodejsembed/src/main/cpp"
		NodeJSEmbed_build )

add_library( TestApp SHARED native-lib.cpp )

find_library(log-lib log)

target_link_libraries(
		TestApp

		NodeJSEmbed
		${log-lib})
