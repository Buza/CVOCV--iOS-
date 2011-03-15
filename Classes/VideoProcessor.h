/*
 *  VideoProcessor.h
 *  CVOCV_IOS
 *
 *  Created by buza on 3/14/11.
 *  Copyright 2011 BuzaMoto. All rights reserved.
 *
 */

class VideoProcessor 
{
    public:
        VideoProcessor();
        char *setFrameImage(int width, int height, int size, char* imgData, int withStep);
        void freeConvertedImage();
};