//
//  ViewController.mm
//  NodeJSEmbedTest-macOS
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#import "ViewController.h"
#include <embed/nodejs/NodeJS.hpp>

@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];

	// Do any additional setup after loading the view.
	embed::nodejs::addProcessEventDelegate(self);
	embed::nodejs::start();
}

-(void)viewDidAppear {
	[super viewDidAppear];
	
	embed::nodejs::queueMain([](napi_env env) {
		printf("we're on the NodeJS thread\n");
	});
}

-(void)setRepresentedObject:(id)representedObject {
	[super setRepresentedObject:representedObject];

	// Update the view, if already loaded.
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
