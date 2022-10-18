클라이언트 참고: 


//클라이언트용 사용 가이드 

Achievement achievement;

/* ======== MSG_CG_AREACONQUEST_STREAM_CMD 수신시 ========== */
MSG_CG_AREACONQUEST_STREAM_CMD msg;
achievement.Serialize(msg.achievement_info_, SERIALIZE_STORE);

//업적 전체 순회
Achievement::ACHIEVEMENT_PART_ITER iter_part = achievement.Begin();
for(; iter_part != achievement.End() ; iter_part++)
{
	AchievementPart::ACHIEVEMENT_ITER iter = iter_part->second.Begin();

	WORD accomplishment_ratio = iter_part->second.GetAccomplishmentRatio(); //지역 달성률

	for(; iter != iter_part->second.End() ; iter++)
	{
		iter->second.GetIndex();
		//iter->second.GetStringCode();
		//iter->second.GetToolTipCode();
		//iter->second.GetIconCode();			
		iter->second.IsReward();

		for(int i = 0 ; i < MAX_ACHIEVEMENT_OBJECT ; i++)
		{
			iter->second.GetObjectInfo(i);			
		}
		for(int i = 0 ; i < MAX_ACHIEVEMENT_REWARD ; i++)
		{
			iter->second.GetRewardInfo(i);			
		}
	}
}
/* ========================================================= */



/* ======== MSG_CG_AREACONQUEST_VALUE_CHANGE_CMD 수신시 ========== */
MSG_CG_AREACONQUEST_VALUE_CHANGE_CMD msg;	
achievement.SetAchievementValue(msg.location_code, msg.index_number_, 
								msg.object_index_, msg.object_value_);
if(achievement.IsAccomplishment(msg.location_code, msg.index_number_))
{
	//업적 달성
}
/* =============================================================== */


	
/* ======== MSG_CG_AREACONQUEST_REWARD_ACK 수신시 ========== */
MSG_CG_AREACONQUEST_REWARD_ACK msg;
if(msg.m_accomplishment_ratio > 0)
{
	achievement.AddAccomplishmentRation(msg.location_code_,msg.m_accomplishment_ratio);
}	
//달성률이 보상인 경우을 제외하고 나머지는 동일하다.
/* ======================================================== */



/* ======== MSG_CG_AREACONQUEST_REWARD_NAK 수신시 ========== */
MSG_CG_AREACONQUEST_REWARD_NAK msg;
//기존과 동일하다.
/* ========================================================= */