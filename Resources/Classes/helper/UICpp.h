#pragma once

#include "cocostudio/CocoStudio.h"
#include "ui/UIHelper.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"
#include "ui/UITextField.h"
#include "ui/UICheckBox.h"
#include "ui/UIListView.h"
#include "ui/UIPageView.h"
#include "ui/UILoadingBar.h"
#include "ui/UISlider.h"
#include "UILayoutEx.h"
#include "AudioHelper.h"
#include "AdsManager.h"
#include "STVisibleRect.h"
#include "StringsManager.h"
#include "SceneManager.h"
#include "Analytics.h"
#include "AppConfigs.h"
#include "NewDialog.hpp"

using namespace cocos2d::ui;
using namespace cocostudio::timeline;
using namespace cocostudio;

#define xAds                            AdsManager::getInstance()
#define xAnalytics                      Analytics::getInstance()
#define FONT_NAME                       "res/Muli-Regular.ttf"
#define UI_COLOR                        (Color3B(255, 255, 255))
