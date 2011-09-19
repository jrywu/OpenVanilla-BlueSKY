//
// LVTextView.h
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

#import "LVTooltipTextView.h"


@implementation LVTooltipTextView

- (void)setSimpleText:(NSString *)text
{
	NSDictionary *attributes = [NSDictionary dictionaryWithObjectsAndKeys:[NSColor whiteColor], NSForegroundColorAttributeName, [NSFont systemFontOfSize:16.0], NSFontAttributeName, nil];
	[[_attributeString mutableString] setString:text];
	[_attributeString setAttributes:attributes range:NSMakeRange(0, [text length])];	
}
- (NSRect)boundingRectForText
{
	NSRect result = [_attributeString boundingRectWithSize:NSMakeSize(2000.0, 2000.0) options:NSStringDrawingUsesLineFragmentOrigin];
	result.size.width += 10;
	result.size.height += 10;
	return result;
}
- (void)drawRect:(NSRect)aRect
{   
    [[NSColor colorWithCalibratedHue:0.63 saturation:0.95 brightness:0.60 alpha:0.90] setFill];

	//[[NSColor colorWithCalibratedHue:0.16 saturation:0.22 brightness:0.97 alpha:0.9] setFill];
	[[NSColor grayColor] setStroke];
#if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_5
	NSBezierPath *path = [NSBezierPath bezierPathWithRoundedRect:[self bounds] xRadius:5 yRadius:5];
#else
	NSBezierPath *path = [NSBezierPath bezierPathWithRect:[self bounds]];
#endif

	[path fill];
	[path stroke];
	[_attributeString drawAtPoint:NSMakePoint(5.0, 5.0)];
}

@end
