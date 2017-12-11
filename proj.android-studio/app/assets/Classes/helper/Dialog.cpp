//
//  Dialog.cpp
//  ColorBook
//
//
//

#include "Dialog.h"
#include "UITools.h"
#include "UICpp.h"
#include "StringsManager.h"


static Dialog *_sharedDialog = nullptr;

Dialog* Dialog::getInstance()
{
    if (! _sharedDialog)
    {
        _sharedDialog = new Dialog();
    }
    
    return _sharedDialog;
}

void Dialog::destroyInstance()
{
    CC_SAFE_DELETE(_sharedDialog);
}

Dialog::Dialog()
{
    _root = (Layout*)GUIReader::getInstance()->widgetFromJsonFile("dialog.json");
    CC_SAFE_RETAIN(_root);
    _root->cocos2d::Node::visit();
    
    _left = quickButton("left", _root, CC_CALLBACK_2(Dialog::onButton, this));
    _right = quickButton("right", _root, CC_CALLBACK_2(Dialog::onButton, this));
    
    _left->setPositionY(_left->getPositionY() - 5);
    _right->setPositionY(_right->getPositionY() - 5);
    
    _text = (Text*)Helper::seekNodeByName(_root, "text");
    
    _left->setTitleFontName(xStr->getDialogFontName());
    _right->setTitleFontName(xStr->getDialogFontName());
    _text->setFontName(xStr->getDialogFontName());
    
    _oldX = _right->getPositionX();
    _newX = _left->getPositionX() + (_right->getPositionX() - _left->getPositionX()) / 2;

    _root->addClickEventListener(CC_CALLBACK_1(Dialog::onAction, this));
}

void Dialog::onAction(Ref* ref)
{
    if(_root->getParent() != nullptr && _callback != nullptr)
    {
//        _callback("notClickDialog");
        _root->removeFromParent();
    }
}

Dialog::~Dialog()
{
    CC_SAFE_RELEASE(_root);
}

void Dialog::show(const string& text, const string& button, const ccDialogCallback &callback, int fontSize /* =-1 */)
{
    show(text, "", button, callback, fontSize);
}

void Dialog::show(const string& text, const string& left, const string& right, const ccDialogCallback &callback, int fontSize /* =-1 */)
{
    if(_root->getParent() == nullptr)
    {
        _callback = callback;
        
        _text->setString(xStr->getString("MSG_" + text));
        if (fontSize > 0)
        {
            _text->setFontSize(fontSize);
        }
        
        if (!left.empty()) {
            _left->setVisible(true);
            _left->setTitleText(xStr->getString("BTN_" + left));
            _right->setTitleText(xStr->getString("BTN_" + right));
            _right->setPositionX(_oldX);
            quickSetImage("bg", _root, "dialog/popup.png");
        }
        else
        {
            _left->setVisible(false);
            _right->setTitleText(xStr->getString("BTN_" + right));
            _right->setPositionX(_newX);
            quickSetImage("bg", _root, "other/popup_1.png");
        }
        
        _leftText = left;
        _rightText = right;
        
        Director::getInstance()->getRunningScene()->addChild(_root, 100);
    }
}

void Dialog::onButton(Ref* pSender, Widget::TouchEventType type)
{
    if (type != Widget::TouchEventType::ENDED) return;
    
    string text = _leftText;
    if (pSender == _right) {
        text = _rightText;
    }
    
    _root->removeFromParent();
    
    if(_callback)
        _callback(text);
}

void Dialog::setUserData(int userData)
{
    _userData = userData;
}

int Dialog::getUserData()
{
    return _userData;
}

