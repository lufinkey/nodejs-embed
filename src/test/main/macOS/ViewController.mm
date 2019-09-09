//
//  ViewController.mm
//  NodeJSEmbedTest-macOS
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#import "ViewController.h"
#include <NodeJSEmbed/NodeJSEmbed.hpp>

@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];

	// Do any additional setup after loading the view.
}

-(void)viewDidAppear {
	[super viewDidAppear];
	
	embed::nodejs::NodeJS::start();
}

- (void)setRepresentedObject:(id)representedObject {
	[super setRepresentedObject:representedObject];

	// Update the view, if already loaded.
}


@end
