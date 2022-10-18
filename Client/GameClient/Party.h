#ifndef		_PARTY_H
#define		_PARTY_H
//------------------------------------------------------------------------------
/**
    @class Party

    ��Ƽ Ŭ����
    ��Ƽ �ʴ�, Ż��, ���� Ż�� ���� ó���ϴ� Ŭ����

    (C) 2005 ckbang
*/
#include "PacketStruct_CG.h"
#include "ResultCode.h"
#include "InterfaceManager.h"

using namespace RC;

class Party
{
public:
    Party();
    ~Party();

    void                    Init ();                    /// ��Ƽ �ʱ�ȭ
    void                    ShowPartyInfoDlgs (BOOL bFlag); /// ��Ƽ ���� ���̾�α׵� ����
    void                    SetInfos (MSG_CG_PARTY_MEMBER_INFO_CMD *pAck);  /// ��Ƽ ������ ä���
    void                    AddInfo (MSG_CG_PARTY_MEMBER_JOIN_BRD *pAck);   /// �߰� ���� ä���
    void                    DelInfo (DWORD objKey);
    void					ClearInfos ();              /// ��Ƽ ���� �ʱ�ȭ
    void                    SetMaster (DWORD objKey);   /// ��Ƽ�� ���� ����
    void                    LeaveParty ();              /// ��Ƽ ������
    
    PARTY_MEMBER_INFO*      GetInfo (DWORD objKey);     /// ��Ƽ�� ���� ���
    DWORD                   GetPartyMasterObjKey ();    /// ��Ƽ�� Ű�� ���
    BYTE                    GetDistributionType ();     /// ��Ƽ �й��� ���
    BYTE                    GetPartyNum ();             /// ��Ƽ ������ �� ���
    InterfaceManager::eDIALOG_ID GetStartPartyDialog ();     /// ��Ƽâ ù��° �� ���
    const char *            GetInviterName ();          /// �ʴ��� ����� �̸�
    DWORD                   GetInviterKey ();           /// �ʴ��� ����� Ű��
    
    BOOL                    IsMaster ();                /// ��Ƽ���ΰ�
    BOOL                    IsVisable ();               /// ��Ƽ �������̽��� Ȱ��ȭ �Ǿ��°�
    BOOL                    VisableSwap ();             /// ��Ƽ �������̽��� Ȱ��ȭ ���� �ڵ���ȯ
    DWORD                   name2key (char *pname);     /// ��Ƽ���� �̸��� Ű������ ������
    char *                  key2name (DWORD key);       /// ��Ƽ���� Ű���� �̸����� ������
    
    /// �˸�
    void                    Notice (TCHAR *pnotice, WzColor Color=WzColor_RGBA( 255, 255, 0, 255 ));
    /// ����
    void                    Reason (ePARTY_RESULT eReason, WzColor Color=WzColor_RGBA(   0,  0,  255, 255 ));

    //
    // PARTY ACTION
    //
    bool                    actForcedExplusion (DWORD objKey); /// ���� Ż��
    bool                    actLeave ();                       /// Ż��
    void                    actInvite (char *pname);           /// �ʴ�
    void                    actAskInviting(char *strInviterName, DWORD dwInviterKey); /// �ʴ� ��������
    void                    actInvitedYes (); /// �ʴ뿡 ����
    void                    actInvitedNo ();                    /// �ʴ뿡 ����
    bool                    actMigration (DWORD objKey);       /// ���� ����
    bool                    actToggleShowBuffInfo ();          /// ���� ���� ���̱�/�����
    bool                    actDestroyParty ();                /// ��Ƽ ��ü

    //
    // PARTY PARSE PACKET
    //
    void                    Parse (MSG_BASE * pMsg);

protected:
    BOOL                    m_bVisable;             /// ��Ƽ �������̽� Ȱ��ȭ �÷�
    DWORD				    m_dwMasterObjKey;		/// ��Ƽ�� ������Ʈ Ű
    BYTE				    m_byDistributionType;	/// ������ �й���
    BYTE				    m_byMemberNum;			/// ��Ƽ�� ��(��Ƽ�� ����)
    vector<PARTY_MEMBER_INFO>  m_MemberInfo;        /// ��Ƽ ���������� ���
    BOOL                    m_bShowBuffInfo;        /// ��Ƽ ����â ���̱�
    std::string             m_strInviterName;       /// �ʴ��� ��� �̸�
    DWORD                   m_dwInviterKey;         /// �ʴ��� ��� Ű
};

//------------------------------------------------------------------------------
/**
*/
inline
BOOL
Party::VisableSwap()
{
    if (m_bVisable)
        m_bVisable = FALSE;
    else
        m_bVisable = TRUE;

    return m_bVisable;
}

//------------------------------------------------------------------------------
/**
*/
inline
BOOL
Party::IsVisable()
{
    return m_bVisable;
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
Party::GetPartyMasterObjKey()
{
    return m_dwMasterObjKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char *
Party::GetInviterName()
{
    return m_strInviterName.c_str();
}

//------------------------------------------------------------------------------
/**
*/
inline
DWORD
Party::GetInviterKey()
{
    return m_dwInviterKey;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE
Party::GetDistributionType()
{
    return m_byDistributionType;
}

//------------------------------------------------------------------------------
/**
*/
inline
BYTE
Party::GetPartyNum()
{
    return m_MemberInfo.size();
}

//------------------------------------------------------------------------------
#endif	//	_PARTY_H
