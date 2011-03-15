//
//  CVOCV_IOSViewController.h
//  CVOCV_IOS
//
//  Created by buza on 3/14/11.
//  Copyright 2011 BuzaMoto. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>

#import <UIKit/UIKit.h>

#if defined __cplusplus
class VideoProcessor;
#else
typedef struct VideoProcessor VideoProcessor;
#endif

@interface CVOCV_IOSViewController : UIViewController <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    CGColorSpaceRef mCS;
    CALayer *mLayer;
    VideoProcessor *mVideoProcessor;
    AVCaptureSession *mCaptureSession;
}

@end

