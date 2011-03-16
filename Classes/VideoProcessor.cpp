/*
 *  VideoProcessor.cpp
 *  CVOCV_IOS
 *
 *  Created by buza on 3/14/11.
 *  Copyright 2011 BuzaMoto. All rights reserved.
 *
 */

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "Constants.h"

#include "VideoProcessor.h"

char* close(IplImage* frameImage);
char* canny(IplImage *frameImage);
char* erode(IplImage *frameImage);
char* adaptiveThreshold(IplImage* frameImage);

IplImage *frameImage;
IplImage *convertedImage;

VideoProcessor::VideoProcessor() 
{
    convertedImage = NULL;
    frameImage = (IplImage*)malloc(sizeof(IplImage));
}

char* VideoProcessor::setFrameImage(int width, int height, int size, char* imgData, int withStep) 
{
    //Fill in the OpenCV image struct from the data from CoreVideo.
    frameImage->nSize       = sizeof(IplImage);
    frameImage->ID          = 0;
    frameImage->nChannels   = 4;
    frameImage->depth       = IPL_DEPTH_8U;
    frameImage->dataOrder   = 0;
    frameImage->origin      = 0; //Top left origin.
    frameImage->width       = width;
    frameImage->height      = height;
    frameImage->roi         = 0; //Region of interest. (struct IplROI).
    frameImage->maskROI     = 0;
    frameImage->imageId     = 0;
    frameImage->tileInfo    = 0;
    frameImage->imageSize   = size;
    frameImage->imageData   = imgData;
    frameImage->widthStep   = withStep;
    frameImage->imageDataOrigin = imgData;
    
    //Change the function used to process the image.
    
    return adaptiveThreshold(frameImage);
    //return close(frameImage);
    //return erode(frameImage);
    //return canny(frameImage);

}

/*!
 * @function adaptiveThresh
 * @discussion Perform adaptive thresholding.
 * @updated 2011-4-15
 */
char* adaptiveThreshold(IplImage* frameImage)
{
    IplImage *grayTex;
    IplImage *grayTemp;
    
    switch(captureSize)
    {
        case(SMALL_BACK):
        case(SMALL_FRONT):
            grayTex = cvCreateImage(cvSize(192, 144), IPL_DEPTH_8U, 1);
            grayTemp = cvCreateImage(cvSize(192, 144), IPL_DEPTH_8U, 1);
            
            break;
        case(MEDIUM_BACK):
        case(LARGE_FRONT):
        case(MEDIUM_FRONT):
            grayTex = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
            grayTemp = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 1);
            
            break;
        case(LARGE_BACK):
            grayTex = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 1);
            grayTemp = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 1);
            
            break;
    }
    
 
    cvCvtColor(frameImage, grayTex, CV_RGB2GRAY);
    
    int type =  CV_THRESH_BINARY;           //CV_THRESH_BINARY_INV; 
    int method = CV_ADAPTIVE_THRESH_MEAN_C; //CV_ADAPTIVE_THRESH_GAUSSIAN_C; 
    
    int blockSize = 173; 
    double offset = 1; 
    
    cvAdaptiveThreshold(grayTex, grayTemp, 255, method, type, blockSize, offset); 
    
    switch(captureSize)
    {
        case(SMALL_BACK):
        case(SMALL_FRONT):
            convertedImage = cvCreateImage(cvSize(192, 144), IPL_DEPTH_8U, 4);
            break;
        case(MEDIUM_BACK):
        case(LARGE_FRONT):
        case(MEDIUM_FRONT):
            convertedImage = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);
            break;
        case(LARGE_BACK):
            convertedImage = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 4);
            break;
    }
    
    cvCvtColor(grayTemp, convertedImage, CV_GRAY2RGB);
    
    cvReleaseImage(&grayTex);
    cvReleaseImage(&grayTemp);
    
    return convertedImage->imageDataOrigin;
}

/*!
 * @function close
 * @discussion Perform image closing with a custom kernel.
 * @updated 2011-4-13
 */
char* close(IplImage* frameImage)
{
    //Select based on the capture dimensions.
    switch(captureSize)
    {
        case(SMALL_BACK):
        case(SMALL_FRONT):
            convertedImage = cvCreateImage(cvSize(192, 144), IPL_DEPTH_8U, 4);
            break;
        case(MEDIUM_BACK):
        case(LARGE_FRONT):
        case(MEDIUM_FRONT):
            convertedImage = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);
            break;
        case(LARGE_BACK):
            convertedImage = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 4);
            break;
    }
    
    cvCopy(frameImage, convertedImage, 0);
    
    IplConvKernel* closeKernel = cvCreateStructuringElementEx(7, 7, 3, 3, CV_SHAPE_RECT, NULL);
    
    //Default number of iterations is 1. We'll do a few iterations to make the effect more pronounced.
    cvMorphologyEx(convertedImage, convertedImage, NULL, (IplConvKernel *)closeKernel, CV_MOP_CLOSE, 3);
    
    return convertedImage->imageDataOrigin;
}

/*!
 * @function erode
 * @discussion Perform image erosion. Erosion computes a local minimum over the area of the kernel. 
 * @updated 2011-4-13
 */
char* erode(IplImage *frameImage)
{
    //Select based on the capture dimensions.
    switch(captureSize)
    {
        case(SMALL_BACK):
        case(SMALL_FRONT):
            convertedImage = cvCreateImage(cvSize(192, 144), IPL_DEPTH_8U, 4);
            break;
        case(MEDIUM_BACK):
        case(LARGE_FRONT):
        case(MEDIUM_FRONT):
            convertedImage = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);
            break;
        case(LARGE_BACK):
            convertedImage = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 4);
            break;
    }
    
    cvCopy(frameImage, convertedImage, 0);
    
    cvErode(convertedImage, convertedImage, NULL, 3);
    
    return convertedImage->imageDataOrigin;
}

/*!
 * @function cannyTest
 * @discussion Canny edge detection.
 * @updated 2011-4-13
 */
char* canny(IplImage *frameImage)
{
    //Select based on the capture dimensions.
    switch(captureSize)
    {
        case(SMALL_BACK):
        case(SMALL_FRONT):
            convertedImage = cvCreateImage(cvSize(192, 144), IPL_DEPTH_8U, 4);
            break;
        case(MEDIUM_BACK):
        case(LARGE_FRONT):
        case(MEDIUM_FRONT):
            convertedImage = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 4);
            break;
        case(LARGE_BACK):
            convertedImage = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 4);
            break;
    }

    CvSize sz = cvSize(frameImage->width & -2, frameImage->height & -2);
    IplImage* timg = cvCloneImage(frameImage);
    IplImage* gray = cvCreateImage(sz, IPL_DEPTH_8U, 1); 
    IplImage* tgray =  cvCreateImage(sz, IPL_DEPTH_8U, 1);
    
    cvSetImageCOI(frameImage, 1);
    
    cvCopy(frameImage, tgray, 0);
    cvCanny(tgray, gray, 0, 5, 5);
    
    cvDilate(gray, gray, 0, 1);
    
    cvCvtColor(gray, convertedImage, CV_GRAY2RGB);
    
    cvReleaseImage(&gray);
    cvReleaseImage(&tgray);
    cvReleaseImage(&timg);   
    
    return convertedImage->imageDataOrigin;
}

void VideoProcessor::freeConvertedImage()
{
    if(convertedImage)
        cvReleaseImage(&convertedImage);
}
