//
//  UndoManager.cpp
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#include "UndoManager.h"
#include "FillNode.h"
#include "GameScene.h"
#include "PalletteManager.hpp"


static UndoManager *_sharedUndo = nullptr;

UndoManager* UndoManager::getInstance()
{
    if (! _sharedUndo)
    {
        _sharedUndo = new UndoManager();
    }
    
    return _sharedUndo;
}

void UndoManager::destroyInstance()
{
    xScheduler->unscheduleAllForTarget(this);
    CC_SAFE_DELETE(_sharedUndo);
}

UndoManager::UndoManager()
{
}

void UndoManager::init(ColoringNode * fillSVGNode, GameScene *gameScene)
{
    _fillNode = fillSVGNode;
    _gameScene = gameScene;
}

bool UndoManager::undo()
{
    if(!_dotColorDatas.empty())
    {
        //移除最上面的一个touch ended
        if (_dotColorDatas.back().type == 2) {
            _dotColorDatas.pop_back();
        }
        
        if(_dotColorDatas.empty())
            return false;
            
        bool colorSet = false;
        bool bgChange = false;
        
        while (!_dotColorDatas.empty()) {
            auto data = _dotColorDatas.back();
            if (data.type == 2)
                break;
            
            if (data.type == 0)
            {
                _fillNode->resetPatternColor();
                
                if (!colorSet) {
                    colorSet = true;
                    _color = data.color;
                    _fillNode->setStartColor(_color);
                }
                else if(_color != data.color)
                {
                    _color = data.color;
                    _fillNode->setStartColor(_color);
                }
                _fillNode->coloringNodeSpace(data.dot);
            }
            else
            {
                if (!colorSet) {
                    colorSet = true;
                    _color = data.color;
                    _fillNode->setStartColor(_color);
                }
                else if(_color != data.color)
                {
                    _color = data.color;
                    _fillNode->setStartColor(_color);
                }
                
                _bgColor = _color;
                bgChange = true;
            }
            
            _dotColorDatas.pop_back();
        }
        
        //最后来设置背景颜色
        
        if (bgChange) {
            _fillNode->resetPatternColor();
            _gameScene->undoBackgroundColor(Color3B(_bgColor));
        }

        _fillNode->setStartColor(Color4B(xPallette->getCurrentColor()));
        _fillNode->resetPatternColor();
        
        return true;
    }
    
    return false;
}

void UndoManager::painted()
{
    _painted = true;
    
    DotColorData data;
    data.type = 2;
    
    _dotColorDatas.push_back(data);
}

void UndoManager::record(const Color3B &color)
{
    DotColorData data;
    data.color = Color4B(color);
    data.type = 1;
        
    _dotColorDatas.push_back(data);
}

void UndoManager::record(const Vec2 &pos)
{
    //转换为NodeSpace
    auto nodePos = _fillNode->convertToNodeSpace(pos);
    auto color = _fillNode->getCanvas()->readPixel(_fillNode->getCanvas(), nodePos.x, nodePos.y);

    if( color.r == color.g && color.g == color.b && color.r < 248) return;
    
    DotColorData data;
    data.dot = nodePos;
    data.color = color;
    data.type = 0;
    
    _dotColorDatas.push_back(data);
}

void UndoManager::reset()
{
    _dotColorDatas.clear();
}

ssize_t UndoManager::getRecordCount()
{
    return _dotColorDatas.size();
}
