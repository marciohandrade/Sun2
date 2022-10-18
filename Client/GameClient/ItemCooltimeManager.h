#ifndef _ITEM_COOL_TIME_MGR_H_
#define _ITEM_COOL_TIME_MGR_H_

#include <Singleton.h>

#if _MSC_VER > 1000
#pragma once
#endif
//------------------------------------------------------------------------------
/**
    ������ ��Ÿ�� ����

    @author		����

    @since		10.07.28

    @remarks	Ŭ�� ������ ��Ÿ�ӿ��������� �Ŵ���
                ��Ÿ���� ������ Ư�������� �Ѵ�
*/


// ��Ÿ�� Ÿ��
enum CoolTimeType
{
    kEtheriaCombine = 0,
    kCrystalization,
};

// ��� ����ü(������ �߰��Ǹ� union�� �߰��ϴ½�)
typedef struct _CooltimeStruct
{
    CoolTimeType type;
    DBSERIAL serial_key;
    SLOTIDX slot_index; 
    POSTYPE item_pos;
    bool execute;
    
    float cooltime;
    float max_time;
    float dealy_time;
    
    union
    {
        struct 
        {
            POSTYPE from_pos;
            POSTYPE to_pos;

        }EtheriaCombine;

        struct
        {
            POSTYPE target_pos;
            SLOTCODE slot_code;
        }Crystalization;
    };

}CooltimeStruct;

typedef std::map<DBSERIAL, CooltimeStruct*> CoolTimeMap;
typedef CoolTimeMap::iterator CoolTimeItr;
typedef std::pair<DBSERIAL, CooltimeStruct*>CoolTimePair;

class ItemCooltimeManager : public util::Singleton<ItemCooltimeManager>
{
public:
    ItemCooltimeManager( void );
    ~ItemCooltimeManager( void );
    
    void Release();

    void InsertItem(const CooltimeStruct* data);

    void Process(DWORD tick);

    bool FindCooltime(DBSERIAL key, float* progress_time = NULL);
    bool CancelCooltime(DBSERIAL key); // ��Ÿ�� ���
private:

    bool SetItemLock(CooltimeStruct* data, bool lock);

    void DeleteItem(CooltimeStruct* cooltime_data);

    void Execute(CooltimeStruct* cooltime_data);
    
    CoolTimeMap& cooltime_container(){return cooltime_container_;}

    CoolTimeMap cooltime_container_;
    
};


#endif//_ITEM_COOL_TIME_MGR_H_












