#pragma once
#include "uiBaseMan.h"
#include "AchievementParser.h"

struct IM_AREACONQUEST_MANAGER
{
    static const DWORD	AREACONQUEST_DIALOG;
    static const DWORD	AREACONQUEST_COMPLIMENT;
};

class Achievement;
class uiAreaConquestDialog;
class uiAreaConquestCompliment;
class Item;

struct _stAcTreelist
{
    _stAcTreelist()
    {
        childTree.clear(); 
        PerentItem = NULL;
        size = 0;
        depth = 0;
        item_max_count_ = 0;
        item_current_count_= 0;
        index_to_ac_ = 0;
    }

    int		iStringCode;
    int		index;
    int     perentidx;
    int     type;
    int     size;
    int     depth;
    int     nation_code_;
    int     item_max_count_;
    int     item_current_count_;
    int     index_to_ac_;

    enum 
    {
        eType_Main,
        eType_Sun,
        eType_Item,
    };

    std::vector<_stAcTreelist*> childTree;
    _stAcTreelist*             PerentItem;

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

    void AddChild(_stAcTreelist* Item)
    {
        Item->depth = depth + 1;//해당 아이템 깊이는 부모 + 1
        Item->PerentItem = this;
        Item->perentidx = perentidx;
        childTree.push_back(Item);
        size = (int)childTree.size();
    }

    //하위 트리 전체 검색
    _stAcTreelist* GetTreeCategoryForChild(int idx)
    {
        if(index == idx)
        {
            return this;
        }

        for(size_t i = 0 ; i < childTree.size() ; ++i)
        {
            _stAcTreelist* tree = GetTreeCategory(i);
            if(tree)
            {
                return tree;
            }
        }

        return NULL;
    }

    //하위 단계만 검사 - 트리 전체 검사 안함
    _stAcTreelist* GetTreeCategory(int idx)
    {
        for(size_t i = 0 ; i < childTree.size() ; ++i)
        {
            _stAcTreelist* tree = childTree[i];
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

    void sort()
    {
        if(isMaxDepth())
        {
            return;
        }

        std::sort(childTree.begin(),childTree.end(),_stAcTreelist::list_compare_lower);

        for(size_t i = 0 ; i < childTree.size() ; ++i)
        {
            childTree[i]->sort();
        }
    }

    static bool list_compare_lower(const _stAcTreelist* lhs, const _stAcTreelist* rhs)
    {
        //이건 내림차순
        return lhs->index < rhs->index;
    }

    static bool list_compare_upper(const _stAcTreelist* lhs, const _stAcTreelist* rhs)
    {
        //이건 오름차순
        return lhs->index > rhs->index;
    } 
};

class uiAreaConquestMan : public uiBaseMan
{
    enum ePacketPending
    {
        kPacketPending_Base = 0,
        kPacketPending_Reward,
        
        kPacketPending_Max,
    };
    const static DWORD TIMEOUT_PACKET_PENDING = 5;

public:
    uiAreaConquestMan(InterfaceManager *pUIMan);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void OnInitialize();

    virtual void InitPKBooleans();
    virtual void OnRelease();
    virtual void Update();
    virtual void NetworkProc(MSG_BASE* recv_msg);

    void SEND_CG_AREACONQUEST_REWARD_SYN();
    bool CheckPacketPending_Reward();

    Achievement* GetAchievement() { return achievement_; }
    bool IsCompleteACItem(SLOTCODE item_code);

    void DeleteComplimentList();
    static bool CanPickACItem(Item* item_object, bool is_show_notice); // 일반아이템/AC수집중 true, AC수집완료 false

	//내부 사용 함수. inline화
	inline void AchievementVaulePrint(WORD	index_number_,WORD	location_code,BYTE	object_index_);


    void RECV_CG_AREACONQUEST_STREAM_CMD(MSG_BASE* recv_msg);
    void RECV_CG_AREACONQUEST_VALUE_CHANGE_CMD(MSG_BASE* recv_msg);
    void RECV_CG_AREACONQUEST_REWARD_ACK(MSG_BASE* recv_msg);
    void RECV_CG_AREACONQUEST_REWARD_NAK(MSG_BASE* recv_msg);
    void UpdateCanGetACItem(WORD index_number);

private:
    Achievement* achievement_;
    uiAreaConquestDialog* ui_ac_dialog_;
    uiAreaConquestCompliment* ui_ac_notice_;
#ifdef _NA_006541_RENEWER_ACDIALOG
    std::vector<_stAcTreelist*>   ac_data_list_;

    void Load_Ac_Data();
    void make_ac_tree(Achievement_ParserInfo* info);
    void AddSubItem(Achievement_ParserInfo* info,_stAcTreelist* perent);

public:
    std::vector<_stAcTreelist*>& GetAcDataList() { return ac_data_list_; }
#endif
};

inline bool uiAreaConquestMan::CheckPacketPending_Reward()
{
    return SafePKBool(kPacketPending_Reward);
}
