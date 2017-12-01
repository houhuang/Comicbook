//
//  BaseScene.hpp
//  ColorGarden
//
//  Created by houhuang on 17/4/10.
//
//

#ifndef BaseScene_hpp
#define BaseScene_hpp

#include <stdio.h>
#include "UITools.h"
USING_NS_CC;

class BaseScene : public Scene
{
public:
    CREATE_FUNC(BaseScene);
    bool init();
    
    BaseScene();
    ~BaseScene();
public:
    void responseForcedUpdate(EventCustom* event);
    void onDialog(const std::string& name);
    
    virtual void addBackListener();
    void responseBack(const std::string& name);
    void updateClickBackState(float dt);
    
    void responseUpdateBackTrue(EventCustom* event);
    
    void timingClearTexture(float detalTime);

    void resetFristClick();
    
    void setTouchEnable(bool isEnable);
protected:
    bool _isForcedUpdate;
    bool _firstClickBack;
    
    float _time;
};

#endif /* BaseScene_hpp */
