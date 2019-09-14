//
//  Events.mm
//  NodeJSEmbed-macOS-static
//
//  Created by Luis Finke on 9/10/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#ifdef __APPLE__

#include "Events.hpp"
#include "EventDispatch.hpp"
#include <list>
#include <mutex>

#import <Foundation/Foundation.h>
#import "NodeJSProcessEventDelegate.h"

namespace embed::nodejs {
	NSMutableArray<id<NodeJSProcessEventDelegate>>* processEventDelegates = [NSMutableArray new];
	std::mutex processEventDelegatesMutex;
	
	void emit(NSString* eventName, napi_value data) {
		emit(std::string(eventName.UTF8String), data);
	}
	
	void addProcessEventDelegate(id<NodeJSProcessEventDelegate> delegate) {
		std::unique_lock<std::mutex> lock(processEventDelegatesMutex);
		[processEventDelegates addObject:delegate];
	}
	
	void removeProcessEventDelegate(id<NodeJSProcessEventDelegate> delegate) {
		std::unique_lock<std::mutex> lock(processEventDelegatesMutex);
		[processEventDelegates removeObject:delegate];
	}
	
	void _dispatchProcessDelegateEvent(ProcessEventType eventType, std::vector<void*> args) {
		std::unique_lock<std::mutex> lock(processEventDelegatesMutex);
		NSArray<id<NodeJSProcessEventDelegate>>* delegates = [NSArray arrayWithArray:processEventDelegates];
		lock.unlock();
		for(id<NodeJSProcessEventDelegate> delegate in delegates) {
			switch(eventType) {
				case ProcessEventType::WILL_START: {
					if([delegate respondsToSelector:@selector(nodejsProcessWillStart:)]) {
						NSMutableArray<NSString*>* procArgs = [NSMutableArray new];
						int argc = *((int*)args.at(0));
						char** argv = (char**)args.at(1);
						for(int i=0; i<argc; i++) {
							[procArgs addObject:[NSString stringWithUTF8String:argv[i]]];
						}
						[delegate nodejsProcessWillStart:procArgs];
					}
				} break;
				
				case ProcessEventType::DID_START: {
					if([delegate respondsToSelector:@selector(nodejsProcessDidStart:)]) {
						napi_env env = (napi_env)args.at(0);
						[delegate nodejsProcessDidStart:env];
					}
				} break;
				
				case ProcessEventType::WILL_END: {
					if([delegate respondsToSelector:@selector(nodejsProcessWillEnd:)]) {
						napi_env env = (napi_env)args.at(0);
						[delegate nodejsProcessWillEnd:env];
					}
				} break;
				
				case ProcessEventType::DID_END: {
					if([delegate respondsToSelector:@selector(nodejsProcessDidEnd:)]) {
						int exitCode = *((int*)args.at(0));
						[delegate nodejsProcessDidEnd:exitCode];
					}
				} break;
				
				case ProcessEventType::EMIT_EVENT: {
					if([delegate respondsToSelector:@selector(nodejsProcessDidEmitEvent:data:env:)]) {
						napi_env env = (napi_env)args.at(0);
						NSString* eventName = [NSString stringWithUTF8String:(const char*)args.at(1)];
						napi_value data = (napi_value)args.at(1);
						[delegate nodejsProcessDidEmitEvent:eventName data:data env:env];
					}
				} break;
			}
		}
	}
}

#endif
