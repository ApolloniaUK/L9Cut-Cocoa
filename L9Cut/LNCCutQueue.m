//
//  LNCCutQueue.m
//  L9Cut
//
//  Created by Alan Staniforth on 30/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import "LNCCutQueue.h"

@implementation LNCCutQueue

#pragma mark Creation/Destruction methods

- (id)init
{
	return [self initWithNotifyTarget:nil];
}

- (id)initWithNotifyTarget:(LNCAppDelegate *)cutFinishedTarget
{
	self = [super init];
	if (nil != self) {
		// tempting though it is, do not retain the appDelegate, doing so will create a circular
		// retain as the app delegate retains the file queue
		appDelegate = cutFinishedTarget;
		filesToCut = [[NSMutableArray alloc] init];
		busy = NO;
	}
	return self;
}

- (void)dealloc
{
	[filesToCut release];
	[appDelegate release];
	[curTask release];
	[super dealloc];
}

#pragma mark Accessor methods

- (BOOL)isBusy
{
    return busy;
}
- (void)setBusy:(BOOL)flag
{
    busy = flag;
}

- (LNCCutTask *)curTask
{
    return [[curTask retain] autorelease];
}
- (void)setCurTask:(LNCCutTask *)aCurTask
{
    if (curTask != aCurTask) {
        [curTask release];
        curTask = [aCurTask retain];
    }
}

#pragma mark Operational methods

- (void)addFileToQueue:(NSString *)inFilePath
{
	[filesToCut addObject:inFilePath];
}

- (void)startNextTask
{
	NSString *filePath = [filesToCut objectAtIndex:0];
	NSString *gameFilePath = [filePath stringByAppendingString:@".l9game"];
	NSArray *args = [NSArray arrayWithObjects:filePath, gameFilePath, nil];
	[filesToCut removeObjectAtIndex:0];
	[self setCurTask:[[[LNCCutTask alloc] initWithArgs:args notifyOnExit:self threadExitSelector:@selector(curTaskDone:)] autorelease]];
	if (nil != curTask) {
		[curTask runTask:self];
	}
}

- (void)run
{
	if (0 != [filesToCut count]) {
		busy = YES;
		[self startNextTask];
	}
}

-(void)curTaskDone:(NSTimer *)timer
{
	// Called by timer scheduled by an LNCCutTask object as its conversion thread
	// exits. Need to check (from the LNCCutTask ivar 'isRunning' that the conversion
	// thread has finished before releasing the task.
	if (![curTask isRunning]) {
		// kill the timer now the NSTask doing the cut has exited.
		[timer invalidate];
		// pass the task to the app delegate to handle output/display updating &c
		[appDelegate cutTaskDone:curTask];
		
		// More files to cut?
		if ([filesToCut count] == 0) {
			// No, clear busy flag
			busy = NO;
			// release the last cut task
			[self setCurTask:nil];
		} else {
			// Yes, start the next one running
			[self startNextTask];
		}
	}
}

@end
