//
//  AppDelegate.mm
//  NodeJSEmbedTest-iOS
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#import "AppDelegate.h"
#include <embed/nodejs/NodeJS.hpp>
#include <napi.h>

@interface AppDelegate()<NodeJSProcessEventDelegate>

@end

@implementation AppDelegate

void testNodeJS() {
	embed::nodejs::start();
	NSString* modulePath = [NSBundle.mainBundle pathForResource:@"testmodule" ofType:@"js"];
	NSError* error = nil;
	NSString* moduleContent = [[NSString alloc] initWithContentsOfFile:modulePath encoding:NSUTF8StringEncoding error:&error];
	if(error != nil) {
		NSLog(@"Error getting module content from path: %@", error);
		return;
	}
	embed::nodejs::queueMain([=](napi_env env) {
		embed::nodejs::loadModuleFromMemory(env, "[eval]/test", moduleContent.UTF8String);
	});
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	// Override point for customization after application launch.
	embed::nodejs::addProcessEventDelegate(self);
	testNodeJS();
	return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and invalidate graphics rendering callbacks. Games should use this method to pause the game.
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
	// Called as part of the transition from the background to the active state; here you can undo many of the changes made on entering the background.
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}


- (void)applicationWillTerminate:(UIApplication *)application {
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
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
