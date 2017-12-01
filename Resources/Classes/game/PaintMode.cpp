//
//  PaintMode.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "PaintMode.h"
#include "GameScene.h"
#include "UndoManager.h"


PaintMode::PaintMode(GameScene * layer) : BaseMode(layer)
{
    
}

PaintMode::~PaintMode()
{
    
}

void PaintMode::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
    auto current = _gameScene->convertToWorldSpace(touches[0]->getLocation());
    auto previous = _gameScene->convertToWorldSpace(touches[0]->getPreviousLocation());

    _gameScene->paint(current, previous);
}

void PaintMode::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
    BaseMode::onTouchesEnded(touches, unused_event);
    
    auto current = _gameScene->convertToWorldSpace(touches[0]->getLocation());
    //_gameScene->paintStamp(current);
}

void PaintMode::stop()
{
    _gameScene->stopPaint();
    xUndo->painted();
}