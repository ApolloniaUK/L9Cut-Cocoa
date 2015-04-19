//
//  LNCFileListItem.m
//  L9Cut
//
//  Created by Alan Staniforth on 31/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import "LNCFileListItem.h"

@implementation LNCFileListItem

#pragma mark Creation/destruction methods

-(void)dealloc
{
	[srcFilePath release];
	[dstFilePath release];
	[outText release];
	
	[super dealloc];
}

#pragma mark Accessor Methods

- (NSString *)srcFilePath
{
    return [[srcFilePath retain] autorelease];
}
- (void)setSrcFilePath:(NSString *)aSrcFilePath
{
    if (srcFilePath != aSrcFilePath) {
        [srcFilePath release];
        srcFilePath = [aSrcFilePath retain];
    }
}

- (NSString *)dstFilePath
{
    return [[dstFilePath retain] autorelease];
}
- (void)setDstFilePath:(NSString *)aDstFilePath
{
    if (dstFilePath != aDstFilePath) {
        [dstFilePath release];
        dstFilePath = [aDstFilePath retain];
    }
}

- (NSString *)outText
{
    return [[outText retain] autorelease];
}
- (void)setOutText:(NSString *)anOutText
{
    if (outText != anOutText) {
        [outText release];
        outText = [anOutText retain];
    }
}

- (LNCCutItemStatus)status
{
    return status;
}
- (void)setStatus:(LNCCutItemStatus)aStatus
{
    status = aStatus;
}


@end
