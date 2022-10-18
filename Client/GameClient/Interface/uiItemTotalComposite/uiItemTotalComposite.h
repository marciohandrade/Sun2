#ifndef _uiItemTotalComposite_h_
#define _uiItemTotalComposite_h_

#include "uiBase.h"
#include "SCItemSlotContainer.h"
#include "uiItemTotalComposite_def.h"
#include "ItemComposeListParser.h"
#include "uiitemcompositeman/uiItemCompositeMan.h"
#include "globalfunc.h"
#include "InterfaceManager.h"
#include "ItemComposeListParser.h"
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
#include "TreeMenuGroup.h"
#include "TreeController.h"
#endif

const int MATRIAL_MAX = 6;
const int MAPCODE_MAX = sITEM_COMPOSE_LIST::MAX_COMPOSE_MAP_NUM;

enum eDIALOG_POS
{
	DIALOG_PIC_SLOT1 = 0,
	DIALOG_PIC_SLOT2,
	DIALOG_PIC_SLOT3,
	DIALOG_PIC_SLOT4,
	DIALOG_PIC_SLOT5,
	DIALOG_PIC_SLOT6,
	DIALOG_PIC_RESULT,
	DIALOG_TXT_CAPTION,
	DIALOG_TXT_CUR_CATEGORY,
	DIALOG_TXT_TOTAL_NUM,
	DIALOG_TXT_MANUAL_NUM,
	DIALOG_TXT_CHARGE,
	DIALOG_TXT_RESULT_ITEM,
	DIALOG_TXT_SLOT1,
	DIALOG_TXT_SLOT2,
	DIALOG_TXT_SLOT3,
	DIALOG_TXT_SLOT4,
	DIALOG_TXT_SLOT5,
	DIALOG_TXT_SLOT6,
    DIALOG_TXT_SLOT7,
	DIALOG_BTN_EXIT,
	DIALOG_BTN_LIST_ON,
	DIALOG_BTN_ALL_CREATE,
	DIALOG_BTN_MANUAL_CREATE,
	DIALOG_BTN_LEFT,
	DIALOG_BTN_RIGHT,
	DIALOG_BTN_CANCLE,
	DIALOG_SCROLBAR,
	DIALOG_LIST_EACH_CATEGORY,
	DIALOG_LIST_CATEGORY,
#ifdef _NA_000000_20130114_RENEWER_UI
	DIALOG_PICTURE_DRAPDOWN,
	DIALOG_SCROLBAR_DRAPDOWN,
#endif

	DIALOG_MAX,
};

enum 
{
	TXT_CAPTION = 0,
	TXT_CUR_CATEGORY,
	TXT_TOTAL_NUM,
	TXT_MANUAL_NUM,
	TXT_CHARGE,
	TXT_RESULT_ITEM,
};
enum
{
	MATERIAL_NAME_BEGIN = DIALOG_TXT_SLOT1,
	MATERIAL_NAME_SIZE  = DIALOG_TXT_SLOT6 - DIALOG_TXT_SLOT1 + 1,
	STATIC_CONTROL_SIZE = DIALOG_TXT_RESULT_ITEM  - DIALOG_TXT_CAPTION +1,
	SLOT_SIZE			= DIALOG_PIC_RESULT - DIALOG_PIC_SLOT1 +1,
	SLOT_BEGIN			= DIALOG_PIC_SLOT1,
};

struct BaseCategory
{
	int			iStringCode;
	int			iCode;
	MAPCODE		arryMapCode[MAPCODE_MAX];
};

struct SubCategory
{
	int		index;
	int		iStringCode;
	vector<BaseCategory> vec_baseCat;
};

struct MainCategory
{
	int		iStringCode;
	int		index;
	vector<SubCategory> vec_subCat;
};
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
struct TreeCategory
{
    TreeCategory()
    {
       childTree.clear(); 
       PerentItem = NULL;
       size = 0;
       depth = 0;
    }

    enum 
    {
        eType_Main,
        eType_Sun,
        eType_Item,
    };

    int		iStringCode;
    int		index;
    int     perentidx;
    int     CompositionCode;
    int     type;
    int     size;
    int     depth;

    MAPCODE		arryMapCode[MAPCODE_MAX];
    std::vector<TreeCategory*> childTree;
    TreeCategory*             PerentItem;

    void Release()
    {
        for(size_t i = 0 ; i < childTree.size() ; ++i)
        {
            childTree[i]->Release();

            delete childTree[i];
            childTree[i] = NULL;
        }

        childTree.clear();
    }

    void AddChild(TreeCategory* Item)
    {
        Item->depth = depth + 1;//해당 아이템 깊이는 부모 + 1
        Item->PerentItem = this;
        Item->perentidx = perentidx;

        childTree.push_back(Item);

        size = (int)childTree.size();
    }

    //하위 트리 전체 검색
    TreeCategory* GetTreeCategoryForChild(int idx)
    {
        if(index == idx)
        {
            return this;
        }

        for(size_t i = 0 ; i < childTree.size() ; ++i)
        {
            TreeCategory* tree = GetTreeCategory(i);
            if(tree)
            {
                return tree;
            }
        }

        return NULL;
    }

    //하위 단계만 검사 - 트리 전체 검사 안함
    TreeCategory* GetTreeCategory(int idx)
    {
        for(size_t i = 0 ; i < childTree.size() ; ++i)
        {
            TreeCategory* tree = childTree[i];
            if(tree->index == idx)
            {
                return tree;
            }
        }

        return NULL;
    }

    bool isMaxDepth()
    {
        return childTree.empty() ? true : false;
    }
};
#endif

class uiItemCompositeMan;

class uiItemTotalcomposite :
	public uiBase, public SCItemSlotContainer
{

public:
	enum
	{
		eValue_None = 0,
		eValue_Plus,
		eValue_Minus,
	};
public:
	uiItemTotalcomposite(InterfaceManager *pUIMan);
	~uiItemTotalcomposite();

//--------------------------------------------------------------------------
// init/release

public:
	virtual void            Init(CDrawBase* pDrawBase);
	virtual void            Release();
	void                    InitControls();

//--------------------------------------------------------------------------
// dialog functions
//
public:
	uiItemCompositeMan*     GetManager();
protected:
	virtual void	        OnShowWindow( BOOL val );
	virtual void            OnRenderSolarDialog();
	virtual void            OnUpdateSolarDialog();
	CControlWZ *	        getControl( POSTYPE AtPos );
	POSTYPE			        getCtrlIDToPos( WzID wzId );

protected:
	static WzID		        m_wzId[DIALOG_MAX];
	
//--------------------------------------------------------------------------
// functions
//
public:
	BOOL					SetNeedHaim( MONEY Money );
	void					updateInfo(BOOL bInitCompositeNum =TRUE);
	int						GetCurrentSelectItemCode() {return m_iCurSelectedItemCode;}
	ItemUnitRenderer *		GetItemUnitRender() const;
	BOOL					AppendMat( POSTYPE AtPos, sITEM_COMPOSE_LIST::sITEM_FOR_COMPOSE CompositeInfo,BOOL bExist);
	BOOL					AppendResultMat( POSTYPE AtPos,  BOOL bExist);

protected:
	void					updateMatInfo();
	void					updateAllInfomation();
	void					updateMouseWheel();

	void					IncreaseListCategoryScroll();
	void					DecreaseListCategoryScroll();
	BOOL					CheckItemExistValidMap(int subID, int baseID);

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & IN rSlot );									// 슬롯에 객체 추가
	BOOL									DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );
	void					ClearInfo();
	BOOL					SetCurrentSelectedItemCode();
	void					SetItemSubInfo();

//--------------------------------------------------------------------------
// Make Tree function
//
protected:
	void					MakeTree();
	void					UpdateTree();
	void					UpdateTree2();
	void					ComposeCategory(sITEM_COMPOSE_LIST * pItem); 
	void					InsertListData(CCtrlListWZ* pList, TCHAR* message,int* extraData = 0, BOOL bCenter = TRUE);

private:
	std::vector<MainCategory>	vMainCategory;

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    std::vector<TreeCategory*>   vMainTreeCategory;
    TreeController*              Tree_Controller_Ptr_;
#endif

	int						m_iMainCategory;
	int						m_iSubCategory;


//--------------------------------------------------------------------------
// procedure
//
public:
	virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure
	virtual VOID			MouseOver_MsgProc( SI_MESSAGE * pMessage );

private:
	// controll
	CCtrlStaticWZ*			m_pStatic[6];
	CCtrlStaticWZ*			m_pNeedHeim;
	CCtrlStaticWZ*			m_pMaterialName[MATERIAL_NAME_SIZE];
	CCtrlVScrollWZ*			m_pVScroll;
#ifdef _NA_000000_20130114_RENEWER_UI
	CCtrlPictureWZ*			m_pImageDrapDown;
	CCtrlVScrollWZ*			m_pVScrollDrapDown;
#endif

	CCtrlListWZ*			m_pList_Each_Category;
	CCtrlListWZ*			m_pList_Category;
    CCtrlStaticWZ*			m_pCreateRate;
	// variable	
	int						m_iCurSelectedItemCode;
	ItemUnitRenderer *		m_pTotalItemUnitRenderer;
	TCHAR					m_szText[INTERFACE_STRING_LENGTH];
	TCHAR					m_szResult[INTERFACE_STRING_LENGTH];
	int						m_iMaxScrollValue;
	int						m_iCurScrollValue;

	int						m_iMaxScrollValue2;
	int						m_iCurScrollValue2;

    BYTE                    m_byCreateRate;
#ifdef _NA_000000_20130114_RENEWER_UI
	int						m_iDropdownMaxScrollValue;
	int						m_iDropdownCurScrollValue;
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

	bool					is_click_Btn_;
	BYTE					is_Set_Plus_Min_;
	CTimerSimple			Update_Counter_timer_;

#ifdef _SCJ_LONG_TEXT_REDUCING
    typedef std::pair<DWORD, std::string> MaterialNamePair;
    typedef std::vector<MaterialNamePair> MaterialNameList;
    MaterialNameList material_name_tooltip_list_;
#endif //_SCJ_LONG_TEXT_REDUCING

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER

    void UpdateTreeReViewer();
    void MakeCompoisiteTreeitem(HTREEITEM perentItem,TreeCategory* Item);
    void AddSubTreeItem(sITEM_COMPOSE_LIST * pItemComposeList,TreeCategory* SubTree,int Count,int incCount);
#endif


};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiItemTotalcomposite::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
	{
		
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return DIALOG_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiItemTotalcomposite::getControl( POSTYPE AtPos )
{
	ASSERT( AtPos < DIALOG_MAX );

	return GetControlWZ( m_wzId[AtPos] );
}


#endif // _uiItemTotalComposite_h_
