#include "Config.h"

#define CONFIG_ITEM(__type, __name, __default)	\
  __type Config::_##__name = __default;

#define KEY_BIND(__name, __p1, __p2, __p3, __p4)	\
  int Config::_key##__name[4] = {		\
    (int)cocos2d::EventKeyboard::KeyCode::KEY_##__p1,	\
    (int)cocos2d::EventKeyboard::KeyCode::KEY_##__p2,	\
    (int)cocos2d::EventKeyboard::KeyCode::KEY_##__p3,	\
    (int)cocos2d::EventKeyboard::KeyCode::KEY_##__P4,	\
  };

#include "ConfigList.txt"

#undef CONFIG_ITEM
#undef KEY_BIND
