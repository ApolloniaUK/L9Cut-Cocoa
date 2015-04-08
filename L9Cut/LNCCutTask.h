//
//  LNCCutTask.h
//  L9Cut
//
//  Created by Alan Staniforth on 18/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface LNCCutTask : NSObject

// task support properties
@property (retain) NSArray *taskArgs;
@property (retain) id notifyOnExit;
@property (assign) NSMutableData *outData;
@property (assign) SEL threadExitSelector;
@property (assign) NSInteger exitStatus;
@property (nonatomic, getter = isRunning) BOOL running;

@property (nonatomic, strong) __block NSTask *cutTask;
@property (nonatomic, strong) NSPipe *outputPipe;
@property (nonatomic, strong) NSPipe *inPipe;

// creation and destruction
-(id)initWithArgs:(NSArray *)args notifyOnExit:(id)target threadExitSelector:(SEL)selector;

// cut task methods
- (void)runTask:(id)sender;


@end
