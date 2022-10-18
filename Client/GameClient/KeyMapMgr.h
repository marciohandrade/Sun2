//-------------------------------------------------------------------------------------------
/**
	KeyMapMgr 

	@desc
		Ű ���� �����ؼ� �⺻ Ű ������ �����ص�.

	@todo
		-> KeyInfoParser�� �̵��ؾ���.
*/


#pragma once
               
class KeyMapMgr : public util::Singleton<KeyMapMgr>
{
public:
    KeyMapMgr(void);
    ~KeyMapMgr(void);

    void                Init();
    
    KEYSETCODE          GetCode(util::_tstring strKeySet);
    const util::_tstring & GetKeyString(KEYSETCODE code);
	
	KEY_SET_ITR			GetBegin()	{ return m_KeyMap.begin(); }
	KEY_SET_ITR			GetEnd()	{ return m_KeyMap.end(); }

private:
    KEY_SET_MAP     m_KeyMap;
};
