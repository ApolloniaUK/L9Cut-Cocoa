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


// 'Empty' category to declare "private' methods
@interface LNCCutTask ()
- (LNCFileListItem *)findListItemMatchingString:(NSString *)inString;
@end


@implementation LNCAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// Insert code here to initialize your application
}

-(IBAction)runCutTask:(id)sender
{
	if (_cutQueue == nil) {
		_cutQueue = [[LNCCutQueue alloc] initWithNotifyTarget:self];
	}
	if (nil == _fileList) {
		_fileList = [[NSMutableArray alloc] init];
	}
	// create first test item and add to run queue
	NSMutableString *gameFile = [[NSMutableString alloc] initWithString:@"/Users/alan/Documents/Development/MyProjects/L9Cut/L9test Folder/SNOW_P.V2"];
	LNCFileListItem *newItem = [[LNCFileListItem alloc] init];
	[newItem setSrcFilePath:gameFile];
	ASDLog(@"gameFile retain count: %lu.", [gameFile retainCount]);
	[gameFile autorelease];
	ASFlLog(@"gameFile retain count: %lu.", [gameFile retainCount]);
	[newItem setStatus:LNCCutStatusWaiting];
	[_fileList addObject:newItem];
	[_cutQueue addFileToQueue:gameFile];
	// create second test item and add to run queue
	gameFile = [[NSMutableString alloc] initWithString:@"/Users/alan/Documents/Development/MyProjects/L9Cut/L9test Folder/WORM.Z80" ];
	newItem = [[LNCFileListItem alloc] init];
	[newItem setSrcFilePath:gameFile];
	[gameFile autorelease];
	[newItem setStatus:LNCCutStatusWaiting];
	[_fileList addObject:newItem];
	[_cutQueue addFileToQueue:gameFile];
	// start the queue processing
	[_cutQueue run];
}

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
	self.outputText.string = [self.outputText.string stringByAppendingString:[NSString stringWithFormat:@"%@", outStr]];
	// Scroll to end of outputText field
	NSRange range;
	range = NSMakeRange([self.outputText.string length], 0);
	[self.outputText scrollRangeToVisible:range];
}

- (LNCFileListItem *)findListItemMatchingString:(NSString *)inString
{
	LNCFileListItem *retVal = nil;
	for (LNCFileListItem *curItem in _fileList) {
		NSString *tmpListStr = [curItem srcFilePath];
		if (tmpListStr == inString) {
			retVal = curItem;
		}
	}
	return retVal;
}

@end
