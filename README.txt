
CVOCV_IOS: A CoreVideo-based OpenCV experimentation environment.
----------------------------------------------------------------

CVOCV was built in order to create an extremely simple and
lightweight OpenCV experimentation environment for
OS X 10.5.x and iOS 4.x. Using CoreVideo makes it possible to 
send video frames to OpenCV with a minimal amount of computational
overhead.

Limitations:
------------

The current implementation was designed to target the second generation
iPad (iPad2). Targeting the iPhone running iOS 4.x shouldn't pose any
significant issue, should you decide to target that device instead.

Prerequisites:
--------------

In order to get this project to run on an iOS device, you'll first
need to build OpenCV. These instructions worked for me, and I highly
recommend following them to build yourself a version of OpenCV that
you can run on your iOS device.

http://computer-vision-talks.com/2011/02/building-opencv-for-iphone-in-one-click/

Once you're done building, you'll need to change some of the project settings
to reflect where you've built OpenCV. Follow these instructions to do so (Note:
you'll only need to follow the first two steps to set up your include and lib 
directories:

http://computer-vision-talks.com/2011/01/using-opencv-in-objective-c-code/

Usage
-----

At the heart of CVOCV is the 'captureOutput' method found in
CVOCV_IOSViewController.mm. This is a callback procedure that is 
invoked by CoreVideo to provide direct access to the pixels coming from 
a video input device. 

To experiment with the existing examples offered by CVOCV, simply 
uncomment one of the lines in the VideoProcessor::setFrameImage()
method.

For example, specifying the line

return adaptiveThreshold(frameImage);

at the end of setFrameImage() will perform adaptive thresholding
on the incoming video frame and display the result on screen.

Feel free to add your own examples and try them out!

There are three native capture sizes for video frames on iOS 
devices: (192x144, 640x480, 1280x720). You may toggle between
these sizes via the 'captureSize' variable defined in Constants.c.
Note that increasing the capture frame size will increase 
the computational overhead of the OpenCV processing step.

A Note on Cameras
-----------------

You may switch between which camera device is used by editing the
'startCamera' method in CVOCV_IOSViewController.mm. 

Contributions
-------------

Contributions in the form of elegant, demonstrative examples are
welcome and encouraged. Contact me if you have something you
think would make a nice addition to this project.

-----

2011 Kyle Buza
buza@buzamoto.com

