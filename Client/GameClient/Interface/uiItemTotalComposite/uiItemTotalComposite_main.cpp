#include "SunClientPrecompiledHeader.h"

#include "uiItemTotalComposite.h"
#include "ItemUnitRenderer.h"
#include "map.h"



//------------------------------------------------------------------------------
/**
*/
uiItemTotalcomposite::uiItemTotalcomposite(InterfaceManager *pUIMan):
uiBase(pUIMan),
m_iMainCategory(0),
m_iSubCategory(0),
m_iCurSelectedItemCode(0),
is_click_Btn_(false),
is_Set_Plus_Min_(eValue_None)
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
,Tree_Controller_Ptr_(NULL)
#endif
{
	SCItemSlotContainer::Init(SLOT_SIZE, SI_ITEM_COMPOSE);
	m_iMaxScrollValue = 0;
	m_iCurScrollValue = 0;
	m_iMaxScrollValue2 = 0;
	m_iCurScrollValue2 = 0;

	m_pTotalItemUnitRenderer = NULL;
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
	m_iDropdownMaxScrollValue = 0;
	m_iDropdownCurScrollValue = 0;
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
}




//------------------------------------------------------------------------------
/**
*/
uiItemTotalcomposite::~uiItemTotalcomposite()
{
	this->Release();

	SCItemSlotContainer::Release();
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    if(Tree_Controller_Ptr_)
    {
        delete Tree_Controller_Ptr_;
        Tree_Controller_Ptr_ = NULL;
    }
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
}

//------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::Init(CDrawBase* pDrawBase)
{
	for(int i = 0 ; i < STATIC_CONTROL_SIZE ; ++i)
	{
		m_pStatic[i] = static_cast<CCtrlStaticWZ*>( getControl( i + DIALOG_TXT_CAPTION ) );
	}

	for(int i = 0; i <  MATERIAL_NAME_SIZE; ++i)
	{
        m_pMaterialName[i] = static_cast<CCtrlStaticWZ*>(getControl(i + MATERIAL_NAME_BEGIN));
#ifdef _SCJ_LONG_TEXT_REDUCING
        m_pMaterialName[i]->SetTextReducing(true);
#endif //_SCJ_LONG_TEXT_REDUCING
    }

#ifdef _SCJ_LONG_TEXT_REDUCING
    material_name_tooltip_list_.reserve(MATERIAL_NAME_SIZE);
#endif //_SCJ_LONG_TEXT_REDUCING

    m_pCreateRate = static_cast<CCtrlStaticWZ*>(getControl(DIALOG_TXT_SLOT7));

	m_pList_Each_Category	= static_cast<CCtrlListWZ*>(getControl(DIALOG_LIST_EACH_CATEGORY));

	m_pList_Category		= static_cast<CCtrlListWZ*>(getControl(DIALOG_LIST_CATEGORY));
	m_pList_Category->SetStartLineGapTop( 6 );
#ifdef _NA_000000_20130114_RENEWER_UI
	m_pImageDrapDown		= static_cast<CCtrlPictureWZ*>(getControl(DIALOG_PICTURE_DRAPDOWN));
	m_pVScrollDrapDown		= static_cast<CCtrlVScrollWZ*>(getControl(DIALOG_SCROLBAR_DRAPDOWN));
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

#ifdef _JAPAN
    // 일본의 경우 조합 카테고리 항목이 많아 높이값 줄임
    // 일본에 UI리소스가 따로 들어가 있는 상황이으로 일본 디파인에 적용
    m_pList_Category->SetSellHeight(16);
#endif

	m_pVScroll				= static_cast<CCtrlVScrollWZ*>(getControl(DIALOG_SCROLBAR));
	
	m_pList_Category->ShowWindowWZ(WZ_HIDE);
#ifdef _NA_000000_20130114_RENEWER_UI
	m_pImageDrapDown->ShowWindowWZ(WZ_HIDE);
	m_pVScrollDrapDown->ShowWindowWZ(WZ_HIDE);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

	TCHAR szMessage[INTERFACE_STRING_LENGTH] ={0,} ;
	g_InterfaceManager.GetInterfaceString(eST_ITEM_COMPOSITE , szMessage , INTERFACE_STRING_LENGTH );
	GlobalFunc::SetCaption(m_pStatic[TXT_CAPTION],szMessage);
	m_pTotalItemUnitRenderer = new ItemUnitRenderer;
	m_pTotalItemUnitRenderer->Init( g_pSunRenderer, SCSlotContainer::GetMaxSlotNum() );

	m_iMainCategory = 0;
	m_iSubCategory	= 0;
    m_byCreateRate = 0;

	MakeTree();

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    m_pList_Each_Category->SetTextReducing(true);
    Tree_Controller_Ptr_ = TreeController::CreateTree(20,20,m_pVScroll,m_pList_Each_Category);
#endif

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
#else
    UpdateTree();	
#endif

	UpdateTree2();
}																


//------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::Release()
{
	for (int i = 0; i < GetMaxSlotNum(); ++i)
	{
		if ( !IsEmpty(i) )
			DeleteSlot(i, NULL);
	}

	if ( m_pTotalItemUnitRenderer )
	{
		this->m_pTotalItemUnitRenderer->Release();
		SAFE_DELETE(this->m_pTotalItemUnitRenderer);
	}

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    for (int i = 0 ; i < (int)vMainTreeCategory.size() ; ++i)
    {
        vMainTreeCategory[i]->Release();
        delete vMainTreeCategory[i];
        vMainTreeCategory[i] = NULL;
    }

    vMainTreeCategory.clear();

    

#endif

}


//-------------------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::MakeTree()
{
    ComposeCategotyMap& category_map = ItemComposeListParser::Instance()->GetComposeCategotyMap();
    ComposeCategotyMap::iterator categoty_iterator = category_map.begin();
    ComposeCategotyMap::iterator end_iterator = category_map.end();
    for ( ; categoty_iterator != end_iterator; ++categoty_iterator)
    {
        ComposeCategory(categoty_iterator->second);
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::ComposeCategory(sITEM_COMPOSE_LIST * pItemComposeList)
{
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER 
    //메인카운터
    int count = pItemComposeList->m_dCategoryCount;
    if(count == 0)
    {
        return;
    }

    TreeCategory* Category = NULL;

    bool isFine = false;
    for (int i = 0 ; i < (int)vMainTreeCategory.size() ; ++i)
    {
        //해당 아이템의 하위에 들어가야됨.
        if(pItemComposeList->m_pCategory[0] == vMainTreeCategory[i]->index)
        {
            Category = vMainTreeCategory[i];
            isFine = true;
            break;
        }
    }
    int IncCount = 1;

    //최초 셋팅은 아무거도 없으니 다 채우자
    if(!isFine)
    {
        Category = new TreeCategory();
        Category->perentidx = -1;
        Category->iStringCode = pItemComposeList->m_pCategoryCode[0];
        Category->index = pItemComposeList->m_pCategory[0];
        Category->type = TreeCategory::eType_Main;
        Category->CompositionCode = pItemComposeList->m_Code;
        AddSubTreeItem(pItemComposeList,Category,count,IncCount);

        vMainTreeCategory.push_back(Category);
    }
    else
    {
        AddSubTreeItem(pItemComposeList,Category,count,IncCount);
    }

#else
	BOOL bMainInsert = TRUE;
	int  iPendingMain = 0;
	BOOL bSubInsert = TRUE;
	int	 iPendingSub = 0 ;


	if(vMainCategory.size() == 0) bMainInsert = TRUE;
	for(unsigned int i =0; i < vMainCategory.size(); ++i)
	{
		if(vMainCategory[i].index == pItemComposeList->m_dwCategory1)
		{
			bMainInsert = FALSE;
			iPendingMain = i;
		}
	}

	if(bMainInsert)
	{
		BaseCategory base;
		base.iStringCode = pItemComposeList->m_NCode;
		base.iCode		 = pItemComposeList->m_Code;
		memcpy(base.arryMapCode,pItemComposeList->m_MapCode,sizeof(MAPCODE) * MAPCODE_MAX);

		SubCategory	sub;
		sub.index		= pItemComposeList->m_dwCategory2;
		sub.iStringCode	= pItemComposeList->m_Category2NCode;
		sub.vec_baseCat.clear();
		sub.vec_baseCat.push_back(base);

		MainCategory main;
		main.index		= pItemComposeList->m_dwCategory1;
		main.iStringCode = pItemComposeList->m_Category1NCode;
		main.vec_subCat.clear();
		main.vec_subCat.push_back(sub);

		vMainCategory.push_back(main);
	}
	else
	{
		if(vMainCategory[iPendingMain].vec_subCat.size() == 0)
			bSubInsert = TRUE;
		for(unsigned int i = 0; i < vMainCategory[iPendingMain].vec_subCat.size(); ++i)
		{
			if (vMainCategory[iPendingMain].vec_subCat[i].index == pItemComposeList->m_dwCategory2)
			{
				bSubInsert	 = FALSE;
				iPendingSub = i;
			}
		}

		if(bSubInsert)
		{
			BaseCategory base;
			base.iStringCode = pItemComposeList->m_NCode;
			base.iCode		 = pItemComposeList->m_Code;
			memcpy(base.arryMapCode,pItemComposeList->m_MapCode,sizeof(MAPCODE) * MAPCODE_MAX);
			SubCategory	sub;
			sub.index		= pItemComposeList->m_dwCategory2;
			sub.iStringCode	= pItemComposeList->m_Category2NCode;
			sub.vec_baseCat.push_back(base);

			vMainCategory[iPendingMain].vec_subCat.push_back(sub);
		}
		else
		{

			BaseCategory base;
			base.iStringCode = pItemComposeList->m_NCode;
			base.iCode		 = pItemComposeList->m_Code;
			memcpy(base.arryMapCode,pItemComposeList->m_MapCode,sizeof(MAPCODE) * MAPCODE_MAX);
			vMainCategory[iPendingMain].vec_subCat[iPendingSub].vec_baseCat.push_back(base);

		}
	}
#endif
}

//-------------------------------------------------------------------------------------------
/**
*/
#ifdef _NA_000000_20130114_RENEWER_UI
void uiItemTotalcomposite::UpdateTree2()
{
	m_pList_Category->DeleteItemAll();

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER
    for(unsigned int i = 0; i < vMainTreeCategory.size(); ++i)
    {
        g_InterfaceManager.GetInterfaceString(vMainTreeCategory[i]->iStringCode,szMessage,INTERFACE_STRING_LENGTH);
        InsertListData(m_pList_Category,szMessage,0,false);
    }
#else
	for(unsigned int i = 0; i < vMainCategory.size(); ++i)
	{
		g_InterfaceManager.GetInterfaceString(vMainCategory[i].iStringCode,szMessage,INTERFACE_STRING_LENGTH);
		InsertListData(m_pList_Category,szMessage,0,false);
	}
#endif

	m_pList_Category->SetCurSel(0);

	int sizeRow = m_pList_Category->GetListRowCnt() + 1;
	m_iDropdownMaxScrollValue = sizeRow - MAX_LIST_DATA;
	if(m_iMaxScrollValue < 0 ) 
	{
		m_iDropdownCurScrollValue = 0;
		m_iDropdownMaxScrollValue = 0;
	}
	else
	{
		m_iDropdownCurScrollValue = m_pList_Category->GetCurSel()  - MAX_LIST_DATA + 1;
		if(m_iDropdownCurScrollValue < 0) m_iDropdownCurScrollValue = 0;
	}

	m_pVScrollDrapDown->SetScrollRangeWZ(0,m_iDropdownMaxScrollValue);
	m_pVScrollDrapDown->SetScrollPosWZ(m_iDropdownCurScrollValue);
	m_pList_Category->SetBeginDrawIndex(m_iDropdownCurScrollValue);

}
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
void uiItemTotalcomposite::UpdateTree()
{
	int stringNumber;
	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

	int SelectCategory = m_pList_Category->GetCurSel();
	m_pList_Each_Category->DeleteItemAll();

	bool bChangeCategory = false;
	if ( SelectCategory != m_iMainCategory)
	{
		m_iMainCategory = SelectCategory;
		bChangeCategory = true;
	}

	if(m_iMainCategory < 0) m_iMainCategory =0; // m_iMainCategory는 무조건 0보다 크다
	if((unsigned int)m_iMainCategory >= vMainCategory.size()) m_iMainCategory = 0;

	g_InterfaceManager.GetInterfaceString(vMainCategory[m_iMainCategory].iStringCode,
		szMessage,INTERFACE_STRING_LENGTH);
	m_pStatic[TXT_CUR_CATEGORY]->SetTextWZ(szMessage);

	for(unsigned int i =0 ; i <vMainCategory[m_iMainCategory].vec_subCat.size(); ++i)
	{

		bool bDelSubCategory = TRUE;

		stringNumber = vMainCategory[m_iMainCategory].vec_subCat[i].iStringCode;
		g_InterfaceManager.GetInterfaceString(stringNumber,szMessage,INTERFACE_STRING_LENGTH);
		InsertListData(m_pList_Each_Category,szMessage,0,FALSE);
	
		for(unsigned int k = 0 ; k < vMainCategory[m_iMainCategory].vec_subCat[i].vec_baseCat.size(); ++k)
		{
			BOOL isValid = CheckItemExistValidMap(i,k);
			if(isValid)
			{

				bDelSubCategory = FALSE;

				stringNumber = vMainCategory[m_iMainCategory].vec_subCat[i].vec_baseCat[k].iStringCode;
				g_InterfaceManager.GetInterfaceString(stringNumber,szMessage,INTERFACE_STRING_LENGTH);
				InsertListData(m_pList_Each_Category,szMessage,
					&vMainCategory[m_iMainCategory].vec_subCat[i].vec_baseCat[k].iCode);
			}
		}

		if( bDelSubCategory )
		{
			int iIndex = max( 0, m_pList_Each_Category->GetListRowCnt() -1);
			m_pList_Each_Category->DeleteItem(iIndex);
		}

	}

	m_iMaxScrollValue = m_pList_Each_Category->GetListRowCnt() - MAX_LIST_DATA;
	if(m_iMaxScrollValue < 0 ) 
	{
		m_iCurScrollValue = 0;
		m_iMaxScrollValue = 0;
	}
	else
	{
		m_iCurScrollValue = m_pList_Each_Category->GetCurSel()  - MAX_LIST_DATA + 1;
		if(m_iCurScrollValue < 0) m_iCurScrollValue = 0;
	}

	m_pVScroll->SetScrollRangeWZ(0,m_iMaxScrollValue);
	if (bChangeCategory)
	{
		m_iCurScrollValue = 0;
		m_pList_Each_Category->SetCurSel(-1);	
	}
	m_pVScroll->SetScrollPosWZ(m_iCurScrollValue);
	m_pList_Each_Category->SetBeginDrawIndex(m_iCurScrollValue);	
}

#ifdef _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER

void uiItemTotalcomposite::UpdateTreeReViewer()
{

    Tree_Controller_Ptr_->DeleteAllItem();

    int stringNumber = 0;
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    int SelectCategory = m_pList_Category->GetCurSel();

    bool bChangeCategory = false;
    if ( SelectCategory != m_iMainCategory)
    {
        m_iMainCategory = SelectCategory;
        bChangeCategory = true;
    }

    if(m_iMainCategory < 0) m_iMainCategory =0; // m_iMainCategory는 무조건 0보다 크다
    if((unsigned int)m_iMainCategory >= vMainTreeCategory.size()) m_iMainCategory = 0;

    g_InterfaceManager.GetInterfaceString(vMainTreeCategory[m_iMainCategory]->iStringCode,
        szMessage,INTERFACE_STRING_LENGTH);
    m_pStatic[TXT_CUR_CATEGORY]->SetTextWZ(szMessage);

    MakeCompoisiteTreeitem(NULL,vMainTreeCategory[m_iMainCategory]);
    Tree_Controller_Ptr_->Reflush();
}

void uiItemTotalcomposite::MakeCompoisiteTreeitem( HTREEITEM perentItem,TreeCategory* Item )
{
    for (int i = 0 ; i < (int)Item->childTree.size() ; ++i)
    {
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(Item->childTree[i]->iStringCode, temp_string);

        HTREEITEM child_Item = 
            Tree_Controller_Ptr_->AddTreeItemCheckBox(temp_string,
            0,
            0,
            false,
            StrToWzID("st10"),
            (void*)Item->childTree[i]
            ,perentItem);

        MakeCompoisiteTreeitem(child_Item,Item->childTree[i]);
    }
}

void uiItemTotalcomposite::AddSubTreeItem( sITEM_COMPOSE_LIST * pItemComposeList,TreeCategory* SubTree,int Count,int incCount )
{
    if(Count == incCount)
        return;

    if(TreeCategory* isTree = SubTree->GetTreeCategory(pItemComposeList->m_pCategory[incCount]))
    {
        AddSubTreeItem(pItemComposeList,isTree,Count,incCount+1);
    }
    else
    {
        TreeCategory* Item = new TreeCategory();
        Item->perentidx = SubTree->index;
        Item->iStringCode = pItemComposeList->m_pCategoryCode[incCount];
        Item->index = pItemComposeList->m_pCategory[incCount];
        Item->type = TreeCategory::eType_Sun;
        Item->CompositionCode = pItemComposeList->m_Code;
        memcpy(Item->arryMapCode,pItemComposeList->m_MapCode,sizeof(MAPCODE) * MAPCODE_MAX);
        SubTree->AddChild(Item);

        AddSubTreeItem(pItemComposeList,Item,Count,incCount+1);
        
    }    
}

#endif

//-------------------------------------------------------------------------------------------
/**
*/
void uiItemTotalcomposite::InsertListData(CCtrlListWZ* pList, TCHAR* message,int* pExtraData,BOOL bCenter)
{
	DWORD dwOption = DT_LEFT | DT_VCENTER;
	DWORD Textcolor = RGBA(0,180,180,255);
	
	if(bCenter)
	{
		dwOption = DT_VCENTER | DT_CENTER;
		Textcolor = RGBA(180,180,180,255);
	}
	int index = pList->InsertItemBack();
#ifdef _INTERNATIONAL_UI
	GlobalFunc::SetItem(pList,index,0,message,StrToWzID("st10"), 
#else
	GlobalFunc::SetItem(pList,index,0,message,StrToWzID("mn12"), 
#endif//_INTERNATIONAL_UI
		dwOption,
		RGBA(0,0,0,0), 
		Textcolor, 
		RGBA(100, 100, 180, 255));
	pList->SetItemData(index,pExtraData);
}

BOOL uiItemTotalcomposite::CheckItemExistValidMap(int subID , int baseID)
{
	for(int kk =0 ; kk < MAPCODE_MAX; kk++)
	{
		int val = vMainCategory[m_iMainCategory].vec_subCat[subID].vec_baseCat[baseID].arryMapCode[kk];
		if( vMainCategory[m_iMainCategory].vec_subCat[subID].vec_baseCat[baseID].arryMapCode[kk] == g_pMap->GetMapID() )
		{
			return TRUE;
		}
	}
	return FALSE;
}
