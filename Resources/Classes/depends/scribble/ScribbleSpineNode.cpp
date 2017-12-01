//
//  ScribbleSpineNode.cpp
//  TestGame
//
//  Created by renhong on 16/1/8.
//
//

#include "ScribbleSpineNode.h"
#ifdef SCRIBBLE_USE_SPINE
ScribbleSpineNode *ScribbleSpineNode::create(Size pCanvasSize){
    ScribbleSpineNode *ret = new ScribbleSpineNode();
    if(ret && ret->init(pCanvasSize)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleSpineNode::ScribbleSpineNode(){
    _worldVertices = nullptr;
    _skeletonAnimation = nullptr;
    _slot = nullptr;
}

ScribbleSpineNode::~ScribbleSpineNode(){
    free(_worldVertices);
    this->setSkeletonAnimation(nullptr);
    this->setSlot(nullptr);
}

bool ScribbleSpineNode::init(Size pCanvasSize){
    if ( !ScribbleTouchNode::init(pCanvasSize) ){
        return false;
    }
    _worldVertices = (float *)malloc(1000 * sizeof(float)); // Max number of vertices per mesh.
    memset(_worldVertices, 0, 1000 * sizeof(float));
    _touchListener->setSwallowTouches(false);
    this->scheduleUpdate();
    return true;
}

void ScribbleSpineNode::displaySkin(){
    switch (this->getSlot()->attachment->type) {
        case SP_ATTACHMENT_REGION:{
            spRegionAttachment* attachment = (spRegionAttachment*)this->getSlot()->attachment;
            spAtlasPage* lScribbleAtlasPage =  this->createScribbleAtlasPage();
            spAtlasRegion* region = (spAtlasRegion*)attachment->rendererObject;
            //    this->getSlot()->flipY = true;
            this->displayScribbleAtlasPage(lScribbleAtlasPage, region);
            
            spRegionAttachment_setUVs(attachment, 0, 1, 1, 0, 0);
            break;
        }
        case SP_ATTACHMENT_MESH:{
            spMeshAttachment* attachment = (spMeshAttachment*)this->getSlot()->attachment;
            spAtlasPage* lScribbleAtlasPage =  this->createScribbleAtlasPage();
            spAtlasRegion* region = (spAtlasRegion*)attachment->rendererObject;
            //    this->getSlot()->flipY = true;
            this->displayScribbleAtlasPage(lScribbleAtlasPage, region);
            
            attachment->regionU = 0;
            attachment->regionV = 1;
            attachment->regionU2 = 1;
            attachment->regionV2 = 0;
            
            spMeshAttachment_updateUVs(attachment);
            break;
        }
        case SP_ATTACHMENT_SKINNED_MESH:{
            spSkinnedMeshAttachment *attachment = (spSkinnedMeshAttachment*)this->getSlot()->attachment;
            spAtlasPage* lScribbleAtlasPage =  this->createScribbleAtlasPage();
            
            spAtlasRegion* region = (spAtlasRegion*)attachment->rendererObject;
            //    this->getSlot()->flipY = true;
            this->displayScribbleAtlasPage(lScribbleAtlasPage, region);
            
            attachment->regionU = 0;
            attachment->regionV = 1;
            attachment->regionU2 = 1;
            attachment->regionV2 = 0;
            attachment->regionRotate = 0;
            
            spSkinnedMeshAttachment_updateUVs(attachment);
            break;
        }
        default:
            break;
    }
}

spAtlasPage* ScribbleSpineNode::createScribbleAtlasPage(){
    spAtlas* atlas = this->getSkeletonAnimation()->getAtlas();
    Texture2D *imgTex = this->getCanvas()->getSprite()->getTexture();
    
    Size size = imgTex->getContentSize();
    spAtlasPage* page =  atlas->pages;
    spAtlasPage* newPage = spAtlasPage_create(atlas , ScribbleSpineAtlasPage);
    newPage->format = page->format;
    newPage->magFilter = page->magFilter;
    newPage->minFilter = page->minFilter;
    newPage->rendererObject = imgTex;
    newPage->width = size.width;
    newPage->height = size.height;
    newPage->uWrap = page->uWrap;
    newPage->vWrap = page->vWrap;
    
    return newPage;
}

void ScribbleSpineNode::displayScribbleAtlasPage(spAtlasPage* atlasPage, spAtlasRegion* region){
    /*自己替换的图片只有一张*/
    region->height= atlasPage->height;
    region->width = atlasPage->width;
    region->offsetX = 0;
    region->offsetY = 0;
    region->originalHeight = atlasPage->height;
    region->originalWidth = atlasPage->width;
    region->rotate = 0;
    region->u = 0;
    region->v = 0;
    region->u2 = 1;
    region->v2 = 1;
    
    /*自己创建的Page自己释放一下,默认创建的，在骨骼动画释放时会释放*/
    if (region->page &&
        strcmp(region->page->name, ScribbleSpineAtlasPage) == 0){
        FREE(region->page->name);
        FREE(region->page);
    }
    region->page = atlasPage;
}

void ScribbleSpineNode::update(float dt){
    if (nullptr == this->getSlot()) {
        return;
    }
    if (nullptr == this->getSkeletonAnimation()) {
        return;
    }
    switch (this->getSlot()->attachment->type) {
        case SP_ATTACHMENT_REGION:{
            spRegionAttachment* attachment = (spRegionAttachment*)this->getSlot()->attachment;
            spRegionAttachment_computeWorldVertices(attachment, this->getSlot()->bone, _worldVertices);
            float lAnimationX = this->getSkeletonAnimation()->getPositionX();
            float lAnimationY = this->getSkeletonAnimation()->getPositionY();
            //左下角
            float lbX = _worldVertices[SP_VERTEX_X1] + lAnimationX;
            float lbY = _worldVertices[SP_VERTEX_Y1] + lAnimationY;
            //左上角
            float ltX = _worldVertices[SP_VERTEX_X2] + lAnimationX;
            float ltY = _worldVertices[SP_VERTEX_Y2] + lAnimationY;
            //右上角
            float rtX = _worldVertices[SP_VERTEX_X3] + lAnimationX;
            float rtY = _worldVertices[SP_VERTEX_Y3] + lAnimationY;
            //右下角
            float rbX = _worldVertices[SP_VERTEX_X4] + lAnimationX;
            float rbY = _worldVertices[SP_VERTEX_Y4] + lAnimationY;
            
            
            Vec2 lWorldPos((ltX + rbX) / 2.0, (ltY + rbY) / 2.0);
            this->setPosition(this->getParent()->convertToNodeSpace(lWorldPos));
            this->setScale(this->getSlot()->bone->scaleX);
            float lDeltaY = rtY - rbY;
            float lDeltaX = rbX - rtX;
            float lDeltaAngle = 0;
            if (lDeltaY > 0) {//一二象限
                if (lDeltaX > 0) {//一象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//二象限
                    lDeltaAngle = 90;
                }
                else{//+Y轴
                    lDeltaAngle = -90;
                }
            }
            else if (lDeltaY < 0){//三四象限
                if (lDeltaX > 0) {//四象限
                    lDeltaAngle = -90;
                }
                else if(lDeltaX < 0){//三象限
                    lDeltaAngle = 90;
                }
                else{//-Y轴
                    lDeltaAngle = 90;//没有测试数据，暂未验证此数据的准确性。
                }
            }
            else{//X轴
                //lDeltaAngle = 0;//没有测试数据，暂未验证此数据的准确性。
            }
            
            Vec2 lX(1, 0);
            Vec2 lV(lDeltaX, lDeltaY);//(slot->rtX - slot->rbX,slot->rtY - slot->rbY);
            /*
             //尝试使用向量计算来求cos角度
             float lCosTheta = lX.dot(lV) / (sqrt(lX.x * lX.x + lX.y * lX.y) * sqrt(lV.x * lV.x + lV.y * lV.y));
             float lR2 = CC_RADIANS_TO_DEGREES(acos(lCosTheta));
             */
            float lR = CC_RADIANS_TO_DEGREES(atan(lDeltaY / lDeltaX)) +  lDeltaAngle;
            
            //    printf("s->bone->rotation %f %f %f\n", lR, lR2, lCosTheta);
            this->setRotation(lR);//lR2 + 90  lR2+90+180
            
            break;
        }
        case SP_ATTACHMENT_MESH:{
            break;
        }
        case SP_ATTACHMENT_SKINNED_MESH:{
            spSlot *lSlot = this->getSlot();
            spBone *lBone = lSlot->bone;
            spSkinnedMeshAttachment* attachment = (spSkinnedMeshAttachment*)lSlot->attachment;
            Vec2 lBonePos = Vec2(lBone->x + 0, lBone->
                             
                             y + attachment->height / 2);
            Vec2 lAnimationWorldPos = this->getSkeletonAnimation()->getParent()->convertToWorldSpace(Vec2(this->getSkeletonAnimation()->getPositionX(), this->getSkeletonAnimation()->getPositionY()));
            this->setPosition(lBonePos + lAnimationWorldPos);
            break;
        }
    }
}
#endif