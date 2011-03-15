//
//  CVOCV_IOSViewController.m
//  CVOCV_IOS
//
//  Created by buza on 3/14/11.
//  Copyright 2011 BuzaMoto. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "CVOCV_IOSViewController.h"

#include "Constants.h"

#import "VideoProcessor.h"

@implementation CVOCV_IOSViewController

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad 
{
    mCS = CGColorSpaceCreateDeviceRGB();
    [self performSelector:@selector(startCamera) withObject:nil afterDelay:2];
    [super viewDidLoad];
}

-(void) startCamera
{
    AVCaptureDeviceInput *captureInput;
    
    //Use the front camera.
    //Note: You need to ensure the 'captureSize' in Constants.c reflects
    //which camera you plan to use, as the image sizes differ.
    for (AVCaptureDevice *camera in [AVCaptureDevice devices]) 
    {
        if (camera.position == AVCaptureDevicePositionFront) 
        {
            captureInput = [AVCaptureDeviceInput deviceInputWithDevice:camera error:nil];
            break;
        }
    }
    
    
    /*
    //Use the rear camera.
    //Note: You need to ensure the 'captureSize' in Constants.c reflects
    //which camera you plan to use, as the image sizes differ.
    AVCaptureDeviceInput *flashDevice = nil;
    for (AVCaptureDevice *camera in [AVCaptureDevice devices]) 
    {
        if (camera.position == AVCaptureDevicePositionBack) 
        {
            captureInput = [AVCaptureDeviceInput deviceInputWithDevice:camera error:nil];
            break;
        }
    }
    */
    
    //Default camera selection.
    //captureInput = [AVCaptureDeviceInput deviceInputWithDevice:[AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo] error:NULL];

    mLayer = [CALayer layer];
    mLayer.frame = self.view.bounds;
    mLayer.transform = CATransform3DRotate(CATransform3DIdentity, M_PI, 0, 0, 1);
    mLayer.contentsGravity = kCAGravityResizeAspectFill;

    [self.view.layer addSublayer:mLayer];
    self.view.backgroundColor = [UIColor blackColor];
    
    AVCaptureVideoDataOutput *captureOutput = [AVCaptureVideoDataOutput new];
    dispatch_queue_t video_queue = dispatch_queue_create("cvocv.processor", NULL);
    [captureOutput setSampleBufferDelegate:self queue:video_queue];
    dispatch_release(video_queue);
    
    NSString* pfKey = (NSString*)kCVPixelBufferPixelFormatTypeKey;
    NSNumber* pfVal = [NSNumber numberWithUnsignedInt:kCVPixelFormatType_32BGRA];
    NSDictionary* vSettings = [NSDictionary dictionaryWithObject:pfVal forKey:pfKey];
    [captureOutput setVideoSettings:vSettings];

    captureOutput.minFrameDuration = CMTimeMake(1, 24);
    captureOutput.alwaysDiscardsLateVideoFrames = YES;
    
    mCaptureSession = [AVCaptureSession new];
    [mCaptureSession addInput:captureInput];
    [mCaptureSession addOutput:captureOutput];

    [mCaptureSession beginConfiguration];
    
    if(captureSize == SMALL_FRONT ||
       captureSize == MEDIUM_FRONT ||
       captureSize == LARGE_FRONT)
    {
        mLayer.transform = CATransform3DRotate(CATransform3DIdentity, M_PI, 0, 0, 1);
    }
    
    switch(captureSize)
    {
        case(SMALL_BACK):
        case(SMALL_FRONT):
            [mCaptureSession setSessionPreset:AVCaptureSessionPresetLow]; //192x144
            break;
        case(MEDIUM_BACK):
        case(MEDIUM_FRONT):
            [mCaptureSession setSessionPreset:AVCaptureSessionPreset640x480];
            break;
        case(LARGE_BACK):
        case(LARGE_FRONT):
            [mCaptureSession setSessionPreset:AVCaptureSessionPresetHigh]; //1280x720
            break;
    }

    [mCaptureSession commitConfiguration];
    [mCaptureSession startRunning];
    mVideoProcessor = new VideoProcessor();
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection
{
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);

    CVPixelBufferLockBaseAddress(imageBuffer,0);
    
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer); 
    size_t width = CVPixelBufferGetWidth(imageBuffer); 
    size_t height = CVPixelBufferGetHeight(imageBuffer); 
    
    char *imgData = mVideoProcessor->setFrameImage(CVPixelBufferGetWidth(imageBuffer), 
                                                   CVPixelBufferGetHeight(imageBuffer),
                                                   CVPixelBufferGetDataSize(imageBuffer),
                                                   (char*)CVPixelBufferGetBaseAddress(imageBuffer),
                                                   CVPixelBufferGetBytesPerRow(imageBuffer));

    CGContextRef frameContext = CGBitmapContextCreate(imgData, width, height, 8, bytesPerRow, mCS, kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
    CGImageRef frameImage = CGBitmapContextCreateImage(frameContext); 
    CGContextRelease(frameContext);
    
    [mLayer performSelectorOnMainThread:@selector(setContents:) withObject:(id)frameImage waitUntilDone:YES];
    
    CGImageRelease(frameImage);
    CVPixelBufferUnlockBaseAddress(imageBuffer,0);
    
    mVideoProcessor->freeConvertedImage();
}

// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

- (void)didReceiveMemoryWarning 
{
    [super didReceiveMemoryWarning];
}

- (void)viewDidUnload 
{

}

- (void)dealloc 
{
    [mCaptureSession release];
    [super dealloc];
}

@end
