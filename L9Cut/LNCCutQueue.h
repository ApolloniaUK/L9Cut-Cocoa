//
//  LNCCutQueue.h
//  L9Cut
//
//  Created by Alan Staniforth on 30/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "LNCAppDelegate.h"


@interface LNCCutQueue : NSObject

@property (assign) NSMutableArray *filesToCut;
@property (assign) LNCAppDelegate *appDelegate;
@property (nonatomic,getter = isBusy) BOOL busy;

- (id)initWithNotifyTarget:(LNCAppDelegate *)cutFinishedTarget;
- (void)addFileToQueue:(NSString *)inFilePath;
- (void)run;
- (void)curTaskDone:(NSTimer *)timer;
@end
