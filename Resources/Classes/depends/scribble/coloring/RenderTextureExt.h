//
//  RenderTextureExt.hpp
//  ColorGarden
//
//  Created by lidong on 16/1/18.
//
//

#ifndef RenderTextureExt_hpp
#define RenderTextureExt_hpp

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;

class RenderTextureExt : public RenderTexture {
    
    
public:
    static RenderTextureExt * create(int w ,int h, Texture2D::PixelFormat format, GLuint depthStencilFormat);
    
    /** creates a RenderTexture object with width and height in Points and a pixel format, only RGB and RGBA formats are valid */
    static RenderTextureExt * create(int w, int h, Texture2D::PixelFormat format);
    
    /** creates a RenderTexture object with width and height in Points, pixel format is RGBA8888 */
    static RenderTextureExt * create(int w, int h);
    
    Color4B readPixel(RenderTexture *pRenderTexture, GLint pX, GLint pY);
    
};


#endif /* RenderTextureExt_hpp */
