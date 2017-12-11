//
//  ColoringNode.h
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#ifndef __ColoringNode__
#define __ColoringNode__

#include <stdio.h>
#include <cocos2d.h>
#include <string>
#include "RenderTextureExt.h"


using namespace std;
using namespace cocos2d;

extern CC_DLL const GLchar * Coloring_frag;

class ColoringNode : public Node {
public:
    struct ColoringArea{
        int left;
        int right;
        int top;
        int bottom;
        
        ColoringArea(int pLeft = 0, int pRight = 0, int pTop = 0, int pBottom = 0) : left(pLeft), right(pRight), top(pTop), bottom(pBottom){}
    };
    
    enum class ColoringMode{
        eFlat = 0x0,//平板着色
        eHorizonSmooth,//水平平滑着色（从左到右）
        eVerticalSmooth,//竖直平滑着色（从下到上）
        eDiagonalSmooth//对角平滑着色（从左下到右上）
    };
    
    enum class ColoringZOrder{
        eZCanvas,
        eZPattern,
        eZLineFrame
    };
    
public:
    static ColoringNode *create(string pTemplateFile, string pLineArtFile);
    ColoringNode();
    ~ColoringNode();
    bool init(string pTemplateFile, string pLineArtFile);
    virtual void setBrushShader();
    bool drawLineFrame();
    virtual bool coloring(Point pWorldPosition);
    virtual bool coloringNodeSpace(Point pNodePosition);
    Texture2D *getShapeBounds(Image *pShapeImage);
    bool insideLineframe(Vec2 pWorldPosition);
    
    inline void setColoringMode(ColoringMode pMode){
        _coloringMode = pMode;
        GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
        lState->setUniformInt("i_color_model", static_cast<int>(_coloringMode));
    }
    inline void setStartColor(const Color4B &pColor){
        _startColor = pColor;
        Color4F lStartColor4F(_startColor);
        GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
        lState->setUniformVec4("v_start_color", Vec4(lStartColor4F.r, lStartColor4F.g, lStartColor4F.b, lStartColor4F.a));
    }
    inline void setEndColor(const Color4B &pColor){
        _endColor = Color4B::WHITE;
        Color4F lEndColor4F(Color4B(255,255,255,255));
        GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getTemplate()->getGLProgram());
        lState->setUniformVec4("v_end_color", Vec4(lEndColor4F.r, lEndColor4F.g, lEndColor4F.b, lEndColor4F.a));
    }
    inline void setDefaultColor(Color4F pColor = Color4F::WHITE){
        _canvasDefaultColor = pColor;
        _canvas->clear(pColor.r, pColor.g, pColor.b, pColor.a);
    }
    inline void resetPatternColor(){
        _prePatternColor = Color4B::BLACK;
    }
    void reset();//重置画布
    virtual void uploadRenderPara();
    bool willColoring(Point pWorldPosition);
protected:
    virtual void initCanvas(Size pCanvasSize);
    virtual void initRenderPara();
    virtual void initPattern(string pPatternImage);
    
public:
    static const int gPixelSize = 5;
    static const int gTextureSize = 256;
    
protected:
    CC_SYNTHESIZE_RETAIN(Sprite *, _template, Template);
    Sprite *_lineArt;
    CC_SYNTHESIZE_READONLY(RenderTextureExt *, _canvas, Canvas);
    ColoringMode _coloringMode;
    Color4B _startColor;
    Color4B _endColor;
    
    Texture2D *_shapeBoundTexture;
    Image *_templateImage;
    Color4F _canvasDefaultColor;
    Color4B _prePatternColor;
};

#pragma mark - ColoringTouchNode
class ColoringTouchNode : public ColoringNode{
public:
    static ColoringTouchNode *create(string pTemplateFile, string pLineArtFile);
    bool init(string pTemplateFile, string pLineArtFile);
    
public:
    CC_SYNTHESIZE(bool, _ignoreOutOfLineframe, IgnoreOutOfLineframe);//忽略线框图外部区域，该区域填色无效
};

#endif /* defined(__ColoringNode__) */