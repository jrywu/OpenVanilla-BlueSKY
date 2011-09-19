//
//  OVIMArrayController.m
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

#import "OVIMArrayController.h"


@implementation OVIMArrayController

- (void)setDictinary:(NSDictionary *)dictionary
{
	[super setDictinary:dictionary];
	if (![[self dictionary] objectForKey:[NSString stringWithUTF8String:"快打模式"]]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:[NSString stringWithUTF8String:"快打模式"]];
	}
	[_quickModeCheckBox setIntValue:[[[self dictionary] valueForKey:[NSString stringWithUTF8String:"快打模式"]] intValue]];

	if (![[self dictionary] objectForKey:[NSString stringWithUTF8String:"特別碼提示"]]) {
		[self setValue:[NSNumber numberWithInt:0] forKey:[NSString stringWithUTF8String:"特別碼提示"]];
	}	
	[_specialCodeCheckBox setIntValue:[[[self dictionary] valueForKey:[NSString stringWithUTF8String:"特別碼提示"]] intValue]];
}

- (IBAction)setQuickModeAction:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[sender intValue]] forKey:[NSString stringWithUTF8String:"快打模式"]];
	[self writeSetting];
}
- (IBAction)setSpecialCodeAction:(id)sender
{
	[self setValue:[NSNumber numberWithInt:[sender intValue]] forKey:[NSString stringWithUTF8String:"特別碼提示"]];
	[self writeSetting];
}


@end
