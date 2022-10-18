#ifndef _EQUIP_LOAD_MANAGER_H_
#define _EQUIP_LOAD_MANAGER_H_

#include <Singleton.h>

#if _MSC_VER > 1000
#pragma once
#endif
//------------------------------------------------------------------------------
/**
    Player ������ �ε�

    @author		����

    @since		10.03.08

    @remarks	������ �ε巺 ���Ҹ� ���ؼ� ������ Player�� ���ҽ�(����.��.���̴�)������
                ���� ������ ������ �������Ƿ� �ε��ϱ�
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
    
    // �ð�üũ
    void Process();

    // �����̷ε��� PlayerŰ������
    void InsertPlayer(DWORD player_key);

    // �����ð����
    DWORD delay_time(DelayType type){return delay_time_[type];}

    // ���� ���� Queue������
    DWORD size(){return load_player_list_.size();}
    
    // �����ð� ����
    void set_delay_time(DelayType type,DWORD time){ delay_time_[type] = time; }
    
    // Queue����
    void clear(){load_player_list_.clear();}
    
private:
    DWORD delay_time_[kMaxType];

    LOAD_PALYER_LIST load_player_list_;

};
#endif//_EQUIP_LOAD_MANAGER_H_












