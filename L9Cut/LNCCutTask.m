//
//  LNCCutTask.m
//  L9Cut
//
//  Created by Alan Staniforth on 18/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import "LNCCutTask.h"


// 'Empty' category to declare "private' methods
@interface LNCCutTask ()
- (void)doCut:(id)arguments;
- (void)handleTaskOutputNotification:(NSNotification *)notification;
- (void)readDataForNotification:(NSNotification *)notification toFileEnd:(BOOL)finish;
@end


@implementation LNCCutTask

#pragma mark Objject creation and destruction

- (id)init
{
	return [self initWithArgs:nil notifyOnExit:nil threadExitSelector:nil];
}

-(id)initWithArgs:(NSArray *)args notifyOnExit:(id)target threadExitSelector:(SEL)selector
{
	self = [super init];
	if (nil != self) {
		self.taskArgs = args;
		// retain the object we must notify of thread exit so it can't disappear from under us
		self.notifyOnExit = target;
		self.threadExitSelector = selector;
		// initialize the data object to collect the tool output
		self.outData = [[NSMutableData alloc] initWithLength:0];
		
		self.running = NO;
	}
	return self;
}

- (void)dealloc
{
	// release the object we must notify of thread exit
	[self.notifyOnExit release];
	[self.taskArgs release];
	// for some weird reason, despite getting this with a
	[self.outData release];
	
	[super dealloc];
}

#pragma mark Cut task methods

- (void)runTask:(id)sender
{
	[NSThread detachNewThreadSelector:@selector(doCut:) toTarget:self withObject:self.taskArgs];
}

- (void)doCut:(id)arguments
{
	self.running = YES;
	NSAutoreleasePool *localPool = [NSAutoreleasePool new];
	NSString *toolPath = [NSString stringWithFormat:@"%@", [[NSBundle mainBundle] pathForResource:@"l9cut" ofType:nil]];
	
	@try {
		_cutTask = [[NSTask alloc] init];
		[_cutTask setLaunchPath:toolPath];
		[_cutTask setArguments:arguments];
		
		// Output Handling
		//1
		//_outputPipe = [[NSPipe alloc] init];
		_outputPipe = [NSPipe new];
		[_cutTask setStandardError:_outputPipe];
		// [[_outputPipe fileHandleForReading] release];
		// _cutTask has taken ownership of outputPipe so release it
		// (thanks to Apple's Quartz Composer CommandLineTool)
		[_outputPipe release];
		_inPipe = [NSPipe new];
		[_cutTask setStandardInput:_inPipe];
		[_inPipe release];
		
		//2
		[[_outputPipe fileHandleForReading] waitForDataInBackgroundAndNotify];
		
		//3
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(handleTaskOutputNotification:) name:NSFileHandleDataAvailableNotification object:nil];
		
		//3
		[_cutTask launch];
		
		//4
		[_cutTask waitUntilExit];
		//[NSThread sleepForTimeInterval:2]; // THIS LINE FOR TESTING
	}
	//4
	@catch (NSException *exception) {
		NSLog(@"Problem Running Task: %@", [exception description]);
	}
	//5
	@finally {
		[[NSNotificationCenter defaultCenter] removeObserver:self name:NSFileHandleDataAvailableNotification object:nil];
		/* 
		 The task can terminate with data left in an output pipe so need to do a final
		 read to get that data - hint from Apple's Quartz Composer CommandLineTool
		 example project. Must do this before releasing the task or the pipe will be 
		 released as well and the data lost. So call our text output handler.
		 */
		[self readDataForNotification:nil toFileEnd:YES];
		// probably don't need to close these file handles but Apple's example code does
		[[_outputPipe fileHandleForReading] closeFile];
		[[_outputPipe fileHandleForWriting] closeFile];
		[[_inPipe fileHandleForReading] closeFile];
		[[_inPipe fileHandleForWriting] closeFile];
		/*
		 need to release the file handle directly because when (earlier) it is sent the
		 waitForDataInBackgroundAndNotify: message its retain count increases by one and
		 when the thread exits we see the following leaks:
		 the file handle - created in
		 Leaked Object			#		Responsible Frame
		 CFRunLoopSource		1		-[NSConcreteFileHandle performActivity:modes:]
		 AEListImpl				1		-[NSConcreteFileHandle performActivity:modes:]
		 OS_dispatch_queue		1		-[NSConcreteFileHandle initWithFileDescriptor:closeOnDealloc:]
		 CFBag					1		-[NSConcreteFileHandle performActivity:modes:]
		 NSConcreteFileHandle	1		-[NSConcretePipe init]
		 AEListImpl				1		-[NSConcreteFileHandle performActivity:modes:]
		 */
		[[_outputPipe fileHandleForReading] release];
		_exitStatus = [_cutTask terminationStatus];
		[_cutTask release];
		NSTimer *doneTimer = [NSTimer timerWithTimeInterval:0.1 target:_notifyOnExit selector:_threadExitSelector userInfo:self repeats:YES];
		[[NSRunLoop mainRunLoop] addTimer:doneTimer forMode:NSDefaultRunLoopMode];
		[localPool drain];
		self.running = NO;
	}
}

- (void)handleTaskOutputNotification:(NSNotification *)notification
{
	[self readDataForNotification:notification toFileEnd:NO];
}

- (void)readDataForNotification:(NSNotification *)notification toFileEnd:(BOOL)finish
{
	@try {
		NSFileHandle* fileHandle;
		NSData *output;
		if (finish) {
			output = [[_outputPipe fileHandleForReading] readDataToEndOfFile];
		} else {
			output = [[_outputPipe fileHandleForReading] availableData];
		}
		[self.outData appendData:output];
		NSString *outStr = [[NSString alloc] initWithData:output encoding:NSUTF8StringEncoding];
		//5
		NSArray *lines = [outStr componentsSeparatedByString:@"\n"];
		NSUInteger numLines = [lines count];
		NSString *lastLine = [lines objectAtIndex:(numLines - 1)];
		//"Do you want to continue anyway (y/N)? "
		NSComparisonResult removeProtection = [lastLine compare:@"Do you want the protection removed (y/N)? "];
		NSComparisonResult compressedC64 = [lastLine compare:@"Do you want to continue anyway (y/N)? "];
		if ((NSOrderedSame == removeProtection) || (NSOrderedSame == compressedC64)) {
			NSString *theMsgText;
			NSString *theInformText;
			if (NSOrderedSame == removeProtection) {
				theMsgText = @"Remove copy-protection from this game?";
				theInformText = @"This game has some form of copy protection. Would you like L9Cut can remove it for you?";
			} else if (NSOrderedSame == compressedC64) {
				theMsgText = @"Attempted extraction?";
				theInformText = @"This appears to be a Commodore 64 game file. These are usually compressed and unextractable. Would you like L9Cut to attempt the extraction anyhow?";
			}
			NSAlert *alert = [NSAlert alertWithMessageText:theMsgText defaultButton:nil alternateButton:@"Cancel" otherButton:nil informativeTextWithFormat:@"%@", theInformText];
			NSInteger buttonHit = [alert runModalSheet];
			char theReply = 'y';
			if (buttonHit == NSAlertSecondButtonReturn) {
				theReply = 'n';
			}
			// write to stdin
			fileHandle = [_inPipe fileHandleForWriting];
			if(fileHandle) {
				@try {
					NSData *inData = [NSData dataWithBytes:&theReply length:1];
					[fileHandle writeData:inData];
				}
				@catch (NSException *exception) {
					[_cutTask terminate];
					[_cutTask interrupt];
				}
			}
			char replyChars[2];
			replyChars[0] = theReply; replyChars[1] = '\n';
			[self.outData appendBytes:&replyChars length:2];
			[[_outputPipe fileHandleForReading] waitForDataInBackgroundAndNotify];
		}
		[[_outputPipe fileHandleForReading] waitForDataInBackgroundAndNotify];
		[outStr release];
	}
	@catch (NSException *exception) {
		NSLog(@"Problem getting data: %@", [exception description]);
	}
}

@end
