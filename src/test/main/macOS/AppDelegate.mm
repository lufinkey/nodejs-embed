//
//  AppDelegate.mm
//  NodeJSEmbedTest-macOS
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#import "AppDelegate.h"
#include <embed/nodejs/NodeJS.hpp>

@interface AppDelegate()<NodeJSProcessEventDelegate>

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
	// Insert code here to initialize your application
	embed::nodejs::addProcessEventDelegate(self);
	embed::nodejs::start();
	embed::nodejs::queueMain([](napi_env env) {
		printf("we're on the NodeJS thread\n");
	});
}

- (void)applicationWillTerminate:(NSNotification*)aNotification {
	// Insert code here to tear down your application
}

-(void)nodejsProcessWillStart:(NSArray<NSString*>*)args {
	NSLog(@"nodejsProcessWillStart: %@", args);
}

-(void)nodejsProcessDidStart:(napi_env)env {
	NSLog(@"nodejsProcessDidStart");
}

-(void)nodejsProcessWillEnd:(napi_env)env {
	NSLog(@"nodejsProcessWillEnd");
}

-(void)nodejsProcessDidEnd:(int)exitCode {
	NSLog(@"nodejsProcessDidEnd %i", exitCode);
}

@end
