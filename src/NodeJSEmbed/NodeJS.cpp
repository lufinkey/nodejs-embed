//
//  NodeJS.cpp
//  SoundHoleCore
//
//  Created by Luis Finke on 9/6/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "NodeJS.hpp"
#include <nodejs/node.h>
#include <stdlib.h>


namespace sh {
	void NodeJS::start(Options options) {
		// set environment variables
		if(options.modulePaths.size() > 0) {
			const char* nodePathStr = getenv("NODE_PATH");
			std::string nodePath = "";
			if(nodePathStr != nullptr) {
				nodePath = nodePathStr;
			}
			if(nodePath.length() > 0) {
				nodePath += ":";
			}
			size_t strLength = nodePath.length();
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
			setenv("NODE_PATH", nodePath.c_str(), 1);
		}
		
	}
}
