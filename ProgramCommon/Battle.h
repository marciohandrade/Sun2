
#ifndef		_BATTLE_H
#define		_BATTLE_H

#include <stdlib.h>
#include <ProgramCommon/WzDataType.h>

//---------------------------------------------------------------------------------------
//
//		부채꼴 모양 각도 범위 검사
//		
//			- pVecSrcPos ; 기준 위치
//			- pVecSrcDir ; 기준이 바라보는 방향벡터
//			- pVecDest   ; 특정 위치
//
//
//		작성자 : 유재영
//
//---------------------------------------------------------------------------------------

bool IsPositionInSector( WzVector * pwvCheckPos, 
						 WzVector * pwvSrcPos, 
						 WzVector * pwvSrcDir,
						 float fRadian = ( WZ_PI / 3.f ) );


//---------------------------------------------------------------------------------------
//
//		원뿔내 특정 포인트 포함관계 검사
//		(!! 단 pVecSrcDir 은 노말라이즈된 벡터라고 가정한다!! )
//			- pVecSrcPostion ; 기준 위치
//			- pVecSrcDir	 ; 기준이 바라보는 노말 방향벡터
//			- pVecDest		 ; 특정 위치 
//		
//													- 2005. 2. 1
//---------------------------------------------------------------------------------------
bool IsPositionInCircleCone( 
							WzVector * pVecSrcPostion, 
							WzVector * pVecSrcDir, 
							WzVector * pVecDest, 
							float fRadian = ( WZ_PI / 3.f ) );

inline float GetRandProb() { return (float)( rand() % 10000 * 0.01 ); }

// 제비 뽑기.. 퍼센트를 입력하면 확률에 의해 TRUE나 FALSE를 리턴한다.
inline BOOL DrawLots( BYTE byPercent ) { return ( rand() % 100 + 1 ) <= byPercent; }

#endif	//	_BATTLE_H
