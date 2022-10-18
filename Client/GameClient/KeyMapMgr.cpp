#include "SunClientPrecompiledHeader.h"
#include "keymapmgr.h"
#include <dinput.h>

KeyMapMgr::KeyMapMgr(void)
{
	Init();
}

KeyMapMgr::~KeyMapMgr(void)
{
    this->m_KeyMap.clear();
}


void 
KeyMapMgr::Init()
{
	m_KeyMap.insert(KEY_SET_PAIR( DIK_ESCAPE,          _T("ESCAPE"			)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_1,               _T("NUM1" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_2,               _T("NUM2" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_3,			   _T("NUM3" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_4,               _T("NUM4" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_5,               _T("NUM5" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_6,               _T("NUM6" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_7,               _T("NUM7" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_8,               _T("NUM8" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_9,               _T("NUM9" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_0,               _T("NUM0" 				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_MINUS,           _T("MINUS"			)));
#ifdef	_JAPAN
	m_KeyMap.insert(KEY_SET_PAIR( DIK_CIRCUMFLEX,      _T("EQUALS" 		)));
#else
	m_KeyMap.insert(KEY_SET_PAIR( DIK_EQUALS,          _T("EQUALS" 		)));
#endif
	m_KeyMap.insert(KEY_SET_PAIR( DIK_BACK,            _T("BACK"   		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_TAB,             _T("TAB"    		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_Q,               _T("Q"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_W,               _T("W"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_E,               _T("E"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_R,               _T("R"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_T,               _T("T"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_Y,               _T("Y"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_U,               _T("U"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_I,               _T("I"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_O,               _T("O"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_P,               _T("P"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_LBRACKET,        _T("LBRACKET"	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_RBRACKET,        _T("RBRACKET"	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_RETURN,          _T("ENTER"   	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_LCONTROL,        _T("CTRL"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_A,               _T("A"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_S,               _T("S"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_D,               _T("D"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F,               _T("F"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_G,               _T("G"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_H,               _T("H"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_J,               _T("J"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_K,               _T("K"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_L,               _T("L"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_SEMICOLON,       _T("SEMICOLON"  	)));
#ifdef	_JAPAN
	m_KeyMap.insert(KEY_SET_PAIR( DIK_COLON,		   _T("APOSTROPHE" 	)));
#else
	m_KeyMap.insert(KEY_SET_PAIR( DIK_APOSTROPHE,      _T("APOSTROPHE" 	)));
#endif
	m_KeyMap.insert(KEY_SET_PAIR( DIK_GRAVE,           _T("GRAVE"      	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_LSHIFT,          _T("SHIFT"		)));
#ifdef	_JAPAN
	m_KeyMap.insert(KEY_SET_PAIR( DIK_YEN,			    _T("BACKSLASH" 	)));
#else
	m_KeyMap.insert(KEY_SET_PAIR( DIK_BACKSLASH,       _T("BACKSLASH"  	)));
#endif

	m_KeyMap.insert(KEY_SET_PAIR( DIK_Z,               _T("Z"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_X,               _T("X"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_C,               _T("C"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_V,               _T("V"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_B,               _T("B"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_N,               _T("N"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_M,               _T("M"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_COMMA,           _T("COMMA"			)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_PERIOD,          _T("PERIOD"			)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_SLASH,           _T("SLASH"			)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_RSHIFT,          _T("SHIFT"			)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_MULTIPLY,        _T("MULTIPLY"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_LMENU,           _T("ALT"			)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_SPACE,           _T("SPACE"			)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_CAPITAL,         _T("CAPITAL"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F1,              _T("F1"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F2,              _T("F2"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F3,              _T("F3"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F4,              _T("F4"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F5,              _T("F5"				)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F6,              _T("F6"       		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F7,              _T("F7"       		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F8,              _T("F8"       		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F9,              _T("F9"       		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F10,             _T("F10"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMLOCK,         _T("NUMLOCK"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_SCROLL,          _T("SCROLL"   		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD7,         _T("NUMPAD7"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD8,         _T("NUMPAD8"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD9,         _T("NUMPAD9"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_SUBTRACT,        _T("SUBTRACT" 		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD4,         _T("NUMPAD4"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD5,         _T("NUMPAD5"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD6,         _T("NUMPAD6"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_ADD,             _T("ADD"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD1,         _T("NUMPAD1"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD2,         _T("NUMPAD2"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD3,         _T("NUMPAD3"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPAD0,         _T("NUMPAD0"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_DECIMAL,         _T("DECIMAL"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_OEM_102,         _T("OEM_102"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F11,             _T("F11"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F12,             _T("F12"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F13,             _T("F13"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F14,             _T("F14"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_F15,             _T("F15"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_KANA,            _T("KANA"     		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_ABNT_C1,         _T("ABNT_C1"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_CONVERT,         _T("CONVERT"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NOCONVERT,       _T("NOCONVERT"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_YEN,             _T("YEN"      		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_ABNT_C2,         _T("ABNT_C2"  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPADEQUALS,    _T("NUMPADEQUALS"	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_PREVTRACK,       _T("PREVTRACK"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_AT,              _T("AT"       		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_COLON,           _T("COLON"	  		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_UNDERLINE,       _T("UNDERLINE"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_KANJI,           _T("KANJI"    		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_STOP,            _T("STOP"     		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_AX,              _T("AX"       		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_UNLABELED,       _T("UNLABELED"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NEXTTRACK,       _T("NEXTTRACK"		)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPADENTER,     _T("NUMPADENTER"	)));	  
	m_KeyMap.insert(KEY_SET_PAIR( DIK_RCONTROL,        _T("CONTROL"		)));	  
	m_KeyMap.insert(KEY_SET_PAIR( DIK_MUTE,            _T("MUTE"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_CALCULATOR,      _T("CALCULATOR"  	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_PLAYPAUSE,       _T("PLAYPAUSE"   	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_MEDIASTOP,       _T("MEDIASTOP"   	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_VOLUMEDOWN,      _T("VOLUMEDOWN"  	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_VOLUMEUP,        _T("VOLUMEUP"    	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WEBHOME,         _T("WEBHOME"     	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NUMPADCOMMA,     _T("NUMPADCOMMA" 	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_DIVIDE,          _T("DIVIDE"      	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_SYSRQ,           _T("SYSRQ"       	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_RMENU,           _T("ALT"			)));	  
	m_KeyMap.insert(KEY_SET_PAIR( DIK_PAUSE,           _T("PAUSE"       	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_HOME,            _T("HOME"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_UP,              _T("UP"          	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_PRIOR,           _T("PRIOR"		 	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_LEFT,            _T("LEFT"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_RIGHT,           _T("RIGHT"       	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_END,             _T("END"         	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_DOWN,            _T("DOWN"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_NEXT,            _T("NEXT"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_INSERT,          _T("INSERT"      	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_DELETE,          _T("DELETE"      	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_LWIN,            _T("LWIN"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_RWIN,            _T("RWIN"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_APPS,            _T("APPS"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_POWER,           _T("POWER"       	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_SLEEP,           _T("SLEEP"       	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WAKE,            _T("WAKE"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WEBSEARCH,       _T("WEBSEARCH"	 	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WEBFAVORITES,    _T("WEBFAVORITES"	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WEBREFRESH,      _T("WEBREFRESH"  	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WEBSTOP,         _T("WEBSTOP"     	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WEBFORWARD,      _T("WEBFORWARD"  	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_WEBBACK,         _T("WEBBACK"     	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_MYCOMPUTER,      _T("MYCOMPUTER"  	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_MAIL,            _T("MAIL"        	)));
	m_KeyMap.insert(KEY_SET_PAIR( DIK_MEDIASELECT,     _T("MEDIASELECT" 	)));
}


KEYSETCODE      
KeyMapMgr::GetCode(util::_tstring strKeySet)
{
    KEY_SET_ITR itr = this->m_KeyMap.begin();

    for (;itr != this->m_KeyMap.end(); ++itr)
    {
        if ( !Strcmp(itr->second.c_str(), strKeySet.c_str()) )
        {
            return (KEYSETCODE)itr->first;
        }
    }

	return 0;
}


const util::_tstring &     
KeyMapMgr::GetKeyString(KEYSETCODE code)
{
    KEY_SET_ITR itr = this->m_KeyMap.find(code);

    if ( itr  != this->m_KeyMap.end() )
    {
        return itr->second;
    }

    assert(0);

	static util::_tstring strTemp = _T("");

    return strTemp;
}
