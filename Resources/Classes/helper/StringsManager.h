//
//  StringsManager.hpp
//  ALABS0010
//
//  Created by luoxiaopeng on 16/3/21.
//
//

#ifndef StringsManager_hpp
#define StringsManager_hpp

#include "UIHeader.h"


#define xTrans(name)    (StringsManager::getInstance()->translate(_root, name))
#define xTransN(name)    (StringsManager::getInstance()->translateNormal(_root, name))
#define xTransB(name)    (StringsManager::getInstance()->translateBold(_root, name))
#define xTransH(name)    (StringsManager::getInstance()->translateHel(_root, name))
#define xStr            (StringsManager::getInstance())

class StringsManager
{
public:
    static StringsManager* getInstance();
    void destroyInstance();
    
    void translate(Widget* root, const string &name);
    void translateNormal(Widget* root, const string &name);
    void translateBold(Widget* root, const string &name);
    void translateHel(Widget* root, const string &name);
    string getString(const string& name);
    string getStringDefault(const string& name);
    
    const string & getDialogFontName();
    const string & getBoldFontName();

protected:
    StringsManager();
    
protected:
    ValueMap    _strings;
    string       _dialogFontName;
    string       _normalFontName;
    string       _boldFontName;
};

#endif /* StringsManager_hpp */
