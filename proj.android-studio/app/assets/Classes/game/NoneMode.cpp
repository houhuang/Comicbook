//
//  NoneMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "NoneMode.h"
#include "GameScene.h"

NoneMode::NoneMode(GameScene * layer) : BaseMode(layer)
{
}

void NoneMode::start()
{
    _gameScene->stopPaint();
}

void NoneMode::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesBegan(touches, unused_event);
    
    //如果收到一个began消息, 立刻进入wait mode
    if(_gameScene->getFingerCount() == 1)
    {
        //_gameScene->changeColor();
        
        //收集paint
        _gameScene->_paints.clear();
        
        GameScene::Paint paint;
        paint.pos = _gameScene->convertToWorldSpace(touches[0]->getLocation());
        
        if (paint.pos.y < _gameScene->getContentSize().height - 10)
        {
            _gameScene->_paints.push_back(paint);
        }
        _gameScene->setWaitBeginVec(touches[0]->getLocation());
        _gameScene->switchMode(Mode::WAIT);
    }
    else if (_gameScene->getFingerCount() == 2)
    {
        _gameScene->switchMode(Mode::ZOOM);
    }
    else
    {
        _gameScene->switchMode(Mode::ZOOM);
    }
}