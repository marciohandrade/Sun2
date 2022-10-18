
#ifndef		_BATTLE_H
#define		_BATTLE_H

#include <stdlib.h>
#include <ProgramCommon/WzDataType.h>

//---------------------------------------------------------------------------------------
//
//		��ä�� ��� ���� ���� �˻�
//		
//			- pVecSrcPos ; ���� ��ġ
//			- pVecSrcDir ; ������ �ٶ󺸴� ���⺤��
//			- pVecDest   ; Ư�� ��ġ
//
//
//		�ۼ��� : ���翵
//
//---------------------------------------------------------------------------------------

bool IsPositionInSector( WzVector * pwvCheckPos, 
						 WzVector * pwvSrcPos, 
						 WzVector * pwvSrcDir,
						 float fRadian = ( WZ_PI / 3.f ) );


//---------------------------------------------------------------------------------------
//
//		���Գ� Ư�� ����Ʈ ���԰��� �˻�
//		(!! �� pVecSrcDir �� �븻������� ���Ͷ�� �����Ѵ�!! )
//			- pVecSrcPostion ; ���� ��ġ
//			- pVecSrcDir	 ; ������ �ٶ󺸴� �븻 ���⺤��
//			- pVecDest		 ; Ư�� ��ġ 
//		
//													- 2005. 2. 1
//---------------------------------------------------------------------------------------
bool IsPositionInCircleCone( 
							WzVector * pVecSrcPostion, 
							WzVector * pVecSrcDir, 
							WzVector * pVecDest, 
							float fRadian = ( WZ_PI / 3.f ) );

inline float GetRandProb() { return (float)( rand() % 10000 * 0.01 ); }

// ���� �̱�.. �ۼ�Ʈ�� �Է��ϸ� Ȯ���� ���� TRUE�� FALSE�� �����Ѵ�.
inline BOOL DrawLots( BYTE byPercent ) { return ( rand() % 100 + 1 ) <= byPercent; }

#endif	//	_BATTLE_H
