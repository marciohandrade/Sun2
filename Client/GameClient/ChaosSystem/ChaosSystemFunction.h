#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"

class Player;
class MapObject;
namespace ChaosSystem
{
    namespace Function
    {
        // ����
        void SendReservationPacket();   // ���� ��Ŷ ����
        void SetBattleGroundReservation(bool is_reservation);  // ���� ���� ��ư ����
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void SetGoldRushReservation(const bool& is_reservation);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void RefreshBattleGroundTeam();  // ���� �� ����
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void RefreshGoldRushTeam(); // ��巯�� �� ����
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void CreateBattleGroundCollectEffect(Player* player, MapObject* map_object); // ���� �� ����Ʈ
        void CreateBattleGroundConqeredEffect(Player* player, MapObject* map_object); // ���� ���� ����Ʈ
        void InitStrongPointAnimaion(const MAPCODE map_code); // ���� �⺻ �ִϸ��̼�
        int GetStrongPointRadarColor(MapObject* map_object);
        void OutputResultStringBattleGround(DWORD result_code); // ���� �����޼��� ���
        void OutputResultStringFreePVP(DWORD result_code);  // ���������� �����޼��� ���
#ifdef _NA_008334_20150608_SONNENSCHEIN
        void OutputResultStringSonnenSchein(DWORD result_code); // �ҳٻ��� �����޼��� ���
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        void OutputResultStringOneDaysGuildMission(DWORD result_code); // ������Ϲ̼� �����޼��� ���
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    };

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    namespace GoldRushFunction
    {
        // ��� ����
        void SendReservationPacket(); // ���� ��Ŷ ����
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH    
};

const WzID battle_ground_object_type_id[] =
{
    StrToWzID("wpnu"), /* �� ����    battle_ground_normal_waypoint      */
    StrToWzID("wpru"), /* �� ����    battle_ground_object_red_waypoint  */
    StrToWzID("wpbu"), /* �� ����    battle_ground_object_blue_waypoint */

    StrToWzID("trF1"), /* �� ���    battle_ground_object_normal_flag   */
    StrToWzID("trF2"), /* �� ���    battle_ground_object_red_flag      */
    StrToWzID("trF3"), /* �� ���    battle_ground_object_blue_flag     */
    StrToWzID("trF4"), /* ���� ���  battle_ground_object_half_blue_flag */
    StrToWzID("trF5")  /* ���� ���  battle_ground_object_half_red_flag */
};
const WzID battle_ground_waypoint_animation_id[] = 
{
    StrToWzID("A000"), /* battle_ground_normal_waypoint_whitefield                 */
    StrToWzID("A001"), /* battle_ground_normal_waypoint_whitefield_redconquering   */
    StrToWzID("A002"), /* battle_ground_normal_waypoint_redfield_redconquered      */
    StrToWzID("A003"), /* battle_ground_normal_waypoint_redfield                   */
    StrToWzID("A004"), /* battle_ground_normal_waypoint_redfield_blueconquering    */
    StrToWzID("B001"), /* battle_ground_normal_waypoint_whitefield_blueconquering  */
    StrToWzID("B002"), /* battle_ground_normal_waypoint_bluefield_blueconquered    */
    StrToWzID("B003"), /* battle_ground_normal_waypoint_bluefield                  */
    StrToWzID("B004"), /* battle_ground_normal_waypoint_bluefield_redconquering    */

    StrToWzID("A000"), /* battle_ground_red_waypoint_redfield                      */
    StrToWzID("A001"), /* battle_ground_red_waypoint_redfield_blueconquering       */
    StrToWzID("A002"), /* battle_ground_red_waypoint_redfield_redconquered         */

    StrToWzID("A000"), /* battle_ground_blue_waypoint_bluefield                    */
    StrToWzID("A001"), /* battle_ground_blue_waypoint_bluefield_redconquering      */
    StrToWzID("A002"),  /* battle_ground_blue_waypoint_bluefield_blueconquerd       */

    StrToWzID("A001")  /* battle_ground_flag_change_animation                      */
};
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND