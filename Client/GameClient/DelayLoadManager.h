#ifndef _EQUIP_LOAD_MANAGER_H_
#define _EQUIP_LOAD_MANAGER_H_

#include <Singleton.h>

#if _MSC_VER > 1000
#pragma once
#endif
//------------------------------------------------------------------------------
/**
    Player 딜레이 로딩

    @author		광수

    @since		10.03.08

    @remarks	마을의 로드렉 감소를 위해서 생성된 Player의 리소스(파츠.펫.라이더)정보를
                일정 간격을 가지고 순차적의로 로딩하기
*/
enum DelayType
{
    kProcessDelay = 0,
    kLoadDelay,
    kMaxType,
};

typedef std::list<DWORD> LOAD_PALYER_LIST;
typedef std::list<DWORD>::iterator LOAD_PALYER_LIST_ITR;

class DelayLoadManager : public util::Singleton<DelayLoadManager>
{
public:
    DelayLoadManager( void );
    ~DelayLoadManager( void );
    
    // 시간체크
    void Process();

    // 딜레이로딩할 Player키값삽입
    void InsertPlayer(DWORD player_key);

    // 지연시간얻기
    DWORD delay_time(DelayType type){return delay_time_[type];}

    // 현재 쌓인 Queue사이즈
    DWORD size(){return load_player_list_.size();}
    
    // 지연시간 지정
    void set_delay_time(DelayType type,DWORD time){ delay_time_[type] = time; }
    
    // Queue비우기
    void clear(){load_player_list_.clear();}
    
private:
    DWORD delay_time_[kMaxType];

    LOAD_PALYER_LIST load_player_list_;

};
#endif//_EQUIP_LOAD_MANAGER_H_












