#ifndef _UI_ITEM_COMPOSITE_MAN_H_
#define _UI_ITEM_COMPOSITE_MAN_H_ 


#include "uiItemCompositeMan_def.h"
#include "uiBaseMan.h"
#include "SCItemSlot.h"
#include "solarDialog.h"

#include "ItemComposeListParser.h"

#include "uiSystemMan/uiSystemMessage.h"

class uiItemTotalcomposite;
class uiItemCube;
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
class ItemType;
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
enum eItemCompositeCatagory
{
	ITEM_COMPOSITE_CATEGORY_NONE = 0,
	ITEM_COMPOSITE_CATEGORY_COMPOSITE,		// 사용안함!!!!!!!!!!!!!
	ITEM_COMPOSITE_CATEGORY_TOTAL_COMPOSITE, //조합(생성)
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	ITEM_COMPOSITE_CATEGORY_CRYSTALIZE,
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
};


class uiItemCompositeMan : public uiBaseMan
{
public:
	enum { INVALID_MAIN_MAT_POS = -1, };

	uiItemCompositeMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void			OnInitialize();
    virtual void            InitPKBooleans();

private:
    BOOL                    _LoadUI();

    //--------------------------------------------------------------------------
    // functions
    //
public:
	SolarDialog *			ShowItemComposite( 
								eItemCompositeCatagory eCompositeCategoryType, 
								eITEMCOMPOSITECODE ItemCompositeCode );

	eItemCompositeCatagory	GetCompositeCategory() { return this->m_eCurItemCompositeCatagory; }
	eITEMCOMPOSITECODE		GetCompositeCode() { return this->m_eCurItemCompositeCode; }	



	//--------------------------------------------------------------------------
    // composite funcs
    //
public:
	//BOOL					IsReadyForCompositeMaterials();
	BOOL					fillTotalCompositeInfo(int itemCode);
	BOOL					IsReadyForTotalCompositeMaterials();
	BOOL					NET_SEND_CG_TOTAL_ITEM_COMPOSITE_SYN();
	int						GetAvalibleCompositeNum(CODETYPE ItemCode, int iItemNum,int iFirst);
	BOOL					isValidTotalCompositeFlag();
	BOOL					spendTotalComposeMaterials();
	int						GetMaxCompositeNum() {return m_maxCompositeNum;}
	int						GetManualCompositeNum() {return m_ManualCompositeNum;}
	void					SetManualCompositeNum(int val) { m_ManualCompositeNum = val;}
	void					UpdateInfo(BOOL bInitCompositeNum = TRUE);
	int						GetCompositeResultType() { return m_iCompositeResultType;}
	void					ClearInfo();

#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    BOOL IsExistItemForComposite(
        SLOTCODE material_code,
        BYTE material_number,
        ItemType* item_type_ptr = NULL,
        DWORD item_type_index = 0);
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION

protected:
	//BOOL					isValidCompositeFlag();	
	//BOOL					fillCompositeInfo();
	//BOOL					spendComposeMaterials();

	//--------------------------------------------------------------------------
    // crystalize funcs
    //
public:
    static void             _Callback_Crystalization_Confirm(uiSystemMessage::ButtonIndex button_index);
    BOOL                    NET_SEND_CG_ITEM_CRYSTALIZE_SYN(const POSTYPE& target_Pos, const SLOTCODE& slot_code);

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    eItemCompositeCatagory		m_eCurItemCompositeCatagory;
	eITEMCOMPOSITECODE			m_eCurItemCompositeCode;
	eITEMCOMPOSITECODE			m_ePendingItemCompositeCode;    

	uiItemTotalcomposite *		m_pUIItemTotalComposite;
	MSG_CG_ITEM_COMPOSE_SYN		m_MsgTotalComposeSyn;
	int							m_iCompositeResultType;
	int							m_maxCompositeNum;
	int							m_ManualCompositeNum;
	int							m_PendingManualCompositeNum;

//	uiItemComposite *			m_pUIItemComposite;
	MSG_CG_ITEM_COMPOSE_SYN		m_MsgComposeSyn;

    uiItemCube* ui_item_cube_;
};

#endif //_UI_ITEM_COMPOSITE_MAN_H_