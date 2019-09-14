//
//  ProcessEventListener.hpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/10/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <vector>
#include <string>
#include "NAPI_Types.hpp"

namespace embed::nodejs {
	class ProcessEventListener {
	public:
		virtual ~ProcessEventListener() {}
		
		virtual void onNodeJSProcessWillStart(const std::vector<std::string>& args) {}
		virtual void onNodeJSProcessDidStart(napi_env env) {}
		virtual void onNodeJSProcessWillEnd(napi_env env) {}
		virtual void onNodeJSProcessDidEnd(int exitCode) {}
		
		virtual void onNodeJSProcessDidEmitEvent(napi_env env, std::string eventName, napi_value data) {};
	};
}
