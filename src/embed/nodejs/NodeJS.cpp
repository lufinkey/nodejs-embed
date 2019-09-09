//
//  NodeJS.cpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/6/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "NodeJS.hpp"
#include <limits>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <nodejs/node.h>
#include <nodejs/node_api.h>
#include "NAPI_Macros.hpp"
#include "NAPI_Types.hpp"

napi_value NodeJSEmbed_init(napi_env env, napi_value exports);
napi_value NodeJSEmbed_send(napi_env env, napi_callback_info info);

namespace embed::nodejs {
	std::thread nodejsThread;
	std::mutex nodejsThreadMutex;
	
	void nodejs_main(int argc, char* argv[]);
	
	void start(StartOptions options) {
		std::lock_guard<std::mutex> lock(nodejsThreadMutex);
		if(nodejsThread.joinable()) {
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
			"\nconst Module = require(\"module\");\nconsole.log(\"Module: \", Module);\n"
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
			strcpy(argsDataPtr, arg.data());
			argv[argIndex] = argsDataPtr;
			argsDataPtr += (arg.length() + 1);
			argIndex += 1;
		}
		int argc = (int)args.size();
		
		nodejsThread = std::thread([argsData=std::move(argsData), argv=std::move(argv), argc]() {
			nodejs_main(argc, argv.get());
		});
	}
	
	void nodejs_main(int argc, char* argv[]) {
		int exitCode = node::Start(argc, argv);
		printf("NodeJS exited with code %i\n", exitCode);
	}
}




napi_value NodeJSEmbed_init(napi_env env, napi_value exports) {
	napi_property_descriptor properties[] = {
		NAPI_METHOD_DESCRIPTOR("send", NodeJSEmbed_send)
	};
	NAPI_CALL(env, napi_define_properties(env, exports, sizeof(properties) / sizeof(*properties), properties));
	return exports;
}

napi_value NodeJSEmbed_send(napi_env env, napi_callback_info info) {
	size_t argc = 0;
	napi_value args[argc];
	
	NAPI_CALL(env, napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));
	NAPI_ASSERT(env, argc == 2, "Wrong number of arguments.");
	
	std::string eventName;
	NAPI_GET_STRING_FROM_VALUE(env, eventName, args[0]);
	
	// TODO forward message to listener
	
	return nullptr;
}

NAPI_MODULE(__native_embed, NodeJSEmbed_init)
