//
//  MainController+TableView.m
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

#import "MainController+TableView.h"
#import "OVViewController.h"

@implementation MainController(TableView)

- (void)setActiveModuleSettingView:(NSView *)view
{
	NSArray *subViews = [NSArray arrayWithArray:[_moduleContentView subviews]];
	NSEnumerator *enumerator = [subViews objectEnumerator];
	NSView *subView = nil;
	while (subView = [enumerator nextObject]) {
		[subView removeFromSuperview];
	}
	[view setFrame:[_moduleContentView bounds]];
	[_moduleContentView addSubview:view];
       
   		
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	return [_controllersArray count];
}


- (void)tableView:(NSTableView *)aTableView willDisplayCell:(id)aCell forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	[aCell setFont:[NSFont systemFontOfSize:[NSFont systemFontSizeForControlSize:NSRegularControlSize]]];
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	NSString *identifier = [aTableColumn identifier];
    
	OVViewController *controller = [_controllersArray objectAtIndex:rowIndex];
    //NSLog(@"objectValueForTableColum: %@",[controller identifier]);
    
	if ([identifier isEqualToString:@"localizedName"]) {
		return [controller localizedName];
	}else if ([identifier isEqualToString:@"enabled"]) {
        //NSLog(@"objectValueForTableColumn: %@",[controller identifier]);
		NSButtonCell *cell;
        cell = [[NSButtonCell alloc] init];
        [cell setButtonType:NSSwitchButton];
        [cell setState:[controller enabled]];
        [cell setTag:rowIndex];
        [cell setIdentifier:[controller identifier]];
        [cell setTarget:self];
        [aTableColumn setDataCell:cell];
        return cell;
    }
	return nil;
}
- (void)tableView:(NSTableView *)tableView setObjectValue:(id)value forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {          
    NSString *identifier = [aTableColumn identifier];
    OVViewController *controller = [_controllersArray objectAtIndex:rowIndex];
    if ([identifier isEqualToString:@"enabled"]) {
        NSButtonCell *cell = (NSButtonCell *) [tableView selectedCell];
        NSLog(@"setObjectValue: %@",[controller identifier]);
        [controller setEnabled:![controller enabled]];
    }
        
}

- (void)tableViewSelectionDidChange:(NSNotification *)aNotification
{
	NSTableView *tableview = [aNotification object];
	NSInteger index = [tableview selectedRow];
	if (index >= 0 && index < [_controllersArray count]) {
		OVViewController *controller = [_controllersArray objectAtIndex:index];
		NSView *view = [controller view];

		[self setActiveModuleSettingView:view];
		[_titleLabel setStringValue:[controller localizedName]];
        
        NSRect windowFrame = [[self window] frame];
        // Shrink the window width to show only modulelistview when OVViewController (empty) view is applied
        if ([ [controller controllerName] isEqualToString:@"OVViewController"]){
            windowFrame.size.width = [_moduleListTableView frame].size.width +12;
            
        }else
        {
            windowFrame.size.width = [_moduleListTableView frame].size.width 
            + [_moduleContentView frame].size.width ;
        }
        [[self window] setFrame:windowFrame display:YES animate:YES];	

	}
}

@end	
