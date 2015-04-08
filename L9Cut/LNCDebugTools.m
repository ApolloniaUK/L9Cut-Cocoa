//
//  LNCDebugTools.m
//  L9Cut
//
//  Created by Alan Staniforth on 31/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import "LNCDebugTools.h"

// From Mark Dalrymple @ Big Nerd Ranch
// <https://www.bignerdranch.com/blog/a-quieter-log/>
void QuietLog (NSString *format, ...) {
    va_list argList;
    va_start (argList, format);
	
    NSString *message = [[NSString alloc] initWithFormat: format
											   arguments: argList];
	
	// no ARC in this build so...
	[message autorelease];
	
    va_end (argList);
	
    fprintf (stderr, "%s\n", [message UTF8String]);
	
} // QuietLog

