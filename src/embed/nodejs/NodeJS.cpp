//
//  NodeJS.cpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/6/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "NodeJS.hpp"
#include <limits>
#include <list>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <nodejs/node.h>
#include <nodejs/node_api.h>
#include <uv.h>
#include "NAPI_Macros.hpp"
#include "NAPI_Types.hpp"

namespace embed::nodejs {
	std::thread nodejsMainThread;
	std::mutex nodejsMainThreadMutex;
	
	struct NodeJSThread {
		uv_loop_t* loop = nullptr;
		std::thread::id threadId;
	};
	std::list<NodeJSThread> nodejsThreads;
	std::mutex nodejsThreadsMutex;
	
	void nodejs_main(int argc, char* argv[]);
	
	napi_value NativeModule_init(napi_env env, napi_value exports);
	napi_value NativeModule_send(napi_env env, napi_callback_info info);
	napi_value NativeModule_registerFunctions(napi_env env, napi_callback_info info);
	void NativeModule_main_cleanup(void*);
	
	void start(StartOptions options) {
		std::unique_lock<std::mutex> lock(nodejsMainThreadMutex);
		if(nodejsMainThread.joinable()) {
			throw std::logic_error("NodeJS already started");
		}
		if(options.args.size() >= (size_t)(std::numeric_limits<int>::max() - (2 + sizeof(int)))) {
			throw std::out_of_range("Too many arguments for nodejs");
		}
		// set environment variables
		if(options.modulePaths.size() > 0) {
			const char* prevNodePathStr = getenv("NODE_PATH");
			size_t prevNodePathLen = (prevNodePathStr != nullptr) ? strlen(prevNodePathStr) : 0;
			std::string nodePath;
			size_t strLength = (prevNodePathLen > 0) ? (prevNodePathLen + 1) : 0;
			for(auto& path : options.modulePaths) {
				strLength += path.length();
			}
			strLength += (options.modulePaths.size() - 1);
			nodePath.reserve(strLength);
			size_t index = 0;
			for(auto& path : options.modulePaths) {
				nodePath += path;
				if(index < (options.modulePaths.size()-1)) {
					nodePath += ':';
				}
				index++;
			}
			if(prevNodePathLen > 0) {
				nodePath += ':';
				nodePath.append(prevNodePathStr, prevNodePathLen);
			}
			setenv("NODE_PATH", nodePath.c_str(), 1);
		}
		
		std::vector<std::string> args = {
			"node",
			"-e",
			"\nconst native_embed = process.binding(\"__native_embed\");\nconsole.log(Object.getOwnPropertyDescriptors(native_embed));\n"
		};
		args.insert(args.end(), options.args.begin(), options.args.end());
		
		size_t argsDataSize = 0;
		for(auto& arg : args) {
			argsDataSize += (arg.length() + 1);
		}
		auto argsData = std::make_unique<char[]>(argsDataSize);
		auto argsDataPtr = argsData.get();
		auto argv = std::make_unique<char*[]>(args.size());
		size_t argIndex = 0;
		for(auto& arg : args) {
			std::strcpy(argsDataPtr, arg.data());
			argv[argIndex] = argsDataPtr;
			argsDataPtr += (arg.length() + 1);
			argIndex += 1;
		}
		int argc = (int)args.size();
		
		nodejsMainThread = std::thread([argsData=std::move(argsData), argv=std::move(argv), argc]() {
			nodejs_main(argc, argv.get());
		});
	}
	
	void nodejs_main(int argc, char* argv[]) {
		int exitCode = node::Start(argc, argv);
		printf("NodeJS exited with code %i\n", exitCode);
	}
	
	
	
	napi_value NativeModule_init(napi_env env, napi_value exports) {
		{
			// add thread to list
			std::unique_lock<std::mutex> lock(nodejsThreadsMutex);
			NodeJSThread nodejsThread;
			nodejsThread.threadId = std::this_thread::get_id();
			NAPI_CALL(env, napi_get_uv_event_loop(env, &nodejsThread.loop));
			NAPI_CALL(env, napi_add_env_cleanup_hook(env, NativeModule_main_cleanup, nodejsThread.loop));
			nodejsThreads.push_back(nodejsThread);
			lock.unlock();
		}
		napi_property_descriptor properties[] = {
			NAPI_METHOD_DESCRIPTOR("send", NativeModule_send),
			NAPI_METHOD_DESCRIPTOR("registerFunctions", NativeModule_registerFunctions)
		};
		NAPI_CALL(env, napi_define_properties(env, exports, sizeof(properties) / sizeof(*properties), properties));
		return exports;
	}
	
	napi_value NativeModule_send(napi_env env, napi_callback_info info) {
		size_t argc = 0;
		napi_value args[argc];
		
		NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
		NAPI_ASSERT(env, argc == 2, "Wrong number of arguments.");
		
		std::string eventName;
		NAPI_GET_STRING_FROM_VALUE(env, eventName, args[0]);
		
		// TODO forward message to listener
		if(eventName == "response") {
			// TODO match to corresponding request
		}
		else if(eventName == "request") {
			// TODO look for function to correspond to request
		}
		
		return nullptr;
	}
	
	napi_value NativeModule_registerFunctions(napi_env env, napi_callback_info info) {
		
		// TODO register functions
		
		return nullptr;
	}
	
	void NativeModule_main_cleanup(uv_loop_t* loop) {
		// remove thread from list
		std::unique_lock<std::mutex> lock(nodejsThreadsMutex);
		auto it = std::find_if(nodejsThreads.begin(), nodejsThreads.end(), [=](NodeJSThread& item) {
			return (item.loop == loop);
		});
		if(it != nodejsThreads.end()) {
			nodejsThreads.erase(it);
		}
		lock.unlock();
	}
}

NAPI_MODULE_X(__native_embed, embed::nodejs::NativeModule_init, nullptr, 0x1)
