//
//  EventDispatch.cpp
//  NodeJSEmbed-macOS-static
//
//  Created by Luis Finke on 9/10/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "Events.hpp"
#include "EventDispatch.hpp"
#include <list>
#include <mutex>

namespace embed::nodejs {
	std::list<ProcessEventListener*> processEventListeners;
	std::mutex processEventListenersMutex;
	
	void addProcessEventListener(ProcessEventListener* listener) {
		std::unique_lock<std::mutex> lock(processEventListenersMutex);
		processEventListeners.push_back(listener);
	}
	
	void removeProcessEventListener(ProcessEventListener* listener) {
		std::unique_lock<std::mutex> lock(processEventListenersMutex);
		auto it = std::find(processEventListeners.begin(), processEventListeners.end(), listener);
		if(it != processEventListeners.end()) {
			processEventListeners.erase(it);
		}
	}
	
	void dispatchProcessEvent(ProcessEventType eventType, std::vector<void*> args) {
		_dispatchProcessListenerEvent(eventType, args);
		#ifdef __APPLE__
		_dispatchProcessDelegateEvent(eventType, args);
		#endif
	}
	
	void _dispatchProcessListenerEvent(ProcessEventType eventType, std::vector<void*> args) {
		std::unique_lock<std::mutex> lock(processEventListenersMutex);
		std::list<ProcessEventListener*> listeners = processEventListeners;
		lock.unlock();
		for(auto listener : listeners) {
			switch(eventType) {
				case ProcessEventType::WILL_START: {
					std::vector<std::string> procArgs;
					int argc = *((int*)args.at(0));
					char** argv = (char**)args.at(1);
					procArgs.reserve((size_t)argc);
					for(int i=0; i<argc; i++) {
						procArgs.push_back(argv[i]);
					}
					listener->onNodeJSProcessWillStart(procArgs);
				} break;
				
				case ProcessEventType::DID_START: {
					napi_env env = (napi_env)args.at(0);
					listener->onNodeJSProcessDidStart(env);
				} break;
				
				case ProcessEventType::WILL_END: {
					napi_env env = (napi_env)args.at(0);
					listener->onNodeJSProcessWillEnd(env);
				} break;
				
				case ProcessEventType::DID_END: {
					int exitCode = *((int*)args.at(0));
					listener->onNodeJSProcessDidEnd(exitCode);
				}
			}
		}
	}
}
