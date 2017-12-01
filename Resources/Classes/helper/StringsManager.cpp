//
//  StringsManager.cpp
//  ALABS0010
//
//  Created by luoxiaopeng on 16/3/21.
//
//

#include "StringsManager.h"
#include "UICpp.h"


static StringsManager *_sharedString = nullptr;

StringsManager* StringsManager::getInstance()
{
    if (! _sharedString)
    {
        _sharedString = new StringsManager();
    }
    
    return _sharedString;
}

void StringsManager::destroyInstance()
{
    CC_SAFE_DELETE(_sharedString);
}

StringsManager::StringsManager()
{
    LanguageType currentLanguageType = Application::getInstance()->getCurrentLanguage();
    switch (currentLanguageType)
    {
        case LanguageType::ENGLISH:
//            _dialogFontName = "fonts/copyfonts.com_gotham-medium.ttf";
//            _normalFontName = "fonts/Seravek-ExtraLight.ttf";
//            _boldFontName = "fonts/ArbutusSlab-Regular.ttf";
            _dialogFontName = "res/kalinga.ttf";
            _normalFontName = "res/kalinga.ttf";
            _boldFontName = "res/kalinga.ttf";
            break;
        case LanguageType::CHINESE:  // only for test
//            _dialogFontName = "fonts/copyfonts.com_gotham-medium.ttf";
//            _normalFontName = "fonts/Seravek-ExtraLight.ttf";
//            _boldFontName = "fonts/ArbutusSlab-Regular.ttf";
            _dialogFontName = "res/kalinga.ttf";
            _normalFontName = "res/kalinga.ttf";
            _boldFontName = "res/kalinga.ttf";
            break;
        default:
            _dialogFontName = "res/kalinga.ttf";
            _normalFontName = "res/kalinga.ttf";
            _boldFontName = "res/kalinga.ttf";
            //CCASSERT(false, "Invalid language type.");
            break;
    }
    
    _strings = FileUtils::getInstance()->getValueMapFromFile("strings.xml");
}

void StringsManager::translate(Widget* root, const string &name)
{
    Text* text = (Text*)Helper::seekNodeByName(root, name);
    CCASSERT(_strings.find(name) != _strings.end(), "error");
    text->setString(_strings[name].asString());
}

void StringsManager::translateNormal(Widget* root, const string &name)
{
    Text* text = (Text*)Helper::seekNodeByName(root, name);
    CCASSERT(_strings.find(name) != _strings.end(), "error");
    text->setFontName(_normalFontName);
    text->setString(_strings[name].asString());
    
}

void StringsManager::translateBold(Widget* root, const string &name)
{
    Text* text = (Text*)Helper::seekNodeByName(root, name);
    CCASSERT(_strings.find(name) != _strings.end(), "error");
    text->setFontName(_boldFontName);
    text->setString(_strings[name].asString());
}

void StringsManager::translateHel(Widget* root, const string &name)
{
    Text* text = (Text*)Helper::seekNodeByName(root, name);
    CCASSERT(_strings.find(name) != _strings.end(), "error");
    text->setFontName("fonts/copyfonts.com_gotham-medium.ttf");
    text->setString(_strings[name].asString());
}

string StringsManager::getString(const string& name)
{
    return std::move(_strings[name].asString());
}

string StringsManager::getStringDefault(const string& name)
{
    string key = "TXT_" + name;
    if (_strings.find(key) != _strings.end()) {
        return std::move(_strings[key].asString());
    }
    else
    {
        return name;
    }
}

const string & StringsManager::getDialogFontName()
{
    return _dialogFontName;
}

const string & StringsManager::getBoldFontName()
{
    return _boldFontName;
}

