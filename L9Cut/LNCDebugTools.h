//
//  LNCDebugTools.h
//  L9Cut
//
//  Created by Alan Staniforth on 31/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

// From Mark Dalrymple @ Big Nerd Ranch
// <https://www.bignerdranch.com/blog/a-quieter-log/>
void QuietLog (NSString *format, ...);

// macro to pretty-print QuietLog with function and line info
#define ALog(fmt, ...) NSLog((@"%s [Line %d] " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
