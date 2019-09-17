//
//  NodeJS.hpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/6/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <functional>
#include <string>
#include <vector>
#include "NAPI_Types.hpp"
#include "ProcessEventListener.hpp"
#ifdef __OBJC__
#import "NodeJSProcessEventDelegate.h"
#endif
#if defined(NODEJSEMBED_JNI_ENABLED)
#include <jni.h>
#endif

namespace embed::nodejs {
	// Initialization / Status
	struct StartOptions {
		std::vector<std::string> args;
		std::vector<std::string> prependPaths;
		std::vector<std::string> appendPaths;
	};
	void start(StartOptions options = {});
	bool isRunning();
	
	// Event Loop
	struct EventLoop;
	std::vector<EventLoop*> getEventLoops();
	EventLoop* getMainEventLoop();
	
	// Queue
	void queueMain(std::function<void(napi_env)> work);
	void queue(EventLoop* loop, std::function<void(napi_env)> work);
	
	// Load / Unload
	struct LoadOptions {
		std::vector<std::string> prependPaths;
		std::vector<std::string> appendPaths;
	};
	napi_value loadModuleFromMemory(napi_env env, std::string name, const char* buffer, size_t bufferLength, LoadOptions options = {});
	napi_value loadModuleFromMemory(napi_env env, std::string name, std::string buffer, LoadOptions options = {});
	void unloadModule(napi_env env, std::string name);
	napi_value require(napi_env env, std::string moduleName);

	// Events
	void emit(std::string eventName, napi_value data);
	void addProcessEventListener(ProcessEventListener* listener);
	void removeProcessEventListener(ProcessEventListener* listener);

	#ifdef __OBJC__
	void emit(NSString* eventName, napi_value data);
	void addProcessEventDelegate(id<NodeJSProcessEventDelegate> delegate);
	void removeProcessEventDelegate(id<NodeJSProcessEventDelegate> delegate);
	#endif

	#ifdef NODEJSEMBED_JNI_ENABLED
	void addJavaProcessEventListener(JNIEnv* env, jobject listener);
	void removeJavaProcessEventListener(JNIEnv* env, jobject listener);
	#endif
}
