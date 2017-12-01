//
//  AppConfigs.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/21/15.
//
//

#pragma once

#define THUMB_WIDTH                                     512
#define TOP_LINE        50
#define BOTTOM_LINE     70

#define ACCELERATION_X  12

#define PALETTE_SIZE            (V::isIpad()? Size(658, 84) : Size(500, 200))

#define SPACE                   (V::isIpad()? 20 : 22)
#define BOTTOM_HEIGHT           (V::isIpad()? 56 : 86)
#define TOP_HEIGHT              (V::isIpad()? 55 : 86)
#define TOP_HEIGHT_HEIGHT       (V::isIpad()? 97 : 146)


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define IAP_PREMIUM_SUBSCRIPTION                        "net.applabsinc.coloringbookforwomengirl.unlockall"
#else
#define IAP_PREMIUM_SUBSCRIPTION                        "net.applabsinc.android_coloranimaltopiafree.unlock"
#endif

#define COLORBOOK_IAP                                   "colorbook_iap"