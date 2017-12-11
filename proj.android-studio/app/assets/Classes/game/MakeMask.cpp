//
//  MakeMask.cpp
//  ALABS0010
//
//  Created by luoxiaopeng on 16/3/21.
//
//

#include "MakeMask.h"
#include "ConnectedComponents.h"

void MakeMask::make(const string& filename)
{
    //检查文件是否已经存在
    auto startPos = filename.find_last_of("/");
    if (startPos == string::npos) startPos = 0;
    auto endPos = filename.find_last_of(".");
    auto savePath = FileUtils::getInstance()->getWritablePath() + filename.substr(startPos, endPos - startPos) + "_mask.png";
    
    if (FileUtils::getInstance()->isFileExist(savePath)) return;
    
    auto image = new Image();
    image->autorelease();
    image->initWithImageFile(filename);

    _imageData = image->getData();
    _width = image->getWidth();
    _height = image->getHeight();
    
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            if (getImageAlpha(x, y) < 100)          //不透明度 < 100 ?
            {
                setData(x, y, 1);
            }
            //黑色
            else
            {
                setData(x, y, 0);
            }
        }
    }
    
    unsigned int * input = (unsigned int *)_imageData;
    unsigned int * output = new unsigned int[_width * _height];
    ConnectedComponents cc(_width*_height);
    cc.connected(input, output, _width, _height,
                 std::equal_to<unsigned char>(),
                 false);
    
    
    //generate colors, 后面根据需求要决定是否需要再次填色, 可以直接用memcpy
    vector<Color4B> colors;
    Color3B start(10, 10, 10);
    Color3B end(255, 255, 255);
    
    for (int b = start.b; b < end.b; b += 5) {
        for (int g = start.g; g < end.g; g += 5) {
            for (int r = start.r; r < end.r; r += 5) {
                colors.push_back(Color4B(r, g, b, 255));
            }
        }
    }
    
    std::srand(unsigned(std::time(0)));
    std::random_shuffle(colors.begin(), colors.end());
    
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            
            int label = output[y * _width + x];
            if (label == 0)
            {
                setImageColor(x, y, Color4B::BLACK);
            }
            else
            {
                setImageColor(x, y, colors.at(label));
            }
        }
    }
    
    CC_SAFE_DELETE(output);
    
    image->saveToFile(savePath);
    Director::getInstance()->getRenderer()->render();
}

unsigned int MakeMask::getImageAlpha(int x, int y)
{
    auto data = (unsigned int *)_imageData;
    data += (y * (int)_width + x);
    return (*data >> 24) & 0xFF;
}

void MakeMask::setData(int x, int y, int value)
{
    auto data = (unsigned int *)_imageData;
    unsigned int offset = (y * (int)_width + x);
    data += offset;
    *data = value;
}

void MakeMask::setImageColor(int pX, int pY, const Color4B &pColor)
{
    auto data = (unsigned int *)_imageData;
    data += (pY * (int)_width + pX);
    *data = (pColor.r | (pColor.g << 8) | (pColor.b << 16) | (pColor.a << 24));
}

Color4B MakeMask::getImageColor(int pX, int pY)
{
    unsigned char *data = _imageData;//_lineImage->getData();
    const int bytesPerPixel = 4;//_lineImage->getBitPerPixel() / 8;
    
    int index = pY * _width + pX;//_lineImage->getWidth()
    index *= bytesPerPixel;
    unsigned char r = data[index];
    unsigned char g = data[index + 1];
    unsigned char b = data[index + 2];
    unsigned char a = data[index + 3];
    
    //    CCLOG("getPixelColor %d %d %d %d at %d %d", r, g, b, a , pX, pY);
    return Color4B(r, g, b, a);
}
