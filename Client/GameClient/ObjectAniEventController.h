#pragma once

#include "ObjectEffectData.h"
#include "EventInfo.h"

//------------------------------------------------------------------------------ 
class ObjectAniEventController 
{
public:
    ObjectAniEventController() { current_ani_progress_rate_ = 0.0f; m_EffectData.Initialize(); }
    virtual ~ObjectAniEventController() { DestroyAllEffect(); }

    //! ��ӹ޴� class���� �� ���� �ʿ�
    virtual WzID        GetTrailEffectCode(){ return 0; }
    virtual WzVector    GetTrailBonePos(int index, int start) { WzVector v; SetVector(&v, 0, 0, 0); return v; }
    virtual void        OnFootStep(BOOL b2D = FALSE){}
    virtual void		OnWaterWave() {}
    virtual void		OnWaterEllipse() {}

    // �˱�κ�
    void    StartTrail(int trail_index, const TRAIL_PARAM& trail_param, DWORD object_key);
    void    EndTrail(int trail_index);
    void    UpdateTrail();

    //! ���� �ִϸ��̼� ���� ����
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

    //! ���� �ִϸ��̼� ��������
    //! ĳ���� ���� ��ȭ�� Ż�Ϳ� ���� ���ϴ� ��찡 �ֱ⶧���� ProgressUnitFrame �Ҷ� ������ ���´�.
    float current_ani_progress_rate_;

private:
    ObjectEffectData m_EffectData;
};
