//
//  NodeJS_objc.mm
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/10/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#ifdef __OBJC__

#include "NodeJS.hpp"
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
		switch(eventType) {
			case ProcessEventType::WILL_START: {
				NSMutableArray<NSString*>* procArgs = [NSMutableArray new];
				int argc = *((int*)args.at(0));
				char** argv = (char**)args.at(1);
				for(int i=0; i<argc; i++) {
					[procArgs addObject:[NSString stringWithUTF8String:argv[i]]];
				}
				for(id<NodeJSProcessEventDelegate> delegate in delegates) {
					if([delegate respondsToSelector:@selector(nodejsProcessWillStart:)]) {
						[delegate nodejsProcessWillStart:procArgs];
					}
				}
			} break;

			case ProcessEventType::DID_START: {
				napi_env env = (napi_env)args.at(0);
				for(id<NodeJSProcessEventDelegate> delegate in delegates) {
					if([delegate respondsToSelector:@selector(nodejsProcessDidStart:)]) {
						[delegate nodejsProcessDidStart:env];
					}
				}
			} break;

			case ProcessEventType::WILL_END: {
				napi_env env = (napi_env)args.at(0);
				for(id<NodeJSProcessEventDelegate> delegate in delegates) {
					if([delegate respondsToSelector:@selector(nodejsProcessWillEnd:)]) {
						[delegate nodejsProcessWillEnd:env];
					}
				}
			} break;

			case ProcessEventType::DID_END: {
				int exitCode = *((int*)args.at(0));
				for(id<NodeJSProcessEventDelegate> delegate in delegates) {
					if([delegate respondsToSelector:@selector(nodejsProcessDidEnd:)]) {
						[delegate nodejsProcessDidEnd:exitCode];
					}
				}
			} break;

			case ProcessEventType::EMIT_EVENT: {
				napi_env env = (napi_env)args.at(0);
				NSString* eventName = [NSString stringWithUTF8String:(const char*)args.at(1)];
				napi_value data = (napi_value)args.at(2);
				for(id<NodeJSProcessEventDelegate> delegate in delegates) {
					if([delegate respondsToSelector:@selector(nodejsProcessDidEmitEvent:data:env:)]) {
						[delegate nodejsProcessDidEmitEvent:eventName data:data env:env];
					}
				}
			} break;
		}
	}
}

#endif
