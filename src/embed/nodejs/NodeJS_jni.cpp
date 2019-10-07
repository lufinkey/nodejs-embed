//
//  NodeJS_jni.cpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/15/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#ifdef __ANDROID__

#define NODEJSEMBED_JNI_ENABLED

#include "NodeJS.hpp"
#include "NodeJS_jni.hpp"
#include "EventDispatch.hpp"
#include <jni.h>
#include <list>
#include <mutex>
#include <stdexcept>
#include <string>

namespace embed::nodejs {
	ScopedJNIEnv::ScopedJNIEnv(JavaVM* vm): vm(vm), env(nullptr), attachedToThread(false) {
		if(vm == nullptr) {
			throw std::runtime_error("given VM is null");
		}
		auto envResult = vm->GetEnv((void**)&env, JNI_VERSION_1_6);
		if (envResult == JNI_EDETACHED) {
			if (vm->AttachCurrentThread(&env, NULL) == JNI_OK) {
				attachedToThread = true;
			} else {
				throw std::runtime_error("Failed to attach to thread");
			}
		} else if (envResult == JNI_EVERSION) {
			throw std::runtime_error("Unsupported JNI version");
		}
	}

	ScopedJNIEnv::~ScopedJNIEnv() {
		detach();
	}

	JNIEnv* ScopedJNIEnv::getEnv() const {
		return env;
	}
	
	void ScopedJNIEnv::detach() {
		if (attachedToThread) {
			vm->DetachCurrentThread();
			attachedToThread = false;
		}
	}



	JavaVM* mainJavaVM = nullptr;
	std::list<jobject> jniProcEventListeners;
	std::mutex jniProcEventListenersMutex;

	std::string jniProcessEventListenerClassName = "com/lufinkey/embed/NodeJS$ProcessEventListener";
	jclass jniProcessEventListenerClass(JNIEnv* env) {
		jclass jniClass = env->FindClass(jniProcessEventListenerClassName.c_str());
		if(jniClass == nullptr) {
			throw std::runtime_error("Could not find class named " + jniProcessEventListenerClassName);
		}
		return jniClass;
	}

	void addJavaProcessEventListener(JNIEnv* env, jobject listener) {
		if(env->IsInstanceOf(listener, jniProcessEventListenerClass(env)) != JNI_TRUE) {
			throw std::runtime_error("listener is not of type " + jniProcessEventListenerClassName);
		}
		std::unique_lock<std::mutex> lock(jniProcEventListenersMutex);
		jniProcEventListeners.push_back(env->NewGlobalRef(listener));
	}

	void removeJavaProcessEventListener(JNIEnv* env, jobject listener) {
		if(env->IsInstanceOf(listener, jniProcessEventListenerClass(env)) != JNI_TRUE) {
			throw std::runtime_error("listener is not of type " + jniProcessEventListenerClassName);
		}
		std::unique_lock<std::mutex> lock(jniProcEventListenersMutex);
		auto it = std::find_if(jniProcEventListeners.begin(), jniProcEventListeners.end(), [&](auto& item) {
			return env->IsSameObject(listener, item);
		});
		if(it != jniProcEventListeners.end()) {
			env->DeleteGlobalRef(*it);
			jniProcEventListeners.erase(it);
		}
	}

	void _dispatchJavaProcessListenerEvent(ProcessEventType eventType, std::vector<void*> args) {
		if(jniProcEventListeners.size() == 0) {
			return;
		}
		if(mainJavaVM == nullptr) {
			throw std::runtime_error("No Java VM has been attached via NodeJS.init");
		}
		ScopedJNIEnv scopedEnv(mainJavaVM);
		JNIEnv* javaEnv = scopedEnv.getEnv();
		jclass listenerClass = javaEnv->GetObjectClass(jniProcEventListeners.front());

		std::unique_lock<std::mutex> lock(jniProcEventListenersMutex);
		std::list<jobject> listeners = jniProcEventListeners;
		lock.unlock();

		switch(eventType) {
			case ProcessEventType::WILL_START: {
				int argc = *((int*)args.at(0));
				char** argv = (char**)args.at(1);
				jclass stringClass = javaEnv->FindClass("java/lang/String");
				jobjectArray procArgs = javaEnv->NewObjectArray((jsize)argc, stringClass, nullptr);
				for(int i=0; i<argc; i++) {
					javaEnv->SetObjectArrayElement(procArgs, (jsize)i, javaEnv->NewStringUTF(argv[i]));
				}
				jmethodID methodId = javaEnv->GetMethodID(listenerClass, "onNodeJSProcessWillStart", "([Ljava/lang/String;)V");
				for(auto listener : listeners) {
					javaEnv->CallVoidMethod(listener, methodId, procArgs);
				}
			} break;

			case ProcessEventType::DID_START: {
				jmethodID methodId = javaEnv->GetMethodID(listenerClass, "onNodeJSProcessDidStart", "()V");
				for(auto listener : listeners) {
					javaEnv->CallVoidMethod(listener, methodId);
				}
			} break;

			case ProcessEventType::WILL_END: {
				jmethodID methodId = javaEnv->GetMethodID(listenerClass, "onNodeJSProcessWillEnd", "()V");
				for(auto listener : listeners) {
					javaEnv->CallVoidMethod(listener, methodId);
				}
			} break;

			case ProcessEventType::DID_END: {
				int exitCode = *((int*)args.at(0));
				jmethodID methodId = javaEnv->GetMethodID(listenerClass, "onNodeJSProcessDidEnd", "(I)V");
				for(auto listener : listeners) {
					javaEnv->CallVoidMethod(listener, methodId, (jint)exitCode);
				}
			} break;

			case ProcessEventType::EMIT_EVENT: {
				// TODO figure out how this should be implemented
			} break;
		}
	}
}



extern "C" JNIEXPORT void JNICALL
Java_com_lufinkey_embed_NodeJS_init(JNIEnv* env, jclass) {
	auto vmResult = env->GetJavaVM(&embed::nodejs::mainJavaVM);
	if(vmResult != 0 || embed::nodejs::mainJavaVM == nullptr) {
		throw std::runtime_error("Could not get java VM");
	}
}

extern "C" JNIEXPORT void JNICALL
Java_com_lufinkey_embed_NodeJS_start(JNIEnv* env, jclass) {
	embed::nodejs::start();
}

extern "C" JNIEXPORT void JNICALL
Java_com_lufinkey_embed_NodeJS_addProcessEventListener(JNIEnv* env, jclass, jobject listener) {
	embed::nodejs::addJavaProcessEventListener(env, listener);
}

extern "C" JNIEXPORT void JNICALL
Java_com_lufinkey_embed_NodeJS_removeProcessEventListener(JNIEnv* env, jclass, jobject listener) {
	embed::nodejs::removeJavaProcessEventListener(env, listener);
}

#endif
