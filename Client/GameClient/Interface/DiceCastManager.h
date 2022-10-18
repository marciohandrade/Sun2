#pragma once

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
#include "InterfaceManager.h"
#include "../PartyLib/PartyDefine.h"

const DWORD DICE_DRAWING_TIME = 5000;    // �ֻ����� �������� �ð� (�����ִ� �ð� ����)
const DWORD DICE_ANIMATION_TIME = 1000; // �ֻ����� �������� �ð�

class DiceCastManager
{
    // 1~100 ���� �ֻ��� ���ڷ� ��ȯ(1~6)
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

    // 2011.09.21 ��� ��Ŷ �ް� 1�� �ִϸ��̼��� ������� �ʾ� ��ȸ�� ���� / ��ȹ�� ���������� ���� ����
    //bool    is_draw_; //�ֻ��� ��ȸ�� ������
    bool    is_start_animation_;  // �ֻ����� �����̴���
    DWORD   draw_start_animation_tick_;
    DWORD   high_number_player_key_;  //���� ���� ���� ���� �÷��̾�
    TCHAR   output_message_[INTERFACE_STRING_LENGTH];  // ���Ϳ� ����� �޼���
};




#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING