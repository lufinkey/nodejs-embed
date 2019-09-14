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
#include "Events.hpp"

namespace embed::nodejs {
	struct StartOptions {
		std::vector<std::string> args;
		std::vector<std::string> prependPaths;
		std::vector<std::string> appendPaths;
	};
	void start(StartOptions options = {});
	
	struct EventLoop;
	
	std::vector<EventLoop*> getEventLoops();
	EventLoop* getMainEventLoop();
	
	void queueMain(std::function<void(napi_env)> work);
	void queue(EventLoop* loop, std::function<void(napi_env)> work);
	
	struct LoadOptions {
		std::vector<std::string> prependPaths;
		std::vector<std::string> appendPaths;
	};
	napi_value loadModuleFromMemory(napi_env env, std::string name, const char* buffer, size_t bufferLength, LoadOptions options = {});
	napi_value loadModuleFromMemory(napi_env env, std::string name, std::string buffer, LoadOptions options = {});
	void unloadModule(napi_env env, std::string name);
	
	napi_value require(napi_env env, std::string moduleName);
}
