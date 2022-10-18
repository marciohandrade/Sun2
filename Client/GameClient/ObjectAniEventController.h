#pragma once

#include "ObjectEffectData.h"
#include "EventInfo.h"

//------------------------------------------------------------------------------ 
class ObjectAniEventController 
{
public:
    ObjectAniEventController() { current_ani_progress_rate_ = 0.0f; m_EffectData.Initialize(); }
    virtual ~ObjectAniEventController() { DestroyAllEffect(); }

    //! 상속받는 class에서 상세 구현 필요
    virtual WzID        GetTrailEffectCode(){ return 0; }
    virtual WzVector    GetTrailBonePos(int index, int start) { WzVector v; SetVector(&v, 0, 0, 0); return v; }
    virtual void        OnFootStep(BOOL b2D = FALSE){}
    virtual void		OnWaterWave() {}
    virtual void		OnWaterEllipse() {}

    // 검기부분
    void    StartTrail(int trail_index, const TRAIL_PARAM& trail_param, DWORD object_key);
    void    EndTrail(int trail_index);
    void    UpdateTrail();

    //! 현재 애니메이션 진행 정도
    float current_ani_progress_rate() const { return current_ani_progress_rate_; }

protected:
    void    InitializeEffect();
    void    DestroyAllEffect();
    void    DestroyObjectEffect(int index, int iBuffer);
    void    MoveRemainEffect(int index);
    void    DestroyAllLoopEffect();
    int     GetTrailEffectAttachmentIndex(int TrailIndex) { return m_EffectData.GetTrailAttachmentIndex(TrailIndex); }

    //void    ProcessEffect(float fAddFrame, CWzUnitDraw* unit_draw, const WzVector& visible_pos, const WzVector& direction);
    void    UpdateEffectPos(CWzUnitDraw* unit_draw, const WzVector& visible_pos, const WzVector& direction);

    void    ProcessSoundEvent(const SoundEventInfo& info, const WzVector& visible_pos);
    void    ProcessEffectEvent(const EffectEventInfo& info, 
                                CWzUnitDraw* unit_draw,
                                const WzVector& visible_pos, 
                                const WzVector& direction, 
                                DWORD object_key);
	void    ProcessCustomEvent(const CustomEventInfo& Info, const WzVector& pos);

    ObjectEffectData& effectdata() { return m_EffectData; }

    //! 현재 애니메이션 진행정도
    //! 캐릭터 상태 변화가 탈것에 의해 변하는 경우가 있기때문에 ProgressUnitFrame 할때 저장해 놓는다.
    float current_ani_progress_rate_;

private:
    ObjectEffectData m_EffectData;
};
