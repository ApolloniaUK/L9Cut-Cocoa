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

@interface LNCFileListItem : NSObject

@property (retain) NSString *srcFilePath;
@property (retain) NSString *dstFilePath;
@property (retain) NSString *outText;
@property (assign) LNCCutItemStatus status;

@end
