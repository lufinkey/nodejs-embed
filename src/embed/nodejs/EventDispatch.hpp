//
//  EventDispatch.hpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/10/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <vector>

namespace embed::nodejs {
	enum class ProcessEventType {
		WILL_START,
		DID_START,
		WILL_END,
		DID_END
	};
	
	void dispatchProcessEvent(ProcessEventType eventType, std::vector<void*> args);
	void _dispatchProcessListenerEvent(ProcessEventType eventType, std::vector<void*> args);
	#ifdef __APPLE__
	void _dispatchProcessDelegateEvent(ProcessEventType eventType, std::vector<void*> args);
	#endif
}
