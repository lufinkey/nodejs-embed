//
//  NodeJSProcessEventDelegate.h
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/10/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#include "NAPI_Types.hpp"

@protocol NodeJSProcessEventDelegate <NSObject>

@optional
-(void)nodejsProcessWillStart:(NSArray<NSString*>*)args;
@optional
-(void)nodejsProcessDidStart:(napi_env)env;
@optional
-(void)nodejsProcessWillEnd:(napi_env)env;
@optional
-(void)nodejsProcessDidEnd:(int)exitCode;

@optional
-(void)nodejsProcessDidEmitEvent:(NSString*)eventName data:(napi_value)data env:(napi_env)env;

@end
#endif
