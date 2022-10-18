/*!
 *	\file		uiBaseMan.h file
 *	\brief		uiBaseMan class
 *	\author		���ǥ
 *	\version	2009/02/02	���ǥ (lammy@webzen.co.kr) doxygen command �߰�
 */
#ifndef     __USER_INTERFACE_BASE_MANAGER_H__
#define     __USER_INTERFACE_BASE_MANAGER_H__

#include "uiBase_def.h"
#include "uiBaseMan_def.h"
#include "InterfaceManager.h"
#include <set>

#pragma warning(disable : 4995)

/*!
 *	\class		uiBaseMan class
 *	\brief		���� ������ �޴����� �����Ѵ�.
 *	\author		���ǥ
 */
class uiBaseMan
{
	/// ������ ���̵� ����ϱ� ���� set.	
	typedef	std::set<DWORD>					DIALOG_ID_SET_TYPE;
	typedef	DIALOG_ID_SET_TYPE::iterator	DIALOG_ID_SET_ITR_TYPE;
	
public:
    uiBaseMan(InterfaceManager *pUIMan);
    virtual ~uiBaseMan();

    //--------------------------------------------------------------------------
    // init/release
    //

public:
    BOOL Init( CDrawBase* pDrawBase );
    void Release();
    void Reset();

    virtual void OnInitialize() {}
    virtual void OnRelease() {}
    virtual void OnReset() {}

    virtual void            Update();
	virtual void            Process(DWORD dwTick);
	virtual BOOL			UnLoadUIAll( void );
    virtual void            InitPKBooleans();

	BOOL					ReLoadUI( DWORD dwKey );	
	BOOL					UnLoadUI( DWORD dwKey );

    int                     TimeOutPKBooleans();    // check to timeout packets
    virtual void            HandlerPKBooleans();    // handling for lost or wrong way packets
                                                    // it's process to own each derived classes.
                                                    // may not defined that system has called to warning message by default

	virtual void			PreRenderInterface( DWORD /*dwTick*/ ) {};	
	virtual void			PostRenderInterface( DWORD /*dwTick*/ ) {};

	BOOL					InsertDialogKey( DWORD dwKey );

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    InterfaceManager *      GetUIMan();
    CDrawBase *             GetDrawBase();


    //--------------------------------------------------------------------------
    // functions
	//
public:
    void                    SetPKBool(int eIdx, DWORD dwTimeOut=0);
    void                    UnsetPKBool(int eIdx);
    bool                    GetPKBool(int eIdx);
    inline bool             SafePKBool(int eIdx)
    {
        return !(GetPKBool(eIdx));
    }
	bool					IsPKPending();

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );
    virtual void            NetworkProcWorld( MSG_BASE * pMsg );

    //--------------------------------------------------------------------------
    // member vars
    //
protected:
    std::vector<uicPacketControl>   m_vPKBool;

private:
    InterfaceManager *      m_pUIMan;         // must required
    CDrawBase *				m_pDraw;	
	///	������ �ÿ� �ڵ����� ���� ���̵� ����Ʈ�� �����Ѵ�.
	DIALOG_ID_SET_TYPE		m_DialogIdList;
};

//------------------------------------------------------------------------------
/**
*/
inline
InterfaceManager *
uiBaseMan::GetUIMan()
{
    assert(this->m_pUIMan);
    return this->m_pUIMan;
}

/*!
 *	�����̸� �����Ѵ�.
 *	\param[in]	 dwDialogKey	���� ���̾�α� Ű
 *	\param[in]	 pFileName		�ε��� ���ҽ� �̸�
 *	\param[in]	 pMan			����� �޴���
 *	\param[in]	 bLoad			�ٷ� �ε������� ����	
 *	\param[in]	 bClone			���纻�� �������� ����
 */
template<class T>
T* CreateUIDialog( DWORD dwDialogKey, const char* pFileName, uiBaseMan * pMan, BOOL bLoad = FALSE, BOOL bClone = FALSE, BOOL bDynamicWindow = FALSE )
{
#ifdef _DEV_VER
    const char* id_string = WzIDToString( dwDialogKey );
    int id_number = atoi(id_string);
#endif // _DEV_VER

	T * p = GET_CAST_DIALOG( T, dwDialogKey );

	if( p != NULL )
		return p;

	T* pUI = new T( pMan->GetUIMan() );	
    pUI->SetDialogKey( dwDialogKey );
    pUI->SetDialogResourceName( pFileName );

	pUI->SetMan( pMan );
	pUI->SetClone( bClone );

	if(TRUE == bLoad || TRUE == bDynamicWindow)
	{
		if (pUI->LoadInterface() == false)
        {
            SAFE_DELETE(pUI);
			return NULL;
        }
	}
	else
	{
		if (pUI->GetUIMan()->RegisterDialog(dwDialogKey, pUI) == FALSE)
        {
            SAFE_DELETE(pUI);
			return NULL;	
        }
	}

	if (bDynamicWindow)
	{
		pUI->SetDynamicWindow(TRUE);
	}
	else
	{
		pMan->InsertDialogKey( dwDialogKey );
	}	

	return pUI;
}

//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_BASE_MANAGER_H__
