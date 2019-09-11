//
//  ViewController.h
//  NodeJSEmbedTest-macOS
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <embed/nodejs/NodeJS.hpp>

@interface ViewController: NSViewController<NodeJSProcessEventDelegate>

@end

