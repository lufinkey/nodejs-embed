#include <jni.h>
#include <string>
#include <embed/nodejs/NodeJS.hpp>

extern "C" JNIEXPORT jstring JNICALL
Java_com_lufinkey_nodejsembedtest_MainActivity_stringFromJNI(
		JNIEnv *env,
		jobject /* this */) {
	std::string hello = "Hello from C++";
	return env->NewStringUTF(hello.c_str());
}
