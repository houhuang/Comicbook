//
//  GraphSprite.hpp
//  ALABS0010
//
//  Created by houhuang on 16/3/29.
//
//

#ifndef GraphSprite_hpp
#define GraphSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "GraphManager.hpp"
USING_NS_CC;
using namespace std;

class GraphSprite : public Node
{
public:
    static GraphSprite* create(Graph & graph);
    
    GraphSprite();
    ~GraphSprite();
    
    bool init(Graph & graph);
    
public:
    void display();
    void imageLoadedCallback();
    void loadGraphSprite(Ref* ref);
    bool getCurrentGraphIslock();
public:
    string m_thumbs;
    string m_save;
    Sprite* m_pRotateSprite;
    Texture2D* m_originalTexture = nullptr;
    Texture2D* m_saveTexture = nullptr;
    
    Graph  m_gGraphInfo;
    Color3B m_cBGColor;
    
    bool m_bIsLock;
};

#endif /* GraphSprite_hpp */
