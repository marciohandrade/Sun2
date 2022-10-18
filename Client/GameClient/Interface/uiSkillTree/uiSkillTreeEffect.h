#pragma once
#ifndef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2


class Command;


struct OpenLevelEffect
{
    enum AnimationState
    {
        kAniNone,
        kAniStart,          //������ �ٿ�
        kBlinkChain,
        kTornadoAppear,
        kChainMove,
        kChainMoveStop,
        kBlockBlink,
        kBlockFadeOut,
        kBlockFadeOutStop,  
        kTornadoFadeOut,
        kBlinkChainFast,
        kChainFadeOut,
        kAniEnd,
    };

	int ani_state;
	DWORD ani_time;
	bool Init_tornado;
    DWORD tornado_time;
	//DWORD tornado_tick;
};



class uiSkillTreeEffect
{
private:
	HANDLE skill_effect_;
	HANDLE skill_ui_;
    HANDLE arrow_texture_;
    HANDLE skill_ui_2_;

	OpenLevelEffect open_effect_;

    int start_tab_;

    int cur_tab_;				// ������ �ܰ�
    int cur_stat_;				// �ܰ躰 ȹ�� ����Ʈ
    int next_tab_;              // ���� �ܰ� (Ȥ�� �����ܰ�), ü���� ��ġ�� ���
    float gauge_position_;      // �������� ��ġ
    WzColor render_color_;      // ����̵� ǥ�� ���İ� (0.0f~1.0f)

    RECT dest_rect_;			// ȭ����� ��ǥ

    DWORD animation_time_;      // ���۵� �ð�
    DWORD animation_duration_;  // ���� ������ ���� �ð�


private:
	HANDLE skill_effect(){ return skill_effect_;}
	HANDLE skill_ui(){ return skill_ui_;}

    HANDLE GetArrowTexture() { return arrow_texture_; }

	OpenLevelEffect& open_effect(){return open_effect_;}

    Command* m_pAnimationChangedCommand;   // ���� ���°� ����ɶ��� ���ν���

public:
	uiSkillTreeEffect();
    ~uiSkillTreeEffect();

	void Init();
	void Release();

    void RenderOpenEffect( const RECT& dest_rect );
    void RenderMaxLevel( const RECT& dest_rect );

	void SetTotalSkillLevel( int total_level, bool data_clear );

    int GetGaugeSlideHeight();
	bool IsPlayingEffect() { return open_effect().ani_state != OpenLevelEffect::kAniNone; }


    void SetState( OpenLevelEffect::AnimationState state );
    void SetAnimationChangedCommand( Command* command ) { m_pAnimationChangedCommand = command; }
    OpenLevelEffect::AnimationState GetOpenLevelEffectState() { return (OpenLevelEffect::AnimationState)open_effect().ani_state; }
    int GetOpenLevelEffectAnimationTime() { return open_effect().ani_time; }

private:
	// ������ ����Ʈ ����
    void RenderBlocks();
	void RenderTornado();
	void RenderChain();
	void ProcessOpenState();
    void UpdateGaugePosition();



private:
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // ȭ��ǥ
    class SkillTreeArrowInfo
    {
    private:
        enum eArrowState
        {
            ARROW_STATE_HIDE,
            ARROW_STATE_ANIMATION,
            ARROW_STATE_VISIBLE,
        };

    public:
        SkillTreeArrowInfo()
        {
            arrow_state_ = ARROW_STATE_HIDE;

            arrow_position_ = 0;
            arrow_type_ = 0;

            arrow_skill_level_ = 0;
            arrow_skill_class_code_ = 0;

            animation_start_time_ = 0;
        }

        void SetArrowSkillInfo( int skill_class_code, int skill_level ) { arrow_skill_class_code_ = skill_class_code; arrow_skill_level_ = skill_level; }
        void SetArrowPosition( int pos ) { arrow_position_ = pos; } 
        void SetArrowType( int type ) { arrow_type_ = type; }

        int GetArrowPosition() { return arrow_position_; }
        int GetArrowType() { return arrow_type_; }
        int GetSkillClassCode() { return arrow_skill_class_code_; }
        int GetSkillLeve() { return arrow_skill_level_; }

        void SetVisibleState() { arrow_state_ = ARROW_STATE_VISIBLE; }
        void SetHideState() { arrow_state_ = ARROW_STATE_HIDE; }
        bool IsHideState() { return arrow_state_ == ARROW_STATE_HIDE; }

        void StartAnimation()
        {
            if( arrow_state_ != ARROW_STATE_HIDE )
                return;

            arrow_state_ = ARROW_STATE_ANIMATION;
            animation_start_time_ = GetCurrentTime();
        }

        float GetImageRate()
        {
            if( arrow_state_ == ARROW_STATE_HIDE )
                return 0.f;

            if( arrow_state_ == ARROW_STATE_VISIBLE )
                return 0.7f;

            static DWORD animation_duration = 3000; // 3��

            DWORD current_time = GetCurrentTime();
            if( current_time - animation_start_time_ < animation_duration )
            {
                DWORD delta_time = current_time - animation_start_time_;
                float time = (float)delta_time * WZ_PI / 500.f;
                return cos( time ) * 0.5f + 0.5f;
            }
            else
            {
                arrow_state_ = ARROW_STATE_VISIBLE;
                return 0.7f;
            }

            return 0.f;
        }

    private:
        eArrowState arrow_state_;       // ȭ��ǥ�� ���� (��ǥ��, ����, ǥ��)
        int arrow_position_;            // ȭ��ǥ�� ��ġ
        int arrow_type_;                // � ����� ȭ��ǥ �ΰ�

        int arrow_skill_level_;         // ��ų�� ����
        int arrow_skill_class_code_;    // ��ų�� Ŭ�����ڵ�

        DWORD animation_start_time_;    // �ִϸ��̼� ���� �ð�
    };

private:
    std::vector<SkillTreeArrowInfo> arrow_list_;

    void InitArrowResource();
    void RenderArrows();

public:
    void AddArrowInfo( int arrow_position, int arrow_type, int skill_class_code, int skill_level );
    void SetAnimationArrow( int skill_class_code, int skill_level );
    void SetVisibleArrow( int skill_class_code, int skill_level );
    void ResetArrow();



private:
    //////////////////////////////////////////////////////////////////////////////////////////////////
    // ���ǥ��
    class SkillTreeLockInfo
    {
    private:
        enum eLockState
        {
            LOCK_STATE_VISIBLE,
            LOCK_STATE_ANIMATION,
            LOCK_STATE_HIDE,
        };

    public:
        SkillTreeLockInfo()
        {
            state_ = LOCK_STATE_VISIBLE;
            position_ = 0;
            skill_class_ = 0;
            fateout_begin_time_ = 0;
        }

        void SetPosition( int position ) { position_ = position; }
        void SetSkillClass( int skill_class ) { skill_class_ = skill_class; }


        int GetPosition() const { return position_; }
        int GetSkillClass() const { return skill_class_; }

        void BeginFadeout() 
        {
            if( state_ == LOCK_STATE_VISIBLE )
            {
                state_ = LOCK_STATE_ANIMATION;
                fateout_begin_time_ = GetCurrentTime();
            }
        }

        void SetVisibleState() { state_ = LOCK_STATE_VISIBLE; }
        void SetHideState() { state_ = LOCK_STATE_HIDE; }

        float GetImageRate() 
        {
            if( state_ == LOCK_STATE_VISIBLE )
            {
                return 1.f;
            }
            else if( state_ == LOCK_STATE_HIDE )
            {
                return 0.f;
            }

            static DWORD fateout_duration = 1000;  // 1�ʿ� ���� �������

            //if( fateout_begin_time_ == 0 )
            //{
            //    // ��� ǥ��
            //    return 1.0f;
            //}


            DWORD current_time = GetCurrentTime();

            DWORD delta_time = current_time - fateout_begin_time_;
            if( delta_time >= fateout_duration )
            {
                // ���̵� �ƿ� ����
                state_ = LOCK_STATE_HIDE;
                return 0.0f;
            }

            return (float)(fateout_duration - delta_time) / (float)fateout_duration;
        }

    private:
        eLockState state_;
        int position_;                  // ��ġ
        int skill_class_;               // ��ų �ĺ��� ���� Ŭ����
        DWORD fateout_begin_time_;
    };

private:
    std::vector<SkillTreeLockInfo> lock_info_array_;

public:
    void RenderLockImage();

    void AddLockInfo( int position, int skill_class )
    {
        SkillTreeLockInfo info;
        info.SetPosition( position );
        info.SetSkillClass( skill_class );
        lock_info_array_.push_back( info );
    }

    void BegionFadeout( int required_skill_class )
    {
        for( size_t i=0;i!=lock_info_array_.size();++i)
        {
            if( lock_info_array_.at(i).GetSkillClass() == required_skill_class )
            {
                lock_info_array_[i].BeginFadeout();
            }
        }
    }

    void ResetLock();

    void HideLock( int skill_class )
    {
        for( size_t i=0;i<lock_info_array_.size();++i)
        {
            if( lock_info_array_[i].GetSkillClass() == skill_class )
            {
                lock_info_array_[i].SetHideState();
            }
        }
    }
};

#endif //_NA_20111213_SKILLTREE_VISUAL_RENEWAL2
