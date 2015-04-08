//
//  LNCAppDelegate.h
//  L9Cut
//
//  Created by Alan Staniforth on 17/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "LNCCutTask.h"

@class LNCCutQueue;

@interface LNCAppDelegate : NSObject <NSApplicationDelegate>
@property (assign) IBOutlet NSButton *runButton;
@property (assign) IBOutlet NSWindow *window;
@property (assign) LNCCutQueue *cutQueue;
@property (assign) NSMutableArray *fileList;
@property (nonatomic) BOOL taskIsRunning;
@property (unsafe_unretained) IBOutlet NSTextView *outputText;

-(IBAction)runCutTask:(id)sender;
-(void)cutTaskDone:(LNCCutTask *)doneTask;
-(void)showTaskTextOutput:(id)outStr;

@end
