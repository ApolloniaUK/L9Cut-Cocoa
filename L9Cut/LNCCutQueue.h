//
//  LNCCutQueue.h
//  L9Cut
//
//  Created by Alan Staniforth on 30/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "LNCAppDelegate.h"
#import "LNCCutTask.h"


@interface LNCCutQueue : NSObject {
	NSMutableArray *filesToCut;
	LNCAppDelegate *appDelegate;
	LNCCutTask *curTask;
	BOOL busy;
}

// Creation methods
- (id)initWithNotifyTarget:(LNCAppDelegate *)cutFinishedTarget;

// Accessor methods
- (BOOL)isBusy;
- (void)setBusy:(BOOL)flag;
- (LNCCutTask *)curTask;
- (void)setCurTask:(LNCCutTask *)aCurTask;

// Operational methods
- (void)addFileToQueue:(NSString *)inFilePath;
- (void)run;
- (void)curTaskDone:(NSTimer *)timer;
@end
