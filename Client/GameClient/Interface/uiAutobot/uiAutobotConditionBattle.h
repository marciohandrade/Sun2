#ifndef _uiAutobotConditionBattle_h_
#define _uiAutobotConditionBattle_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifdef _YMS_AUTOBOT_GAME

#include "uiAutobotDialog.h"
#include "SCItemSlotContainer.h"
#include "QuickSlotContainer.h"

#include "Mouse.h"

struct BASE_AutomaticComboInfo;


class cInputEditControlContainer
{
private:
    class cInputEditControl
    {
    public:
        CCtrlDxEditBox* m_pEdit;
        CCtrlStaticWZ* m_pText;

        cInputEditControl( CCtrlDxEditBox* pEdit, CCtrlStaticWZ* pText )
        {
            m_pEdit = pEdit;
            m_pText = pText;
        }
    };

    CDialogWZ* m_pDialog;

    std::vector<cInputEditControl*> m_EditControlArray;


protected:
    cInputEditControlContainer(){}

    virtual CDialogWZ* GetDialog() PURE;


    bool ReleaseInputMode()
    {
        for( size_t i=0;i!=m_EditControlArray.size();++i)
        {
            if( IsInputMode(i) == true )
            {
                CHAR buffer[INTERFACE_STRING_LENGTH];

                m_EditControlArray[i]->m_pEdit->GetTextWZA( buffer, INTERFACE_STRING_LENGTH );
                m_EditControlArray[i]->m_pText->SetTextWZA( buffer, false );

                m_EditControlArray[i]->m_pEdit->ShowWindowWZ(WZ_HIDE);
                m_EditControlArray[i]->m_pText->ShowWindowWZ(WZ_SHOW);

                return true;
            }
        }
        return false;
    }

    void SetInputMode( size_t index )
    {

        for( size_t i=0;i!=m_EditControlArray.size();++i)
        {
            if( index == i )
            {
                CHAR buffer[INTERFACE_STRING_LENGTH];
                m_EditControlArray[i]->m_pText->GetTextWZA( buffer, INTERFACE_STRING_LENGTH );
                m_EditControlArray[i]->m_pEdit->SetTextWZA( buffer, false );

                m_EditControlArray[i]->m_pEdit->ShowWindowWZ(WZ_SHOW);
                m_EditControlArray[i]->m_pText->ShowWindowWZ(WZ_HIDE);

                GetDialog()->RequestFocus(m_EditControlArray[i]->m_pEdit);

                break;
            }
        }
    }

    bool IsInputMode( size_t index )
    {
        if( index >= m_EditControlArray.size() )
            return false;

        return m_EditControlArray[index]->m_pEdit->GetState() != WZ_HIDE;
    }

    bool ProcessEditControlMessage(SI_MESSAGE* message )
    {
        if( message->eResultMsg == RT_MSG_LBUTTONCLICK )
        {
            for (size_t i=0;i!=m_EditControlArray.size();++i)
            {
                CCtrlDxEditBox* pEdit = m_EditControlArray[i]->m_pEdit;

                RECT rect = pEdit->GetSizeRect();
                int mouse_x = Mouse::Instance()->GetMouseX();
                int mouse_y = Mouse::Instance()->GetMouseY();
                if (mouse_x > rect.left && mouse_x < rect.right
                    && mouse_y > rect.top && mouse_y < rect.bottom)
                {
                    if( IsInputMode(i) == false )
                    {
                        ReleaseInputMode();
                        SetInputMode(i);

                        return true;
                    }
                    else
                    {
                        return true;
                    }
                }
            }

            if( ReleaseInputMode() == true )
                return true;

            return false;
        }

        return false;
    }

    void RegisterEditControl( CCtrlDxEditBox* pEdit, CCtrlStaticWZ* pText )
    {
        m_EditControlArray.push_back(new cInputEditControl(pEdit, pText));
    }

    size_t GetEditCount() 
    {
        return m_EditControlArray.size(); 
    }

    CCtrlDxEditBox* GetEditControlWZ( size_t index ) 
    {
        if( index >= m_EditControlArray.size() )
            return NULL;

        return m_EditControlArray[index]->m_pEdit;
    }

    void SetEditText( size_t index, int number )
    {
        static CHAR buffer[INTERFACE_STRING_LENGTH];
        wsprintf(buffer, "%d", number );
        SetEditText( index, buffer );
    }

    void SetEditText( size_t index, const std::string& text )
    {
        if( index >= m_EditControlArray.size() ) 
            return;

        m_EditControlArray[index]->m_pText->SetTextWZA(text.c_str(), false );
    }

    std::string GetEditText( size_t index )
    {
        static CHAR buffer[INTERFACE_STRING_LENGTH];

        GetEditText(index, buffer, INTERFACE_STRING_LENGTH);

        return buffer;
    }

    void GetEditText( size_t index, LPTSTR buffer, DWORD bufferlength  )
    {
        if( index >= m_EditControlArray.size() )
            return;

        m_EditControlArray[index]->m_pText->GetTextWZA( buffer, bufferlength);
    }

    void InitEditStateViewState()
    {
        for( size_t i=0;i!=m_EditControlArray.size();++i)
        {
            m_EditControlArray[i]->m_pEdit->ShowWindowWZ(WZ_HIDE);
            m_EditControlArray[i]->m_pText->ShowWindowWZ(WZ_SHOW);
        }
    }
};


//------------------------------------------------------------------------------ 
class uiAutobotConditionBattle : public QuickSlotContainer, public uiAutobotDialog
    , public cInputEditControlContainer
{
    friend class uiAutobotMan;

    enum UIControlPos
    {
        kControl_CheckBT_C001 = 0,
        kControl_CheckBT_C005,
        kControl_CheckBT_C006,
        kControl_CheckBT_C007,
        kControl_CheckBT_C009,
        kControl_CheckBT_C010,
        kControl_CheckBT_C011,
        kControl_CheckBT_C012,
        kControl_CheckBT_C013,
        kControl_CheckBT_C014,
        kControl_CheckBT_C015,
        kControl_CheckBT_C016,
        kControl_CheckBT_C017,
        kControl_CheckBT_C018,
        kControl_CheckBT_C024,
        kControl_CheckBT_C025,
        kControl_CheckBT_C026,
        kControl_CheckBT_C027,
        kControl_CheckBT_C030,
        kControl_CheckBT_C000,
        kControl_Button_B000,
        kControl_Button_B999,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B005,
        kControl_Button_B006,
        kControl_Button_B007,
        kControl_Button_B008,
        kControl_Button_B009,
        kControl_Button_B010,
        kControl_Button_B011,
        kControl_Button_B013,
        kControl_Button_B012,
        kControl_Button_B015,
        kControl_Button_B014,
        kControl_Button_B016,
        kControl_Button_B017,
        kControl_Button_B018,
        kControl_Button_B019,
        kControl_Button_B020,
        kControl_Button_B021,
        kControl_Button_B022,
        kControl_Button_B023,
        kControl_Button_B024,
        kControl_Button_B025,
        kControl_Button_B026,
        kControl_Button_B027,
        kControl_Button_B028,
        kControl_Button_B029,
        kControl_Button_B030,
        kControl_Button_B031,
        kControl_Button_B032,
        kControl_Button_B033,

        kControl_Text_T101,
        kControl_Text_T999,
        kControl_Text_T006,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S003,
        kControl_Text_S004,
        kControl_Text_S010,
        kControl_Text_S005,
        kControl_Text_S006,
        kControl_Text_S007,
        kControl_Text_S008,
        kControl_Text_S009,
        kControl_Text_S012,
        kControl_Text_S019,
        kControl_Text_S020,
        kControl_Text_S021,
        kControl_Text_S022,
        kControl_Text_S025,
        kControl_Text_S026,
        kControl_Text_S035, //!< hp회복 물약먹기
        kControl_Text_S047, //!< mp/sp회복 물약먹기
        kControl_Text_S048, //!< 즉시회복
        kControl_Text_S049, //!< 사료먹기
        kControl_Text_S050, //!< 휴식 앉기(hp)
        kControl_Text_S051, //!< 휴식 앉기(mp)
        kControl_Text_S011,
        kControl_Text_S013,
        kControl_Text_S014,
        kControl_Text_S015,
        kControl_Text_S016,
        kControl_Text_S017,
        kControl_Text_S018,
        kControl_Text_S023,
        kControl_Text_S024,
        kControl_Text_S027,
        kControl_Text_S028,
        kControl_Text_S029,
        kControl_Text_S030,
        kControl_Text_S031,
        kControl_Text_S032,
        kControl_Text_T021,
        kControl_Text_S036,
        kControl_Text_S037,
        kControl_Text_S038,
        kControl_Text_S039,
        kControl_Text_S040,
        kControl_Text_S041,
        kControl_Text_S042,
        kControl_Text_S043,
        kControl_Text_S044,
        kControl_Text_S045,
        kControl_Text_S046,
        kControl_Text_S034, //!< 범위
        kControl_Text_S033, //!< 파티원

        kControl_Edit_E000, // 쿨타임 입력용 에디트박스
        kControl_Edit_E001,
        kControl_Edit_E003,
        kControl_Edit_E002,
        kControl_Edit_E004,
        kControl_Edit_E005,
        kControl_Edit_E006,
        kControl_Edit_E007,
        kControl_Edit_E008,
        kControl_Edit_E009,
        kControl_Edit_E010,
        kControl_Edit_E011,

        kControl_List_L000, //! 휴식 앉기(hp)
        kControl_List_L001, //! 회복(즉시회복)
        kControl_List_L002, //! 회복(hp 물약)
        kControl_List_L003, //! 휴식 앉기(mp)
        kControl_List_L004, //! 회복(사료)
        kControl_List_L005, //! 회복(mp 물약)
        kControl_List_L006, //! 범위 지정
        kControl_List_L007, //! 파티원
        kControl_Picture_P013,  //!< 픽쳐
        kControl_Picture_P015,
        kControl_Picture_P017,
        kControl_Picture_P018,
        kControl_Picture_P019,
        kControl_Picture_P020,
        kControl_Picture_P021,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_Picture_P004,
        kControl_Picture_P005,
        kControl_Picture_P006,
        kControl_Picture_P007,
        kControl_Picture_P008,
        kControl_Picture_P009,
        kControl_Picture_P010,
        kControl_Picture_P011,
        kControl_Picture_P012,
        kControl_Picture_P023,
        kControl_Picture_P024,
        kControl_Picture_P025,
        kControl_Picture_P026,
        kControl_Picture_P027,
        kControl_Picture_P028,
        kControl_Picture_P029,
        kControl_Picture_P014,
        kControl_Picture_P022,
        kControl_Picture_P030,
        kControl_Picture_P031,
        kControl_Picture_P043,
        kControl_Picture_P042,
        kControl_Picture_P041,
        kControl_Picture_P040,
        kControl_Picture_P039,
        kControl_Picture_P038,
        kControl_Picture_P037,
        kControl_Picture_P036,
        kControl_Picture_P035,
        kControl_Picture_P034,
        kControl_Picture_P033,
        kControl_Picture_P032,
        kControl_Picture_PI04,  //!< 앚기 휴식 hp
        kControl_Picture_P044,  //!< 즉시 회복 물약 먹기
        kControl_Picture_P045,  //!< hp 회복 물약 먹기
        kControl_Picture_P046,  //!< 앚기 휴식 mp
        kControl_Picture_P047,  //!< 사료
        kControl_Picture_P048,  //!< mp 회복 물약 먹기
        kControl_Picture_P050,
        kControl_Picture_P052,  //!< 사냥범위
        kControl_Picture_P053,  //!< 파티원

        kControl_Picture_P016,  //!< 그래이(disable) 판
        kControl_Picture_P049,
        kControl_Picture_P051,
        kControl_Picture_P054,
        kControl_Picture_P055,

        kControl_TextImage_TI07,
        kControl_TextImage_TI06,
        kControl_Size,
    };

public:
    enum SkillType
    {
        kSkill_Attack = 0,  //!< 공격형 스킬
        kSkill_Defense,     //!< 방어형 스킬
        kSkill_Max,
    };

public:
    static const DWORD kDialog_ID;

    static const int kSlotSize;

    static const int kDefaultCooltimeSecond;


private:
    CDialogWZ* GetDialog() { return GetDialogWZ(); }

    int is_mouse_overed_on_button_;
    int mouse_overed_control_;

public:
    uiAutobotConditionBattle(InterfaceManager* ui_man);
    virtual ~uiAutobotConditionBattle(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
	virtual void OnShowWindow(BOOL val);
    virtual void NetworkProc(MSG_BASE* msgbase);
    virtual void MessageProc(SI_MESSAGE* message);

    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot & slot);
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot * slotOut);
    virtual void DeleteAllSlots(void);

    SCSlotContainer* GetSlotContainer();
    ItemUnitRenderer* GetItemUnitRender() const { return m_pItemUnitRenderer; }

    bool CanLinkSlot(const SCSlot* slot, POSTYPE to_pos);

protected:
    void DefaultUI();
    bool UpdateCondition();
    bool UpdateUI();
    eAUTO_HUNTING_TYPE GetHuntingTypeFromControlIndex(DWORD control_index);

private:
    bool OnLButtonClick(SI_MESSAGE* message);

    void OnDropdownEnable();
    void OnDropdownShow(UIControlPos list_index, UIControlPos picture_index);

    //------------------------------------------------------------------------------
    //! Returns:   
    //! Qualifier: 공격스킬 등록 가능한지 여부
    //! Parameter: POSTYPE at_pos
    //------------------------------------------------------------------------------
    bool IsAttackSkillSlot(POSTYPE at_pos);
    SkillType GetSkilType(SLOTCODE skill_code);

    //------------------------------------------------------------------------------
    //! Returns:   int : second
    //! Qualifier: 해당슬롯(아이템/스킬)의 쿨타임을 얻어 온다.
    //! Parameter: SCSlot * slot
    //------------------------------------------------------------------------------
    int GetCoolTime(SCSlot* slot);

    ItemUnitRenderer* m_pItemUnitRenderer;
    
};

#endif //_YMS_AUTOBOT_GAME

#endif
