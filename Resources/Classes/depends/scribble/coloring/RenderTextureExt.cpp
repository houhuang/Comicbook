//
//  RenderTextureExt.cpp
//  ColorGarden
//
//  Created by lidong on 16/1/18.
//
//

#include "RenderTextureExt.h"


RenderTextureExt * RenderTextureExt::create(int w, int h, Texture2D::PixelFormat eFormat)
{
    RenderTextureExt *ret = new RenderTextureExt();
    
    if(ret && ret->initWithWidthAndHeight(w, h, eFormat))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

RenderTextureExt * RenderTextureExt::create(int w ,int h, Texture2D::PixelFormat eFormat, GLuint uDepthStencilFormat)
{
    RenderTextureExt *ret = new RenderTextureExt();
    
    if(ret && ret->initWithWidthAndHeight(w, h, eFormat, uDepthStencilFormat))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

RenderTextureExt * RenderTextureExt::create(int w, int h)
{
    RenderTextureExt *ret = new RenderTextureExt();
    
    if(ret && ret->initWithWidthAndHeight(w, h, Texture2D::PixelFormat::RGBA8888, 0))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

Color4B RenderTextureExt::readPixel(RenderTexture *pRenderTexture, GLint pX, GLint pY)
{
    
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_oldFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    GLubyte *pTempData = NULL;
    
    do{
        if(! (pTempData = new GLubyte[4])){
            break;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(pX,pY,1, 1,GL_RGBA,GL_UNSIGNED_BYTE, pTempData);
        glBindFramebuffer(GL_FRAMEBUFFER, _oldFBO);

        
    } while (0);
    Color4B lColor = {pTempData[0], pTempData[1], pTempData[2], pTempData[3]};
    CC_SAFE_DELETE_ARRAY(pTempData);
    Director::getInstance()->getRenderer()->render();
    glFlush();
    return lColor;

}
