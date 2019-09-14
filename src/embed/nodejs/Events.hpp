//
//  Events.hpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/10/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include "ProcessEventListener.hpp"
#ifdef __OBJC__
#import "NodeJSProcessEventDelegate.h"
#endif

namespace embed::nodejs {
	void emit(std::string eventName, napi_value data);
	void addProcessEventListener(ProcessEventListener* listener);
	void removeProcessEventListener(ProcessEventListener* listener);
	
	#ifdef __OBJC__
	void emit(NSString* eventName, napi_value data);
	void addProcessEventDelegate(id<NodeJSProcessEventDelegate> delegate);
	void removeProcessEventDelegate(id<NodeJSProcessEventDelegate> delegate);
	#endif
}
