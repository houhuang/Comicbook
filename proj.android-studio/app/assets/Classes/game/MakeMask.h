//
//  MakeMask.hpp
//  ALABS0010
//
//  Created by luoxiaopeng on 16/3/21.
//
//

#ifndef MakeMask_hpp
#define MakeMask_hpp

#include "UIHeader.h"

class MakeMask {
    
public:
    void make(const string& filename);
    
private:
    inline unsigned int getImageAlpha(int x, int y);
    inline void setData(int x, int y, int value);
    inline void setImageColor(int pX, int pY, const Color4B &pColor);
    Color4B getImageColor(int pX, int pY);
    
private:
    unsigned char *    _imageData;
    int                 _width;
    int                 _height;
};

#endif /* MakeMask_hpp */
