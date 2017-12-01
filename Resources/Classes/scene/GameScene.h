//
//  GameScene.h
//
//
//  Created by luoxp on 3/18/15.
//
//

#pragma once

#include "UITools.h"
#include "UIPageViewEx.h"
#include "ColoringNode.h"
#include "BaseMode.h"
//#include "ColorScrollLayer.h"
#include "BaseScene.hpp"
#include "ColorLoopScrollLayer.hpp"


class Graph;
class NoneMode;
class PaintMode;
class MoveMode;
class ZoomMode;
class WaitMode;
class PickupMode;
class BlockLayer;

class  GameScene : public BaseScene
{
public:
    struct Paint
    {
        Vec2    pos;
        Vec2    previousPos;
    };
    
public :
    GameScene();
    virtual ~GameScene();
    
    CREATE_FUNC(GameScene);
    
    virtual bool init() override;
//    void onButton(Ref* pSender, Widget::TouchEventType type);
    void onButton(Ref* pSender);
    void onDialog(const string& name);
    void createScribble();
    void createZoomLayer();
    void drawLineFrame();
    float onEnterGame(int index);
    float onExitGame(int index);
    
    void onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event);
    void switchMode(BaseMode::Mode mode);
    void paintPrevious();
    void paint(Vec2 & current, Vec2 & previous);
    void stopPaint();
    Layer * getZoomLayer();
    void undoBackgroundColor(const Color3B & color);
    void onAfterCaptured(bool succeed, const string & filename);
    void onPageMove(EventCustom* event);
    void onChooseColor(EventCustom* event);
    void onAfterVisit(EventCustom* event);
    virtual void onEnterTransitionDidFinish() override;
    void applicationDidEnterBackground();
    
    void changePalletteName();
    
    void waitDownload(Ref* ref);
    
    void responseImageDownloaded(EventCustom* event);
    void makeButtonNotEnable();
    
    void update(float dt) override;
    
    //override BaseScene function
    void addBackListener();
    Color4B getPixelColor(Vec2 lpos);
    void setEnableReturnColor(bool isEnable);
    void updateColorBoard(Color3B color);
    
    void onModeButton(Ref* ref);
private:
    void createPallette();
    void changeBackgroundColor();
    void eraseLineFrame(unsigned char * imageData);
    
public: //状态机模式
    vector<Paint>   _paints;
    CC_SYNTHESIZE(int, _fingerCount, FingerCount);
    CC_SYNTHESIZE(Color3B, _preColorRecord, PreColorRecord);
    
    void setWaitBeginVec(Vec2 pos);
    Vec2 getWaitBeginVec();
private:
    Node *          _root;
    PageViewEx *    _pallettes;
    ColoringNode *  _fillNode;
    Text *          _palletteName;
    LayerColor *    _zoomLayer;
    BaseMode    * _currentMode = nullptr;
    NoneMode    * _noneMode;
    WaitMode    * _waitMode;
    PaintMode   * _paintMode;
    ZoomMode    * _zoomMode;
    MoveMode    * _moveMode;
    PickupMode  * _pickupMode;
    BlockLayer  * _blockLayer;
    
    Graph &     _graph;
    bool            _blockTouch = false;
    bool            _forward = true;
    Vec2            _rotatePos;
    Button *        _undo;
    Button *        _back;
    Button *        _del;
    Button *        _done;
//    ColorScrollLayer*   _palletteLayer;
    ColorLoopScrollLayer*   _palletteLayer;
    float _time;
    bool            _hasEnterGame = false;
    
    int     _undoBackupCount;
    bool    _isReset = false;
    string  _prePaletteName = "";
    string  _nexPaletteName = "";
    
    Sprite* _rotateSpr;
    Vec2    _waitBeginVec2;
    
    Image*  _pickupImage;
    Sprite* _pickupSprite;
    Color3B _currentChooseColor;
    bool _isEnableReturnColor;
    
    MenuItemSprite* _modeMenuItem;
};