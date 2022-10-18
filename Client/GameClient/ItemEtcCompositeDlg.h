//=======================================================================================================================
/** ������ ���� ���� â(�Ǽ����� ����, �ɼ�����,...)
	@author
			���翵
	@desc
		�Ǽ����� ����, �ɼ�����, ���� �߰����� �� ���̾�α�
	@since
		2005. 8. 26.
			
	@remarks
*/

#ifndef _ITEMETCCOMPOSITEDLG_H_
#define _ITEMETCCOMPOSITEDLG_H_

#include "solardialog.h"
#include "SCItemSlotContainer.h"


class SCItemSlotContainer;

class ItemETCCompositeDlg : public SolarDialog, public SCItemSlotContainer
{
public:

	enum eITEMENCHANT_POS
	{
		eITEMENCHANT_BT_SLOT_1 = 0,
		eITEMENCHANT_BT_SLOT_2,
		eITEMENCHANT_BT_SLOT_3,
		eITEMENCHANT_BT_SLOT_4,
		eITEMENCHANT_BT_SLOT_5,
		eITEMENCHANT_BT_SLOT_6,
		eITEMENCHANT_TI_ENCHANT,
		eITEMENCHANT_TI_RETRY,
		eITEMENCHANT_TI_EXIT,
		eITEMENCHANT_VALUE_MONEY,
		eITEMENCHANT_VALUE_KINDOFENCHANT,
		eITEMENCHANT_VALUE_SUB_1_MATERIAL,
		eITEMENCHANT_VALUE_SUB_2_MATERIAL,
		eITEMENCHANT_VALUE_SUB_3_MATERIAL,
		eITEMENCHANT_VALUE_SUB_4_MATERIAL,
		eITEMENCHANT_VALUE_SUB_5_MATERIAL,

		eITEMENCHANT_BT_EXIT,
		

		Dialog_Max
	};

	enum 
	{
		eITEMENCHANT_MAIN_MATERIAL	= eITEMENCHANT_BT_SLOT_1,
		eITEMENCHANT_MAX_SUB_SLOT	= 5,
		eITEMENCHANT_MAX_SLOT		= eITEMENCHANT_BT_SLOT_5+1,
	};

	enum ePACKET_STATUS
	{
		PACKET_STATUS_NONE		= 0,
		PACKET_STATUS_EXTRACT	= 1,
	};

	ItemETCCompositeDlg(void);
	virtual ~ItemETCCompositeDlg(void);
	

	// �⺻ virtual methods
	VOID			Init( CDrawBase * pDrawBase );
	virtual VOID	Release();

	virtual VOID	MessageProc( SI_MESSAGE * pMessage );

	virtual VOID	OnUpdateSolarDialog();
	virtual VOID	OnRenderSolarDialog();

	virtual VOID	OnShowWindow(BOOL val);

	// ������ ���� 
	VOID	InitContainer();
	VOID	InitRenderer();
	VOID	ReleaseContainer();
	VOID	FlushUnitDraw();

	ItemUnitRenderer *		GetItemUnitRender() const;

	virtual RC::eSLOT_INSERT_RESULT			InsertSlot( POSTYPE AtPos, SCSlot & rSlot );									// ���Կ� ��ü �߰�
	BOOL									DeleteSlot( POSTYPE AtPos, SCSlot * pSlotOut );

	// ó��
	VOID				SetCurrentCompositeType(eITEMCOMPOSITECODE eCompositeType);
	eITEMCOMPOSITECODE	GetCurrentCompositeType(void);

	VOID				Enchant();
	VOID				ReTry();
	VOID				Exit();

	VOID				SetRequiredMoney(MONEY Money);
	VOID				SetTargetInfo(SLOTIDX fromSlotIdx, POSTYPE fromPos);

	TYPEDEF_MAP(LOCKING_INFO, POSTYPE, POSTYPE);


	DWORD				GetDialogKey() { return m_DialogKey; }
	void				SetDialogKey( DWORD DialogKey ) { m_DialogKey = DialogKey; }

	// �⺻ proected methods
protected:
	CControlWZ *		getControl( POSTYPE AtPos );
	POSTYPE				getCtrlIDToPos( WzID wzId );

	void				initEnchantInfo();
	void				initEnchantInfoText();
	void				showEnchantTitleText();
	void				fillEnchantExtraInfo();
	void				updateMaterial();

	void				activateMaterials(BOOL bActivate);

	// �⺻ attributes
private:
	DWORD				m_DialogKey;
	static WzID			m_wzId[Dialog_Max];

	eITEMCOMPOSITECODE		m_eCurCompositeType;	// const.h ���� 
	ItemUnitRenderer *		m_pItemUnitRenderer;

	BOOL					m_bValidEnchant;		// ��þƮ ������ ���������� ��Ÿ����.
	SLOTIDX					m_FromTargetSlotIdx;
	POSTYPE					m_FromTargetPos;

	CCtrlStaticWZ *			m_CtrlKindOfCompositeStatic;
	CCtrlStaticWZ *			m_CtrlSubMaterialStatic[4];
	CCtrlStaticWZ *			m_CtrlMoneyStatic;

	MONEY					m_RequireMoney;
	TCHAR					m_szBuf[512];
	TCHAR					m_szBuf2[512];
	TCHAR					m_szResult[512];
	LOCKING_INFO_MAP		m_LockingInfoMap;
};


#endif // _ITEMETCCOMPOSITEDLG_H_