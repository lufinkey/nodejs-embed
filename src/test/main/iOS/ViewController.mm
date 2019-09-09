//
//  ViewController.mm
//  NodeJSEmbedTest-iOS
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#import "ViewController.h"
#include <NodeJSEmbed/NodeJSEmbed.hpp>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
	[super viewDidLoad];
	// Do any additional setup after loading the view.
}

-(void)viewDidAppear:(BOOL)animated {
	[super viewDidAppear:animated];
	
	embed::nodejs::start();
}

@end
