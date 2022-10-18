#pragma once


typedef struct _TRAIL_PARAM
{
    int         iBoneIndex;
    int         iAttachmentIndex;
    float       fLength;
    int         iTailNum;
    WzVector    vOffset;
    BOOL        bUpdated;
} TRAIL_PARAM;


typedef struct _EFFECT_PARAM
{
    int				iBoneIndex;	
    WzVector		vOffset;
    WzQuaternion	quatRot;
    WzVector		vScale;
    BOOL			bRotateWithBone;
    BOOL            bOnlyPosition;
    WzID			EffectID;
    BOOL			bLoopEffect;
    HANDLE			hEffect;
} EFFECT_PARAM;


class EffectEventInfo;


const int MAX_TRAIL = 2;
const int MAX_OBJECT_EFFECT = 10;
const int MAX_EFFECT_BUFFER = 2;
const int CUR_EFFECT  = 0;
const int PRE_EFFECT = 1;


class ObjectEffectData
{
public:
	ObjectEffectData();

	void Initialize();
	void Release();

private:
	BOOL                m_bTrailActivate[MAX_TRAIL];
	HANDLE				m_hTrailEffect[MAX_TRAIL];
	TRAIL_PARAM			m_arTrailParam[MAX_TRAIL];
	EFFECT_PARAM        m_EffectParam[MAX_EFFECT_BUFFER][MAX_OBJECT_EFFECT];

public:

	// Trail
	int GetTrailCount() { return MAX_TRAIL; }
	int GetTrailAttachmentIndex( int TrailIndex );

	bool IsEnableTrail( int TrailIndex ) { return m_hTrailEffect[TrailIndex] != INVALID_HANDLE_VALUE; }

	void StartTrail( int TrailIndex, const TRAIL_PARAM& TrailParam, HANDLE hEffect );
	void EndTrail( int TrailIndex );

	void UpdateTrail( int TrailIndex, const WzVector& Position1, const WzVector& Position2 );



	// Effect
	int GetEffectCount() { return MAX_OBJECT_EFFECT; }
	int GetEffectBufferCount() { return MAX_EFFECT_BUFFER; }

	BOOL BeginEffect( CWzUnitDraw* pUnitDraw, const WzVector& Positioin, const WzVector& Direction, const EffectEventInfo& EventInfo, DWORD ObjectKey );
	void UpdateEffect( CWzUnitDraw* pUnitDraw, const WzVector& Position, const WzVector& Direction );

	EFFECT_PARAM& GetEffect(int Index, int BufferIndex ) { return m_EffectParam[BufferIndex][Index]; }

	void ReleaseAllEffect();
	void ReleaseLoopEffect();
	void ReleaseEffect( int Index, int BufferIndex );

	void ReleaseCurEffect(int Index);
};

