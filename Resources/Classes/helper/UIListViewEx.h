//
//  UIListViewExtend.h
//  SingleEye
//
//  Created by gwsoft on 1/6/14.
//
//

#pragma once

#include "UIHeader.h"

class ListViewEx
{
    class CAccessor : public ListView
    {
        friend class ListViewEx;
    };
    
public:
    CAccessor  *me;
    
    ListViewEx(ListView *_friend)
    {
        me = (CAccessor*)_friend;
    }
    
    void jumpToDestinationEx(const Vec2 &des)
    {
        me->jumpToDestination(des);
    }
};