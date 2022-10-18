#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"

class Player;
class MapObject;
namespace ChaosSystem
{
    namespace Function
    {
        // 전장
        void SendReservationPacket();   // 예약 패킷 전송
        void SetBattleGroundReservation(bool is_reservation);  // 전장 예약 버튼 세팅
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void SetGoldRushReservation(const bool& is_reservation);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void RefreshBattleGroundTeam();  // 전장 팀 갱신
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void RefreshGoldRushTeam(); // 골드러쉬 팀 갱신
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        void CreateBattleGroundCollectEffect(Player* player, MapObject* map_object); // 점령 중 이펙트
        void CreateBattleGroundConqeredEffect(Player* player, MapObject* map_object); // 점령 성공 이펙트
        void InitStrongPointAnimaion(const MAPCODE map_code); // 거점 기본 애니메이션
        int GetStrongPointRadarColor(MapObject* map_object);
        void OutputResultStringBattleGround(DWORD result_code); // 전장 에러메세지 출력
        void OutputResultStringFreePVP(DWORD result_code);  // 자유전투실 에러메세지 출력
#ifdef _NA_008334_20150608_SONNENSCHEIN
        void OutputResultStringSonnenSchein(DWORD result_code); // 소넨샤인 에러메세지 출력
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        void OutputResultStringOneDaysGuildMission(DWORD result_code); // 길드일일미션 에러메세지 출력
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    };

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    namespace GoldRushFunction
    {
        // 골드 러쉬
        void SendReservationPacket(); // 예약 패킷 전송
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH    
};

const WzID battle_ground_object_type_id[] =
{
    StrToWzID("wpnu"), /* 흰 거점    battle_ground_normal_waypoint      */
    StrToWzID("wpru"), /* 빨 거점    battle_ground_object_red_waypoint  */
    StrToWzID("wpbu"), /* 파 거점    battle_ground_object_blue_waypoint */

    StrToWzID("trF1"), /* 흰 깃발    battle_ground_object_normal_flag   */
    StrToWzID("trF2"), /* 빨 깃발    battle_ground_object_red_flag      */
    StrToWzID("trF3"), /* 파 깃발    battle_ground_object_blue_flag     */
    StrToWzID("trF4"), /* 파흰 깃발  battle_ground_object_half_blue_flag */
    StrToWzID("trF5")  /* 빨흰 깃발  battle_ground_object_half_red_flag */
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