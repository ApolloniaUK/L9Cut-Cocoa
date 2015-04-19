//
//  LNCFileListItem.h
//  L9Cut
//
//  Created by Alan Staniforth on 31/03/2015.
//  Copyright (c) 2015 Erehwon, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


typedef enum {
	LNCCutStatusWaiting = 1,
	LNCCutStatusProcessing,
	LNCCutStatusSuceeded,
	LNCCutStatusFailed
} LNCCutItemStatus;

@interface LNCFileListItem : NSObject {
	NSString *srcFilePath;
	NSString *dstFilePath;
	NSString *outText;
	LNCCutItemStatus status;
}

// Accessor methods
- (NSString *)srcFilePath;
- (void)setSrcFilePath:(NSString *)aSrcFilePath;
- (NSString *)dstFilePath;
- (void)setDstFilePath:(NSString *)aDstFilePath;
- (NSString *)outText;
- (void)setOutText:(NSString *)anOutText;
- (LNCCutItemStatus)status;
- (void)setStatus:(LNCCutItemStatus)aStatus;

@end
