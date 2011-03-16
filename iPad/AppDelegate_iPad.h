//
//  AppDelegate_iPad.h
//  CVOCV-iOS
//
//  Created by buza on 3/16/11.
//  Copyright 2011 BuzaMoto. All rights reserved.
//

#import <UIKit/UIKit.h>

@class CVOCV_IOSViewController;

@interface AppDelegate_iPad : NSObject <UIApplicationDelegate> 
{
    CVOCV_IOSViewController *viewController;
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet CVOCV_IOSViewController *viewController;

@end

