#ifndef _ITEM_COOL_TIME_MGR_H_
#define _ITEM_COOL_TIME_MGR_H_

#include <Singleton.h>

#if _MSC_VER > 1000
#pragma once
#endif
//------------------------------------------------------------------------------
/**
    아이템 쿨타임 연출

    @author		광수

    @since		10.07.28

    @remarks	클라 아이템 쿨타임연출을위한 매니저
                쿨타임이 끝나면 특정동작을 한다
*/


// 쿨타임 타입
enum CoolTimeType
{
    kEtheriaCombine = 0,
    kCrystalization,
};

// 등록 구조체(내용이 추가되면 union에 추가하는식)
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
    bool CancelCooltime(DBSERIAL key); // 쿨타임 취소
private:

    bool SetItemLock(CooltimeStruct* data, bool lock);

    void DeleteItem(CooltimeStruct* cooltime_data);

    void Execute(CooltimeStruct* cooltime_data);
    
    CoolTimeMap& cooltime_container(){return cooltime_container_;}

    CoolTimeMap cooltime_container_;
    
};


#endif//_ITEM_COOL_TIME_MGR_H_












