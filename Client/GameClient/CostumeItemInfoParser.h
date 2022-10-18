#pragma once

#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
class cCostumeItemInfo
{
public:
	cCostumeItemInfo() {}


	void SetItemCode( SLOTCODE ItemCode ) { m_ItemCode = ItemCode; }
	SLOTCODE GetItemCode() { return m_ItemCode; }

	void SetIdleAnimation( WzID MaleAnimation, WzID FemaleAnimation ) { m_MaleAnimation = MaleAnimation; m_FemaleAnimation = FemaleAnimation; }
	WzID GetIdleAnimation( BOOL IsMale ) { return IsMale ? m_MaleAnimation : m_FemaleAnimation; }


private:
	SLOTCODE m_ItemCode;
	WzID m_MaleAnimation;
	WzID m_FemaleAnimation;
};


class cCostumeItemInfoParser	: public Singleton<cCostumeItemInfoParser>, public IParser
{
public:
    cCostumeItemInfoParser();
    ~cCostumeItemInfoParser();


    VOID                    Init(DWORD dwPoolSize);
    VOID			        Init(DWORD dwPoolSize, char * pszPackFileName);
    VOID                    Release();
    virtual	BOOL            LoadScript(eSCRIPT_CODE ScriptCode, BOOL bReload);

    cCostumeItemInfo*  GetInfo( SLOTCODE ItemCode );

private:
    VOID		            Unload();
    util::SolarHashTable<cCostumeItemInfo*>*    m_pDataTable;
};

#endif //_NA_COSTUMEITEM_IDLE_ANIMATION