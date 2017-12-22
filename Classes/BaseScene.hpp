//
//  BaseScene.hpp
//  ComicBook
//
//  Created by houhuang on 17/12/22.
//
//

#ifndef BaseScene_hpp
#define BaseScene_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class BaseScene : public Scene
{
public:
    CREATE_FUNC(BaseScene);
    
    ~BaseScene();
    BaseScene();
    bool init();

private:
    void responseGetResourceFaild(EventCustom* event);

};

#endif /* BaseScene_hpp */
