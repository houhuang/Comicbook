//
//  Dialog.h
//  ColorBook
//
//
//

#ifndef __ColorBook__Dialog__
#define __ColorBook__Dialog__

#include "UIHeader.h"

#define xDialog         (Dialog::getInstance())

typedef std::function<void(const string& )> ccDialogCallback;

class Dialog
{
public:
    static Dialog* getInstance();
    void show(const string& text, const string& left, const string& right, const ccDialogCallback &callback, int fontSize = -1);
    void show(const string& text, const string& button, const ccDialogCallback &callback, int fontSize = -1);
    
public:
    ~Dialog();
    void destroyInstance();
    void onButton(Ref* pSender, Widget::TouchEventType type);
    void setUserData(int userData);
    int getUserData();
    
    void onAction(Ref* ref);
protected:
    Dialog();
    
    CC_SYNTHESIZE(string, _userString, UserString);
    
private:
    Layout*             _root;
    Button*             _left;
    Button*             _right;
    Text*               _text;
    ccDialogCallback    _callback;
    int                 _userData;
    string              _leftText;
    string              _rightText;
    float               _oldX;
    float               _newX;
};

#endif /* defined(__ColorBook__Dialog__) */
