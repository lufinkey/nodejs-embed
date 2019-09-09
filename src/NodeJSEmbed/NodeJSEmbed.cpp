//
//  NodeJSEmbed.cpp
//  SoundHoleCore
//
//  Created by Luis Finke on 9/6/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "NodeJSEmbed.hpp"
#include <nodejs/node.h>
#include <nodejs/node_api.h>
#include "NAPI_Macros.hpp"
#include <stdlib.h>

napi_value NodeJSEmbed_init(napi_env env, napi_value exports);
napi_value NodeJSEmbed_send(napi_env env, napi_callback_info info);

namespace embed::nodejs {
	void start(StartOptions options) {
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
		
		// TODO start NodeJS
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

NAPI_MODULE(cpp_embed, NodeJSEmbed_init)
