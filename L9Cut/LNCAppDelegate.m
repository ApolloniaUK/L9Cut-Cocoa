//
//  LNCAppDelegate.m
//  L9Cut
//
//  Created by Alan Staniforth on 17/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import "LNCAppDelegate.h"
#import "LNCCutQueue.h"
#import "LNCFileListItem.h"
#import "ASLog.h"


// 'Empty' category to declare "private" methods
@interface LNCAppDelegate ()
- (LNCFileListItem *)findListItemMatchingString:(NSString *)inString;
@end


@implementation LNCAppDelegate

#pragma mark App delegate methods

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// Insert code here to initialize your application
}

#pragma mark Target methods
-(IBAction)runCutTask:(id)sender
{
	if (cutQueue == nil) {
		cutQueue = [[LNCCutQueue alloc] initWithNotifyTarget:self];
	}
	if (nil == fileList) {
		fileList = [[NSMutableArray alloc] init];
	}
	// create first test item and add to run queue
	NSMutableString *gameFile = [[[NSMutableString alloc] initWithString:@"/Users/alan/Documents/Development/MyProjects/L9Cut Cocoa/(L9test Folder)/SNOW_P.V2"] autorelease];
	LNCFileListItem *newItem = [[LNCFileListItem alloc] init];
	[newItem setSrcFilePath:gameFile];
	[newItem setStatus:LNCCutStatusWaiting];
	[fileList addObject:[newItem autorelease]];
	[cutQueue addFileToQueue:gameFile];
	// create second test item and add to run queue
	gameFile = [[NSMutableString alloc] initWithString:@"/Users/alan/Documents/Development/MyProjects/L9Cut Cocoa/(L9test Folder)/WORM.Z80" ];
	newItem = [[LNCFileListItem alloc] init];
	[newItem setSrcFilePath:gameFile];
	[gameFile release];
	[newItem setStatus:LNCCutStatusWaiting];
	[fileList addObject:[newItem autorelease]];
	[cutQueue addFileToQueue:gameFile];
	// start the queue processing
	[cutQueue run];
}

#pragma mark Accessors

- (NSTextView *)outputText
{
    return [[outputText retain] autorelease];
}
- (void)setOutputText:(NSTextView *)anOutputText
{
    if (outputText != anOutputText) {
        [outputText release];
        outputText = [anOutputText retain];
    }
}

#pragma mark Operational methods

-(void)cutTaskDone:(LNCCutTask *)doneTask
{
	// find the matching file list item and update it with output file and outcome status
	NSString *tmpStr = [[doneTask taskArgs] objectAtIndex:0];
	LNCFileListItem *listItem = [self findListItemMatchingString:tmpStr];
	if ([doneTask exitStatus] == 0) {
		tmpStr = [[doneTask taskArgs] objectAtIndex:1];
		[listItem setDstFilePath:tmpStr];
		[listItem setStatus:LNCCutStatusSuceeded];
	} else {
		[listItem setStatus:LNCCutStatusFailed];
	}
	// get the output text
	NSString *printStr = [[NSString alloc] initWithData:[doneTask outData] encoding:NSUTF8StringEncoding];
	// add it to the file list item
	[listItem setOutText:printStr];
	
	// print it to our 'terminal'; this will end up va
	[self showTaskTextOutput:printStr];
	[printStr release];
}

- (void)showTaskTextOutput:(id)outStr
{
	[[self outputText] setString:[[[self outputText] string] stringByAppendingString:[NSString stringWithFormat:@"%@",outStr]]];
	// Scroll to end of outputText field
	NSRange range;
	range = NSMakeRange([[[self outputText] string] length], 0);
	[[self outputText] scrollRangeToVisible:range];
}

- (LNCFileListItem *)findListItemMatchingString:(NSString *)inString
{
	LNCFileListItem *retVal = nil;
	for (LNCFileListItem *curItem in fileList) {
		NSString *tmpListStr = [curItem srcFilePath];
		if (tmpListStr == inString) {
			retVal = curItem;
		}
	}
	return retVal;
}

@end
