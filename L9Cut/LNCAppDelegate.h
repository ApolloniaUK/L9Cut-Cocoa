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

@interface LNCAppDelegate : NSObject <NSApplicationDelegate> {
	IBOutlet NSButton *runButton;
	IBOutlet NSWindow *window;
	LNCCutQueue *cutQueue;
	NSMutableArray *fileList;
	BOOL taskIsRunning;
	IBOutlet NSTextView *outputText;
}

// Target methods
-(IBAction)runCutTask:(id)sender;

// Accessor methods
- (NSTextView *)outputText;
- (void)setOutputText:(NSTextView *)anOutputText;

// Operational methods
-(void)cutTaskDone:(LNCCutTask *)doneTask;
-(void)showTaskTextOutput:(id)outStr;

@end
