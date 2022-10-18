#ifndef __SCOBJECT_H__
#define __SCOBJECT_H__
#pragma once

#include <Const.h>
struct WzVector;

//==================================================================================================
//  @history
//  090607, waverix - revision code frame
//  100827, waverix - a server object is not derived this class anymore
//                    because of a server and client logic is definitely different system
//                    that don't have shared process using this.
//

//==================================================================================================
//
#if defined(_SERVER)

struct IObjectDispose
{
    virtual ~IObjectDispose() = 0;
};

#elif !defined(_SERVER) //CLIENT

//==================================================================================================
//  <SCObject>
//    12BYTEs
class SCObject
{
public:
    SCObject()
        : m_eObjectType(OBJECT_OBJECT)    // (WAVERIX) (080613) (LOGIC_CHANGE)
        , m_dwObjectKey(0)                // ObjKeyManager에서 발급하는 Object 키
    {
    }

    ~SCObject() {}

    // 상속 받아서 구현해야 할 부분들
    //virtual VOID Init() = 0;
    //virtual VOID Update() = 0;
    //virtual VOID Release() = 0;
#if !defined(_SERVER)
    virtual VOID GetPos(WzVector* pWzVecOut) { __UNUSED(pWzVecOut); }
#endif
    virtual VOID SetPos(const WzVector* pwzVec) { __UNUSED(pwzVec); }
    //////////////////////////////////////////////////////////////////////////

    inline DWORD GetObjectKey() const { return m_dwObjectKey; }
    inline VOID  SetObjectKey(DWORD object_key) { m_dwObjectKey = object_key; }

    inline eOBJECT_TYPE GetObjectType() const { return m_eObjectType; }
    // (WAVERIX) (090607) (ADDIN) Unify object type matching pattern
    inline BOOLEAN IsEqualObjectKind(eOBJECT_TYPE object_type) const;
    //protected:클라이언트 상속 관계를 포함으로 바꾸면서 외부에서 세팅 할 수 있어야 함 
    inline VOID SetObjectType(eOBJECT_TYPE otTYPE) { m_eObjectType = otTYPE; }

    // <FIELDs>
private:
    eOBJECT_TYPE m_eObjectType;  // Object Type(Player, Monster, FieldItem, NPC, etc)
    DWORD        m_dwObjectKey;  // ObjectKeyManager에서 발급하는 Obj키
    //WzVector   m_wzVec;        // 현재 위치
};
//
//==================================================================================================

// (WAVERIX) (090607) (ADDIN) Unify object type matching pattern
inline BOOLEAN
SCObject::IsEqualObjectKind(eOBJECT_TYPE object_type) const
{
    return (m_eObjectType & object_type) == object_type;
}

#endif //CLIENT
//
//==================================================================================================


#endif // __SCOBJECT_H__