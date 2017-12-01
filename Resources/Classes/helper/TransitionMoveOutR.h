#pragma once

#include "UIHeader.h"

NS_CC_BEGIN

class CC_DLL TransitionMoveOutR : public TransitionMoveInL
{
public:
    static TransitionMoveOutR* create(float t, Scene* scene);
    
CC_CONSTRUCTOR_ACCESS:
    TransitionMoveOutR();
    virtual ~TransitionMoveOutR();
    virtual void onEnter() override;
    
protected:
    virtual void initScenes() override;
    virtual ActionInterval* action() override;
    virtual void sceneOrder() override;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(TransitionMoveOutR);
};


class CC_DLL TransitionMoveOutL : public TransitionMoveOutR
{
public:
    static TransitionMoveOutL* create(float t, Scene* scene);
    
CC_CONSTRUCTOR_ACCESS:
    TransitionMoveOutL();
    virtual ~TransitionMoveOutL();
    
protected:
    virtual ActionInterval* action() override;
};
    
NS_CC_END
