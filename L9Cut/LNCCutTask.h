//
//  LNCCutTask.h
//  L9Cut
//
//  Created by Alan Staniforth on 18/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LNCCutTask : NSObject {
	// task support iVars
	NSArray *taskArgs;
	id notifyOnExit;
	NSMutableData *outData;
	SEL threadExitSelector;
	NSInteger exitStatus;
	BOOL running;
	NSTask *cutTask;
	NSPipe *outputPipe;
	NSPipe *inPipe;
}

// creation and destruction
-(id)initWithArgs:(NSArray *)args notifyOnExit:(id)target threadExitSelector:(SEL)selector;

// Accessor methods
- (NSArray *)taskArgs;
- (void)setTaskArgs:(NSArray *)aTaskArgs;
- (id)notifyOnExit;
- (void)setNotifyOnExit:(id)aNotifyOnExit;
- (NSMutableData *)outData;
- (void)setOutData:(NSMutableData *)anOutData;
- (SEL)threadExitSelector;
- (void)setThreadExitSelector:(SEL)aThreadExitSelector;
- (NSInteger)exitStatus;
- (void)setExitStatus:(NSInteger)anExitStatus;
- (BOOL)isRunning;
- (void)setRunning:(BOOL)flag;
- (NSTask *)cutTask;
- (void)setCutTask:(NSTask *)aCutTask;
- (NSPipe *)outputPipe;
- (void)setOutputPipe:(NSPipe *)anOutputPipe;
- (NSPipe *)inPipe;
- (void)setInPipe:(NSPipe *)anInPipe;

// cut task methods
- (void)runTask:(id)sender;


@end
