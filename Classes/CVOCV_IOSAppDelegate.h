//
//  CVOCV_IOSAppDelegate.h
//  CVOCV_IOS
//
//  Created by buza on 3/14/11.
//  Copyright 2011 BuzaMoto. All rights reserved.
//



#import <UIKit/UIKit.h>


@class CVOCV_IOSViewController;

@interface CVOCV_IOSAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    CVOCV_IOSViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet CVOCV_IOSViewController *viewController;

@end

