//------------------------------------------------------------------------------
/**
	@Name : Sound.h 
	@Desc :  ���� ���̺귯���� �ε����ְ� 
			��ü���� ���� ������ ������ �ִ� Ŭ����	
			WZASound�� �޼���ǥ�踦 ����ϱ� ������ 
			3D sound Library�� ������ ��ǥ�踦 ����ϴ� �����
			������ ��ǥ��� �Է¹����� �޼���ǥ��� ��ȯ�Ͽ� ����Ѵ�.

				z					y					
				^ y					^ z					
				|/         <-->		|/              
				+----> x			+----> x			
				������				 �޼�			

	@Author	: ���翵(y2jinc@webzen.co.kr)
	@Date	:	[2/3/2005]
				
	@remark
		- WZAStartup �Լ� ����	
		- ������ ���߰�
		- 3D ���� �� �Լ�	
		- ȯ�溯�� ���� �߰�	[9/9/2005] 
*/

#ifndef		_SOUND_H
#define		_SOUND_H

#include "Sound_Define.h"
#include <Singleton.h>

#define DEFAULT_SOUND_DIR		"\\Data\\Sound"
#define USE_SOUND_LIBRARY		TRUE


class Sound : public util::Singleton<Sound>
{
public:

	Sound( VOID );
	~Sound( VOID );

public:
	BOOL Create( WZSNDINFO * pWzSndInfo );
	void Destroy();

public:
	// Set Functions 
	void			SetUseSoundLibrary(BOOL bFlag);
	void			SetUseEffectSound(BOOL bChoice);
	void			SetUseBGMSound(BOOL bChoice);
	void			SetUseEAXSound(BOOL bFlag);

	void			SetEnvironmentID( SND_ENV_ID SndID);
	void			DisableEnvironment();

	void			SetAmbientEffectVolume(float fVolume)
	{ m_fAmbientEffectVolumeScale = fVolume; }

	void			SetEffectVolume(float fVolume)
	{ m_fEffectVolumeScale = fVolume; }

	void			SetBGMVolume(float fVolume)
	{ 
		m_fBGMVolumeScale = fVolume; 
	}

	void			SetMasterVolume(float fVolume, BOOL bSave);
	void			SetWaveVolume(float fVolume, BOOL bSave);
	int				GetMasterVolume();
	int				GetWaveVolume();

	void			SetListenerPos(float x, float y, float z);
	void			SetListernerOrientation( float fDirx, float fDirY, float fDirZ, 
								  float fUpX, float fUpY, float fUpZ );

    void            Update();
    void            CacheOn(DWORD maintain_milliseconds);
    void            CacheOff();

    int             calltick_updatelistener() const { return calltick_updatelistener_; } 
    int             calltick_cache() const { return calltick_cache_; }
    void            set_calltick_updatelistener(int millisecond) { calltick_updatelistener_ = millisecond; }
    void            set_calltick_cache(int millisecond) { calltick_cache_ = millisecond; }

	// Get Functions
	BOOL			IsUseSoundLibrary(BOOL bFlag)
	{	return this->m_bSoundFlag;	}

	BOOL			IsUseEffectSound(void) const
	{	return BOOL(m_bIsUseEffectSound); }

	BOOL			IsUseBGMSound(void) const
	{	return BOOL(m_bIsUseBGMSound); }

	BOOL			IsUseEAX(void) const
	{	return BOOL(m_bIsUseEAX); }

	float			GetAmbientEffectVolume( VOID ) const
	{ return m_fAmbientEffectVolumeScale; }

	float			GetEffectVolume( VOID ) const
	{ return m_fEffectVolumeScale; }

	float			GetBGMVolume( VOID ) const
	{ return m_fBGMVolumeScale; }

	float			GetMasterVolume() const
	{ return m_fMasterVolumeScale; }

	float			GetWaveVolume() const
	{ return m_fWaveVolumeScale;	}


	const char *	GetCurInfoText() const
	{ return &m_szSoundInfo[0];}

	void			GetListenerPos( WzVector * pWzPos );
	D3DVECTOR *		GetListenerPos(void);
	D3DVECTOR *		GetListenerDir(void);
	D3DVECTOR *		GetListenerUp(void);

	void	StopEffectSoundAll();

	void	UnloadSounds();


private:
	char			m_szSoundInfo[0xff];	// ��Ÿ ������ ��� ���� �ؽ�Ʈ

	// ���� ���̺귯���� ����Ѵ� ���Ѵ�.(���� ī�尡 ���ų�, �ǵ������� �Ⱦ������� �÷���);
	BOOL			m_bSoundFlag;			
	BOOL			m_bIsUseBGMSound;
	BOOL			m_bIsUseEffectSound;		
	BOOL			m_bIsUseEAX;

	float			m_fBGMVolumeScale;
	float			m_fEffectVolumeScale;		// multiply sound volume pitch by this factor.
	float			m_fAmbientEffectVolumeScale;		// multiply sound volume pitch by this factor.

	float			m_fMasterVolumeScale;
	float			m_fWaveVolumeScale;
	float			m_fMicVolumeScale;

	// if calculated 3d spatial sound volume is  0.5f, final output volume is 0.75f ( 0.5f * 1.5f).
	// this optional parameter is used to tune preset volume which defined in sound_table scripts.

	int				m_iForce;					// force to play sound, granted that sound is attanuated almost mute.
	int				m_iEffectFrequency;			// optional, frequency ( default 0 /  min 100   /  max 100000)
	int				m_iBGMFrequency;

	D3DVECTOR		m_vListenerPos;
	D3DVECTOR		m_vListenerDir;
	D3DVECTOR		m_vListenerUp;

	WZSNDINFO		m_WZAInfo;

	float			m_fValidSoundDistance;		// 3D ���� �÷��̽� ���� �߻� ��ȿ �Ÿ�

    //! ���� �и�������
    int             calltick_updatelistener_;
    int             calltick_cache_;
};

#endif	//	_SOUND_H
