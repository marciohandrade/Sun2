// elenodeinfo.h

#ifndef _EFFECT_ELENODEINFO_H_
#define _EFFECT_ELENODEINFO_H_

#include "../typedef.h"

struct St_Create_Info;
struct St_Create_Info;
struct St_Move_Info;
struct St_Change_Info;
struct St_Visual_Info;

struct St_Node_Info
{   
    char szVer[11];     // 이펙트 버전정보
    int m_iType;        // 이펙트 타입
    RECT m_Rect;        // 영역 
    long m_lnPos;       // 위치  
};

struct St_Ele_Node
{
    St_Node_Info m_NodeInfo;
    St_Create_Info* m_pCreate;
    St_Move_Info* m_pMove;
    St_Visual_Info* m_pVisual;  
    St_Change_Info* m_pChange;
};

#endif // _EFFECT_ELENODEINFO_H_