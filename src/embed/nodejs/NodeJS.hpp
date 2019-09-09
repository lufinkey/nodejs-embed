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

namespace embed::nodejs {
	struct StartOptions {
		std::vector<std::string> args;
		std::vector<std::string> modulePaths;
	};
	
	void start(StartOptions options = {});
	void queue(std::function<void()> work);
}
