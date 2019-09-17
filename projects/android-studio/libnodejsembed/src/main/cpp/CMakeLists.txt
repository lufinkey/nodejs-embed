# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 17 )

execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../fetch_nodejs_android.sh")
execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../external/nodejs/build_mobile_headers.sh")

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

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
		NodeJSEmbed

		# Sets the library as a static library.
		STATIC

		# Provides a relative path to your source file(s).
		NodeJSEmbed_android.cpp
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../src/embed/nodejs/NodeJS.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../src/embed/nodejs/NodeJS_jni.cpp")

add_library(node SHARED IMPORTED)
set_target_properties(node PROPERTIES IMPORTED_LOCATION
		"${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libnode.so")

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
		log-lib

		# Specifies the name of the NDK library that
		# you want CMake to locate.
		log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
		NodeJSEmbed

		# Links the target library to the log library
		# included in the NDK.
		node
		${log-lib})
