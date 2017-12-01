//
//  NewDialog.hpp
//  ColorGarden
//
//  Created by houhuang on 17/9/20.
//
//

#ifndef NewDialog_hpp
#define NewDialog_hpp

#include <stdio.h>
#include "UITools.h"
USING_NS_CC;
using namespace std;
typedef std::function<void(const string& )> ccDialogCallback;

class NewDialog : public LayerColor
{
public:
    static NewDialog* create(const string& tText, const string& lText, const string rText, float fontTSize = 15);
    bool init(const string& tText, const string& lText, const string rText, float fontTSize);
    
    ~NewDialog();
    void onExit();
    void onEnterTransitionDidFinish();
    
    void addListener();
    void onButton(Ref* ref);
    
    void addButtonListener(const ccDialogCallback callBack);
    
public:
    void initUIWithiPhone(const string& tText, const string& lText, const string rText, float fontTSize);
    void initUIWithiPad(const string& tText, const string& lText, const string rText, float fontTSize);
protected:
    Sprite* _dialogBG;
    
    ccDialogCallback _callBack;
    
    string _leftText;
    string _rightText;
    Vec2   _firstPos;
};

#endif /* NewDialog_hpp */
