//
//  LNCCutQueue.m
//  L9Cut
//
//  Created by Alan Staniforth on 30/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import "LNCCutQueue.h"

@implementation LNCCutQueue

- (id)init
{
	return [self initWithNotifyTarget:nil];
}

- (id)initWithNotifyTarget:(LNCAppDelegate *)cutFinishedTarget
{
	self = [super init];
	if (nil != self) {
		_appDelegate = cutFinishedTarget;
		_filesToCut = [[NSMutableArray alloc] init];
		_busy = NO;
	}
	return self;
}

- (void)dealloc
{
	[_filesToCut release];
	[super dealloc];
}

- (void)addFileToQueue:(NSString *)inFilePath
{
	[_filesToCut addObject:inFilePath];
}

- (void)run
{
	if (0 != [_filesToCut count]) {
		_busy = YES;
		NSString *filePath = [_filesToCut objectAtIndex:0];
		NSString *gameFilePath = [filePath stringByAppendingString:@".l9game"];
		NSArray *args = [NSArray arrayWithObjects:filePath, gameFilePath, nil];
		[_filesToCut removeObjectAtIndex:0];
		LNCCutTask *newCut = [[LNCCutTask alloc] initWithArgs:args notifyOnExit:self threadExitSelector:@selector(curTaskDone:)];
		if (nil != newCut) {
			[newCut runTask:self];
		}
	}
}

-(void)curTaskDone:(NSTimer *)timer
{
	// Called by timer scheduled by an LNCCutTask object as its conversion thread
	// exits. Need to check (from the LNCCutTask ivar 'isRunning' that the conversion
	// thread has finished before releasing the task.
	LNCCutTask *curTask = [timer userInfo];
	if (![curTask isRunning]) {
		// kill the timer now the NSTask doing the cut has exited.
		[timer invalidate];
		// pass the task to the app delegate to handle output/display updating &c
		[_appDelegate cutTaskDone:curTask];
		
		// the thread has exited so safe to release the cut task
		[curTask release];
		// if no more files to cut, clear busy flag
		if ([_filesToCut count] == 0) {
			_busy = NO;
		} else {
			NSString *filePath = [_filesToCut objectAtIndex:0];
			NSString *gameFilePath = [filePath stringByAppendingString:@".l9game"];
			NSArray *args = [NSArray arrayWithObjects:filePath, gameFilePath, nil];
			[_filesToCut removeObjectAtIndex:0];
			LNCCutTask *newCut = [[LNCCutTask alloc] initWithArgs:args notifyOnExit:self threadExitSelector:@selector(curTaskDone:)];
			if (nil != newCut) {
				[newCut runTask:self];
			}
		}
	}
}

@end
