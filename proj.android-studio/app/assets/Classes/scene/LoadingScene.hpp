//
//  LoadingScene.hpp
//  ALABS0002NEW
//
//  Created by houhuang on 16/8/8.
//
//

#ifndef LoadingScene_hpp
#define LoadingScene_hpp

#include "UITools.h"
#include "BaseScene.hpp"

class LoadingScene : public BaseScene
{
public:
    CREATE_FUNC(LoadingScene);
    bool init();

    void gotoChapterScene(float dt);
    void onEnterTransitionDidFinish();
};

#endif /* LoadingScene_hpp */
