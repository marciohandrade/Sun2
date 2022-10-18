#pragma once

#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#include "uiBase.h"
#include "ItemSlotContainer.h"
#include "uiSystemMan/uiSystemMessage.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"

class uiEquipmentEvolutionMan;
class uiEquipmentEvolutionDialog : public uiBase
{
	enum UIControlPos
	{
		kControl_Button_B002, 
		kControl_Button_B000, 
		kControl_Button_B003,

		kControl_Button_B999, 

		kControl_Button_B005,
		kControl_Button_B006,

		kControl_Text_S008, 
		kControl_Text_S002, 
		kControl_Text_S003, 

		kControl_Button_B001,

		kControl_Size,
	};

	enum EQUIPMENT_EVOLUTION_SLOT_POS
	{
		EQUIPMENT_EVOLUTION_SLOT = 0,
		EQUIPMENT_EVOLUTION_ADDICTIVE_SLOT, 
		EQUIPMENT_EVOLUTION_PREDICTION_SLOT, 
		EQUIPMENT_EVOLUTION_SLOT_MAX,
	};

	enum ePACKET_STATUS
	{
		PACKET_STATUS_NONE = 0,
		PACKET_STATUS_EVOLUTION = 1,
	};
public:
	uiEquipmentEvolutionDialog(InterfaceManager* ui_manager);
	virtual ~uiEquipmentEvolutionDialog(void);

public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();
	virtual void OnShowWindow(BOOL is_show);
	virtual void MessageProc(SI_MESSAGE* message);
	virtual void NetworkProc(MSG_BASE* recv_packet);
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();

	SCItemSlotContainer* GetEquipmentEvolutionContainer() { return slot_container_[0].GetSlotContainer(); }

	static void Callback_SendPacketItemEvolutionSyn(uiSystemMessage::ButtonIndex button_index);
	void        SendPacketItemEvolutionSyn(const bool& is_send_packet);
	void        ResultCodeProc(const RC::eITEM_RESULT& result_code);
	void        ResultCodeSoundProc(const RC::eITEM_RESULT& result_code);
	void        RemoveBitCheck();
	void        ResetEvolutionDialog();
	void        UpdateFtextRenderRectSetting();

private:
	bool         OnLButtonClick(SI_MESSAGE* message);
	bool         OnRButtonClick(SI_MESSAGE* message);
	bool         OnMouseOver(SI_MESSAGE* message);
	void         UpdateTooltip();

private:
	void         InitEquipmentEvolutionSlotContainer();

private:
	void         EvolutionResultSetting();
	void         EvolutionResultClear();
	void         ActiveAdditiveSlot();
	void         DeActiveAdditiveSlot();
	void         ActivePredictionSlot();
	void         DeActivePredictionSlot();
	bool         PermitEquipmentCheck( const SCItemSlot* item );
	bool         PermitAdditiveCheck( const SCItemSlot* item );
	void         Evolution();
	void         OpenTutorialGuid();

public:
	void         OnEquipmentEvolutionItem_Click(const POSTYPE& equipment_evolution_slot_index, const SLOTIDX from_container_index, 
		const POSTYPE from_container_position);
	void         OnEquipmentEvolutionItem_InventoryRightClick(const SLOTIDX from_container_index, 
		                                                       const POSTYPE from_container_position);

protected:
	POSTYPE      getCtrlIDToPos( WzID wzId );
	CControlWZ*  getControl( POSTYPE pos );

private:
	static WzID wz_id_[kControl_Size];

private:
	ItemSlotContainer    slot_container_[EQUIPMENT_EVOLUTION_SLOT_MAX];
	POSTYPE              equip_item_pos_;
	POSTYPE              additive_item_pos_;
	POSTYPE              prediction_item_pos_;

	// 기획자의 요청으로 stringtable로 제어 할 수 있도록 ftextrender 사용
	FTextParser          description_ftext_parser_;
	FTextSentence        description_ftext_sentence_;
	FTextParam           description_ftext_param_;
	FTextRender          description_ftext_render_[3];
};

inline
POSTYPE uiEquipmentEvolutionDialog::getCtrlIDToPos( WzID wzId )
{
	for ( POSTYPE i = 0; i < kControl_Size; ++i )
	{
		if ( wz_id_[i] == wzId )
		{
			return i;
		}
	}
	return kControl_Size;
}
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
