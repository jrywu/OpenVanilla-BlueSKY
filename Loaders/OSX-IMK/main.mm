//
// main.mm
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#import <Cocoa/Cocoa.h>
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
    #import <InputMethodKit/InputMethodKit.h>
#else
    #import "InputMethodKitTiger.h"
#endif
#import "LVConfig.h"
#import "LVModuleManager.h"

int main(int argc, char *argv[])
{
    NSAutoreleasePool *pool = [NSAutoreleasePool new];	
	
	NSMutableArray *moduleLoadPaths = [NSMutableArray array];
	NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
	NSString *defaultModulesPath = [resourcePath stringByAppendingPathComponent:@"Modules"];	
	[moduleLoadPaths addObject:defaultModulesPath];
	
	do {
		NSArray *dirs = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
		
		NSString *userOVPath = [[dirs objectAtIndex:0] stringByAppendingPathComponent:OPENVANILLA_NAME];
		BOOL isDir = YES;
		if (![[NSFileManager defaultManager] fileExistsAtPath:userOVPath isDirectory:&isDir]) {
			[[NSFileManager defaultManager] createDirectoryAtPath:userOVPath withIntermediateDirectories:YES attributes:nil error:nil];
		}		
		
		isDir = YES;
		NSString *userModulePath = [userOVPath stringByAppendingPathComponent:@"Modules"];
		if (![[NSFileManager defaultManager] fileExistsAtPath:userModulePath isDirectory:&isDir]) {
			[[NSFileManager defaultManager] createDirectoryAtPath:userModulePath withIntermediateDirectories:YES attributes:nil error:nil];
		}		
		
		if (isDir) {		
			[moduleLoadPaths addObject:userModulePath];
		}
	} while(0);

	[moduleLoadPaths addObject:@"/Library/OpenVanilla/0.9-Old/Modules"];
	
	[[LVModuleManager sharedManager] setModulePackageBundlePaths:moduleLoadPaths];	
	[[LVModuleManager sharedManager] loadModulePackageBundles];
	
	IMKServer *inputMethodServer = [[IMKServer alloc] initWithName:OPENVANILLA_CONNECTION_NAME bundleIdentifier:[[NSBundle mainBundle] bundleIdentifier]];
	
    if (!inputMethodServer) {
		NSLog(@"OpenVanilla Loader initialization failed!");
        return 1;
    }
	
	[NSBundle loadNibNamed:@"MainMenu" owner:[NSApplication sharedApplication]];
	[[NSApplication sharedApplication] run];
	[pool drain];
	return 0;
}
