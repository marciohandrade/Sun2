Ŭ���̾�Ʈ ����: 


//Ŭ���̾�Ʈ�� ��� ���̵� 

Achievement achievement;

/* ======== MSG_CG_AREACONQUEST_STREAM_CMD ���Ž� ========== */
MSG_CG_AREACONQUEST_STREAM_CMD msg;
achievement.Serialize(msg.achievement_info_, SERIALIZE_STORE);

//���� ��ü ��ȸ
Achievement::ACHIEVEMENT_PART_ITER iter_part = achievement.Begin();
for(; iter_part != achievement.End() ; iter_part++)
{
	AchievementPart::ACHIEVEMENT_ITER iter = iter_part->second.Begin();

	WORD accomplishment_ratio = iter_part->second.GetAccomplishmentRatio(); //���� �޼���

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



/* ======== MSG_CG_AREACONQUEST_VALUE_CHANGE_CMD ���Ž� ========== */
MSG_CG_AREACONQUEST_VALUE_CHANGE_CMD msg;	
achievement.SetAchievementValue(msg.location_code, msg.index_number_, 
								msg.object_index_, msg.object_value_);
if(achievement.IsAccomplishment(msg.location_code, msg.index_number_))
{
	//���� �޼�
}
/* =============================================================== */


	
/* ======== MSG_CG_AREACONQUEST_REWARD_ACK ���Ž� ========== */
MSG_CG_AREACONQUEST_REWARD_ACK msg;
if(msg.m_accomplishment_ratio > 0)
{
	achievement.AddAccomplishmentRation(msg.location_code_,msg.m_accomplishment_ratio);
}	
//�޼����� ������ ����� �����ϰ� �������� �����ϴ�.
/* ======================================================== */



/* ======== MSG_CG_AREACONQUEST_REWARD_NAK ���Ž� ========== */
MSG_CG_AREACONQUEST_REWARD_NAK msg;
//������ �����ϴ�.
/* ========================================================= */