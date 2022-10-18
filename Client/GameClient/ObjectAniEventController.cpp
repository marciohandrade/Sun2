#include "SunClientPrecompiledHeader.h"

#include "ObjectAniEventController.h"
#include "GlobalFunc.h"
#include "SoundEffect.h"
#include "Camera.h"
#include "Hero.h"

//------------------------------------------------------------------------------
void ObjectAniEventController::InitializeEffect()
{
	m_EffectData.Initialize();
}
//------------------------------------------------------------------------------
void ObjectAniEventController::DestroyAllEffect()
{
    m_EffectData.ReleaseAllEffect();
}
//------------------------------------------------------------------------------
void ObjectAniEventController::DestroyObjectEffect(int index, int iBuffer)
{
	m_EffectData.ReleaseEffect(index, iBuffer);
}
//------------------------------------------------------------------------------
void ObjectAniEventController::MoveRemainEffect(int index)
{
	m_EffectData.ReleaseCurEffect(index);
}
//------------------------------------------------------------------------------
void ObjectAniEventController::DestroyAllLoopEffect()
{
    m_EffectData.ReleaseLoopEffect();
}
//------------------------------------------------------------------------------
void ObjectAniEventController::StartTrail(int trail_index, const TRAIL_PARAM& trail_param, DWORD object_key)
{
    if ((trail_index >= 0) && (trail_index < MAX_TRAIL))
    {
        EndTrail(trail_index);

        if (g_pEffectManager)
        {
            HANDLE hEffect = GlobalFunc::CreateEffectBeUpToOption(GetTrailEffectCode(),
                                        GetTrailBonePos(trail_index, 1),
                                        object_key);

            m_EffectData.StartTrail(trail_index, trail_param, hEffect);
        }

        UpdateTrail();
    }
    else
    {
	    assert(trail_index >= 0 && trail_index < MAX_TRAIL);
    }
}

//------------------------------------------------------------------------------
void ObjectAniEventController::EndTrail(int trail_index)
{
	m_EffectData.EndTrail(trail_index);
}

//------------------------------------------------------------------------------
void ObjectAniEventController::UpdateTrail()
{
	for(int i = 0; i < m_EffectData.GetTrailCount(); ++i)
	{
		if (m_EffectData.IsEnableTrail(i) == false)
        {
			continue;
        }
		m_EffectData.UpdateTrail(i, GetTrailBonePos(i, 1), GetTrailBonePos(i, 0));
	}
}

////------------------------------------------------------------------------------
//void ObjectAniEventController::ProcessEffect(float fAddFrame, CWzUnitDraw* unit_draw, const WzVector& visible_pos, const WzVector& direction)
//{
//    static WzEvent ani_event;
//    ZeroMemory(&ani_event, sizeof(ani_event));
//
//    // �ִϸ��̼� ������ó�� (Event���޾Ƽ� ����Ʈ,����,Ŀ���ҵ�ó��) 
//    g_pSunRenderer->ProgressUnitFrame( unit_draw, fAddFrame, &ani_event, &current_ani_progress_rate_);
//
//    EventInfo event_info(ani_event);
//
//#ifdef _DEV_VER
//    if( event_info.IsGood() == false )
//    {
//        if ( CWzUnitDraw* pBase = GetBasePart() )
//        {
//            if( WzAnimationInfo* pAnimationInfo = pBase->GetCurrentAnimationInfo() )
//            {
//                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("event_max: %s"), pAnimationInfo->m_lpszFileName);
//            }
//        }
//    }
//#endif // _DEV_VER
//
//
//    // �����̺�Ʈó��
//    if ((dwTick != 0) && ((g_pHero != this) || (g_pHero->GetShow() == TRUE)))
//    {
//        // ���ΰ� ���϶���
//        const size_t event_count = event_info.GetSoundEventCount();
//        for (size_t i = 0; i != event_count; ++i)
//        {
//            ProcessSoundEvent(event_info.GetSoundEvent(i), visible_pos);
//        }
//    }
//
//    // �̺�Ʈ���� üũ
//    if (bIgnoreEvent == FALSE)
//    {
//        // ����Ʈ �̺�Ʈ
//        DWORD object_key = IsKindOfObject(MAP_OBJECT) ? (0) : (GetObjectKey());
//        const size_t event_count = event_info.GetEffectEventCount();
//        for (size_t i = 0; i != event_count; ++i)
//        {
//            const EffectEventInfo& info = event_info.GetEffectEvent(i);
//            //if ((IsAnimated() == TRUE) || 
//            //    (info.IsTrail_LeftToRight() && info.IsDestroy()) || 
//            //    (info.IsTrail_RightToLeft() && info.IsDestroy()) ||
//            //    (info.IsDestroyAll() == true) ||
//            //    (info.IsDestroy() == true))
//            //{
//            //}
//            //else
//            //{
//            //    AnimateUnit();
//            //}
//
//            ProcessEffectEvent(event_info.GetEffectEvent(i), 
//                unit_draw, 
//                visible_pos, 
//                direction, object_key);
//        }
//
//        // Ŀ���� �̺�Ʈ
//        if (ani_event.m_iNumCustom > 0)
//        {
//            const size_t event_count = event_info.GetCustomEventCount();
//            for (size_t i = 0; i != event_count; ++i)
//            {
//                ProcessCustomEvent(event_info.GetCustomEvent(i), visible_pos);
//            }
//        }
//    }
//
//
//}

//------------------------------------------------------------------------------
void ObjectAniEventController::UpdateEffectPos(CWzUnitDraw* unit_draw, const WzVector& visible_pos, const WzVector& direction)
{
	m_EffectData.UpdateEffect(unit_draw, visible_pos, direction);
}
//------------------------------------------------------------------------------
// ���� �̺�Ʈ ó��
void ObjectAniEventController::ProcessSoundEvent(const SoundEventInfo& info, const WzVector& visible_pos)
{
	g_SoundEffect.Play(info.GetSoundIndex(), (WzVector*)(&visible_pos));
}


//------------------------------------------------------------------------------
// ����Ʈ �̺�Ʈ ó��
void ObjectAniEventController::ProcessEffectEvent(const EffectEventInfo& info, 
                                                CWzUnitDraw* unit_draw,
                                                const WzVector& visible_pos, 
                                                const WzVector& direction, 
                                                DWORD object_key)
{
	if (info.IsTrail_LeftToRight() == true)
	{
		if (info.IsDestroy())
		{
			EndTrail(1);
		}
		else
		{
			TRAIL_PARAM param;
			param.iAttachmentIndex = 1;
			param.iTailNum = 40;
			StartTrail(1, param, object_key);
		}
	}
	else if (info.IsTrail_RightToLeft() == true)
	{
		if (info.IsDestroy())
		{
			EndTrail(0);
		}
		else
		{
			TRAIL_PARAM param;
			param.iAttachmentIndex = 0;
			param.iTailNum = 40;
			StartTrail(0, param, object_key);
		}
	}
	else if (info.IsDestroyAll()  == true)
	{
		// ��� ����Ʈ ����
		DestroyAllEffect();
	}
	else if (info.IsDestroy() == true)
	{
		// ������ �ƴѰ�� �ٷ��������ʰ� �ڿ����� ���̱����� �Űܳ���
		MoveRemainEffect(info.GetEventIndex());

		// �ش� ����Ʈ ����
		DestroyObjectEffect(info.GetEventIndex(), CUR_EFFECT);
	}
	else
	{
		m_EffectData.BeginEffect(unit_draw, visible_pos, direction, info, object_key);
	}
}

//------------------------------------------------------------------------------
// Ŀ���� �̺�Ʈ ó��
void ObjectAniEventController::ProcessCustomEvent(const CustomEventInfo& Info, const WzVector& pos)
{
	if (Info.IsFootStep() == true)
	{
		OnFootStep();
	}
	else if (Info.IsWaterWave() == true)
	{
		OnWaterWave();
	}
	else if (Info.IsCameraWave() == true)
	{
		if (g_pHero != NULL)
		{
			WzVector distance = g_pHero->GetPosition() - pos;
			if (VectorLength(&distance) > 15.f)
			{
				return;
			}

			g_Camera.SetVibeType(Camera::VIBE_TYPE_ALL);
			g_Camera.StartVibration(Info.GetCameraWaveDuration(), Info.GetCameraWavePower());
		}
	}
}
