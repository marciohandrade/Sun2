#pragma once

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
#include "InterfaceManager.h"
#include "../PartyLib/PartyDefine.h"

const DWORD DICE_DRAWING_TIME = 5000;    // 주사위가 보여지는 시간 (멈춰있는 시간 포함)
const DWORD DICE_ANIMATION_TIME = 1000; // 주사위가 굴러가는 시간

class DiceCastManager
{
    // 1~100 값을 주사위 숫자로 변환(1~6)
    enum DICE_NUMBER_RANGE
    {
        DICE_NUMBER_ONE     = 20,
        DICE_NUMBER_TWO     = 40,
        DICE_NUMBER_THREE   = 60,
        DICE_NUMBER_FOUR    = 75,
        DICE_NUMBER_FIVE    = 89,
        DICE_NUMBER_SIX     = 99,
    };
public:
    DiceCastManager(InterfaceManager* interface_manager) : interface_manager_(interface_manager),
      draw_start_animation_tick_(0),
      is_start_animation_(false),
      high_number_player_key_(0)
    {
        ZeroMemory(dice_draw_info_, sizeof(dice_draw_info_));
        ZeroMemory(output_message_, sizeof(output_message_));
    }
    ~DiceCastManager() {}

    int GetDiceNumberToValue(WORD number);
    BYTE GetDiceNumber(DWORD user_key);
    void SetDiceDrawInfo(_DICECAST_RESULT* dice_draw_info, int player_count);
    DWORD GetAnimationTick();
    bool IsDiceDraw();
    bool IsDiceDraw(DWORD user_key);
    void SetDiceDrawOff();
    void SetCenterMessage(TCHAR *center_message);

    DWORD high_number_player_key()
    {
        return high_number_player_key_;
    }
    void set_high_number_player_key(DWORD player_key)
    {
        high_number_player_key_ = player_key;
    }

    bool is_start_animation()
    {
        return is_start_animation_;
    }
private:
    InterfaceManager* interface_manager_;
    _DICECAST_RESULT dice_draw_info_[MAX_PARTYMEMBER_NUM];

    // 2011.09.21 결과 패킷 받고 1초 애니메이션이 어색하지 않아 공회전 삭제 / 기획팀 김장현씨와 협의 내용
    //bool    is_draw_; //주사위 공회전 보일지
    bool    is_start_animation_;  // 주사위가 움직이는지
    DWORD   draw_start_animation_tick_;
    DWORD   high_number_player_key_;  //가장 높은 수가 나온 플레이어
    TCHAR   output_message_[INTERFACE_STRING_LENGTH];  // 센터에 출력할 메세지
};




#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING