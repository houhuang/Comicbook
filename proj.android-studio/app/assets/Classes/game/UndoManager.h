//
//  UndoManager.h
//  ColorBook
//
//  Created by luoxiaopeng on 4/24/15.
//
//

#ifndef __ColorBook__UndoManager__
#define __ColorBook__UndoManager__

#include "UIHeader.h"
#include <deque>

#define xUndo  (UndoManager::getInstance())

class ColoringNode;
class GameScene;

class UndoManager : public Node
{
    struct DotColorData
    {
        int     type;       //0:中间图案, 1:周围背景, 2:touch end
        Vec2    dot;        //记录NodeSpace的Pos, 不要记录WorldSpace的Pos
        Color4B color;
    };
    
public:
    static UndoManager* getInstance();
    void destroyInstance();
    
    void init(ColoringNode * fillSVGNode, GameScene *gameScene);
    bool undo();
    void painted();
    void reset();
    void record(const Vec2 &pos);
    void record(const Color3B &color);
    ssize_t getRecordCount();
    
protected:
    UndoManager();

protected:    
    bool             _painted = false;
    GameScene *     _gameScene;
    ColoringNode *  _fillNode;
    
    deque<DotColorData> _dotColorDatas;
    Color4B          _color;
    Color4B          _bgColor;
};

#endif /* defined(__ColorBook__UndoManager__) */
