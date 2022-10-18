#include "SunClientPrecompiledHeader.h"

#pragma once
#ifdef _NA_007086_20140318_MONSTERBOOK
#include "uiMonsterBookMan/uiMonsterBookBase.h"
#include "uiBase.h"
#include <SCItemSlot.h>

class uiMonsterBookDropList : public uiMonsterBookBase
{
    typedef STLX_HASH_MAP<DWORD, IconImage*> MONTERBOOK_LOAD_ICON_LIST;
public:
    enum UIControlPos
    {
        kControl_Text_S007 = 100,   // lv %s
        kControl_Text_S018,         // �̸�
        kControl_List_L000,         // ����Ʈ - �������Ʈ
        kControl_VScroll_V000,      // ��ũ��

        kControl_Size,
    };

public:
    uiMonsterBookDropList(InterfaceManager* ui_manager);
    virtual ~uiMonsterBookDropList();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

protected:
    void SetMonsterInfo(DWORD monster_code);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RenderTooltip();
    
    bool ClickTreeDropList(TreeMenuGroup::TreeObject* tree_object);
    TreeMenuGroup& tree_drop_list() { return tree_drop_list_; }
    void UnloadIconTexture();

    static void Callback_DropListRender(CCtrlListWZ* list_control,
            CCtrlListWZ::CListSellData* sell_data,
            int sell_index,
            RECT& render_area);

    TreeMenuGroup tree_drop_list_;               // ���� �������Ʈ Ʈ��
    SCItemSlot tooltip_item_slot_;               // ���� ��¿� �ӽý���
  
    static MONTERBOOK_LOAD_ICON_LIST loaded_icon_list_;   // �ε��� ������ ����Ʈ
    static DWORD tooltip_itemcode_;
};

#endif //_NA_007086_20140318_MONSTERBOOK
