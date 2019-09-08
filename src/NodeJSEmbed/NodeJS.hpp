//
//  NodeJS.hpp
//  SoundHoleCore
//
//  Created by Luis Finke on 9/6/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <mutex>
#include <string>
#include <vector>

namespace embed::nodejs {
	class NodeJS {
	public:
		struct Options {
			std::vector<std::string> args;
			std::vector<std::string> modulePaths;
		};
		
		static void start(Options options = {});
		
	private:
		NodeJS(std::vector<std::string> args);
		
		static NodeJS* instance;
	};
}
