#include "TransitionMoveOutR.h"

NS_CC_BEGIN
TransitionMoveOutR::TransitionMoveOutR()
{
}

TransitionMoveOutR::~TransitionMoveOutR()
{
}

TransitionMoveOutR* TransitionMoveOutR::create(float t, Scene* scene)
{
    TransitionMoveOutR* newScene = new TransitionMoveOutR();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionMoveOutR::onEnter()
{
    TransitionScene::onEnter();
    this->initScenes();
    
    ActionInterval *a = this->action();
    
    _outScene->runAction
    (
     Sequence::create
     (
      this->easeActionWithAction(a),
      CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
      nullptr
      )
     );
}

ActionInterval* TransitionMoveOutR::action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveTo::create(_duration, Vec2(-s.width,0));
}

void TransitionMoveOutR::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(Vec2::ZERO);
    _outScene->setPosition(Vec2::ZERO);
}

void TransitionMoveOutR::sceneOrder()
{
    _isInSceneOnTop = false;
}


TransitionMoveOutL::TransitionMoveOutL()
{
}

TransitionMoveOutL::~TransitionMoveOutL()
{
}

TransitionMoveOutL* TransitionMoveOutL::create(float t, Scene* scene)
{
    TransitionMoveOutL* newScene = new TransitionMoveOutL();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

ActionInterval* TransitionMoveOutL::action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveTo::create(_duration, Vec2(s.width,0));
}

NS_CC_END
