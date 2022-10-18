#pragma once

#if USING_XIGNCODE

//! 2011.2.8 / i4u4me / 
//! _WIN32_WINNT 0x500 ������ StringCbCopyW, StringCbCatW �Լ��� �������� �ʴ���.
//! ����� ����� ��û���� _STRSAFE_H_INCLUDED_ undef �Ѵ�.

#undef _STRSAFE_H_INCLUDED_

//#include "../../../include/XignCode/IncludeListForXignCode.h"
#define XIGNCODE_INCLUDE_PATH( v )	 <../../../include/XignCode/##v/zwave_sdk_client.h>
#include XIGNCODE_INCLUDE_PATH( XIGNCODE_VERSION )

//------------------------------------------------------------------------------ 
namespace XignCodeApi
{
    struct XignCode_UserInfo
    {
        const static int kStringLength = 80;

        xbool is_ready;
        xwch user_id[kStringLength];
        xwch character_name[kStringLength];
        xwch server_name[kStringLength];

        xint map_id;
        xint field_id;
        xwch field_name[kStringLength];

        XignCode_UserInfo()
        {
            memset(this, 0, sizeof(*this));
        }
    };

    // user context
    XignCode_UserInfo* xigncode_user_info();

    //! ��Ƽ ����Ʈ�� �޾Ƽ� �����ڵ�� ������
    bool SetInfo(xcstr _server_name, xcstr _user_id, xcstr _character_name, xint _map_id, xint _field_id, xcstr _field_name);

    //! Init XIGNCODE LIB warpper function
    bool Init();
    void Release();

    //! ���� ���� ���� ȣ��
    bool Connect();

    //! ���� ���� ���� ����
    void Disconnect();

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : �ش� �Լ������� �޼����� ó���ϰ� ���� ���μ������� ��� ����
    //!                     false : �޼��� �ѱ�
    //! Qualifier: xigncode ��Ŷ�� �Դ��� ó�� ����
    //! Parameter: MSG_BASE * pk : ��Ŷ ���̽�
    //------------------------------------------------------------------------------
    bool RecvPacket(MSG_BASE* pk);

    //! XIGNCODE �ڵ� ��Ŷ ���� �� ��� ���� �ݹ� �Լ�
    void CALLBACK OnProbeComplete(HANDLE CodeBox
								     ,CPACKETDATA Packet
								     ,PACKETDATA Buffer
								     ,SIZE_T BufferSize
								     ,PVOID Context);

    //! �����ڵ� �ݹ� ���
    xstatus XCALL MyGameGetUserInfoCallback(xuint info_id, xwstr buffer, xsize buffer_size, xpvoid context);
};

#endif //USING_XIGNCODE