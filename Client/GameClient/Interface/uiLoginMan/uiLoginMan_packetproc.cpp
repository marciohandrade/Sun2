//------------------------------------------------------------------------------
//  uiLoginMan_packetproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginMan.h"
#include "PacketStruct_CL.h"
#include "SceneBase.h"
#include "WrapArmorKit.h"

#include "AutoTestManager.h"
#if USING_GAMEGUARD
#include "GameGuard/GameGuardApi.h"
#include "GlobalFunc.h"
#endif // USING_GAMEGUARD

void uiLoginMan::NetworkProc( MSG_BASE * pMsg )
{
	if(GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_LOGIN)
		return;

	switch( pMsg->m_byCategory )
	{
	case CATEGORY_AUTH:
		{
			switch( pMsg->m_byProtocol)
			{
			// ����
			case FC_AUTH_READY_CMD:
				{
					// READY ����
					this->RECV_AUTH_READY_CMD(pMsg);

					// ���� Ȯ�� ����
					this->SEND_AUTH_VERIFY_SYN();
				}
				break;

			case FC_AUTH_VERIFY_ACK:
				{
					// ����Ȯ�μ���
					this->RECV_AUTH_VERIFY_ACK(pMsg);


#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
                    if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
                    {
                        AutoTestManagerEx::Instance()->RunLogin();
                    }
#endif
				}
				break;

#ifdef __CN_269_MATRIX_CARD_AUTH
                // �߱� ����ī��
			case FC_AUTH_MATRIX_CARD_SYN:
				{
					// Matrix Card ���� ����
					RECV_AUTH_MATRIX_CARD_SYN(pMsg);
				}
				break;
#endif

#if defined(__CN_20100909_SECURITY_TOKEN__)
            case AU_AUTH_SECURITY_TOKEN_ACK:
                {
                    // �߱� ��ū
                    RECV_AUTH_SECURITY_TOKEN_ACK(pMsg);
                }
                break;

#elif defined(_JP000000_ONETIMEPASSWORD_AUTHFLOW_)
                // �Ϻ� OneTimePassword
            case CF_AUTH_OTP_PASS_CMD:
                {
                    // �Ϻ� OTP CMD ��Ŷ ����
                    RECV_AUTH_OTP_PASS_CMD(pMsg);
                }
                break;
            case AU_AUTH_OTP_PASS_ACK:
                {
                    // �Ϻ� OTP CMD ��Ŷ ����
                    RECV_AUTH_OTP_PASS_ACK(pMsg);
                }
                break;
#endif //_JP000000_ONETIMEPASSWORD_AUTHFLOW_ __CN_20100909_SECURITY_TOKEN__

			case FC_AUTH_ID_PASSWORD_CHECK_ACK:
			//case FC_AUTH_TOKEN_PASS_ACK:
			//case FC_AUTH_REAUTH_PASS_ACK:			//<- �� 3���� ���������� ���� ��
				{
					// ���� ���� ��� ����
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
					if (m_bServerSelect)
					{
						RECV_AUTH_REAUTH_PASS_ACK(pMsg);
					}
					else
#endif //__NA001334_090420_SERVER_SELECT_FACILITY__
					{
#if defined(_APPLY_KOREA_CHANNELING)
						RECV_AUTH_TOKENPASS_ACK(pMsg);
#elif defined(_NHN_USA_CHANNELING)
						RECV_AUTH_TOKEN_NORTHA_PASS_ACK(pMsg);
#elif defined(_APPLY_JAPAN_GAMECHU_CHANNELING)
                        RECV_AUTH_TOKEN_JP_GAMECHU_ACK(pMsg);
#elif defined(_APPLY_RUSSIA_INGAMBA_CHANNELING)
                        RECV_AUTH_PASS_RU_INGAMBA_ACK(pMsg);
#else
						RECV_AUTH_ID_PASSWORD_CHECK_ACK(pMsg);
#endif
					}					
#if USING_GAMEGUARD
                    // ���� ���忡 ID ����
                    // 0 - ����, 1 - ����
                    if (GameGuardApi::Instance()->SendUserId(m_strTryID) == FALSE)
                    {
#ifdef _NA_000000_20140519_GAMEGUARD_LOG
                        TCHAR result_log[INTERFACE_STRING_LENGTH] = {0, };
                        Snprintf(result_log, INTERFACE_STRING_LENGTH, _T("GameGuardApi::SendUserId() ���� ���� : %s\n"), m_strTryID);
                        GlobalFunc::Write_GameGuard_Log(result_log);
#endif // _NA_000000_20140519_GAMEGUARD_LOG

                    }
#endif // USING_GAMEGUARD
				}
				break;
			case FC_AUTH_SERVER_LIST_WORLD_ACK:
				{
					// ����(���� �׷�) �̸� ��� ����
					this->RECV_AUTH_SERVER_LIST_WORLD_ACK(pMsg);
				}
				break;

			case FC_AUTH_SERVER_LIST_CHANNEL_ACK:
				{
					// ä��(����) ��� �� ���� ����
					this->RECV_AUTH_SERVER_LIST_CHANNEL_ACK(pMsg);
#if defined(_DEBUG) || defined(_GAMECLIENT_VER)
                    if( AutoTestManagerEx::Instance()->IsAutoLogin() == true )
                    {
                        AutoTestManagerEx::Instance()->RunSelectServer();
                    }
#endif
				}
				break;

			case FC_AUTH_SERVER_SELECT_ACK:
				{
					// ���� ���� ��� ����
					this->RECV_AUTH_SERVER_SELECT_ACK(pMsg);
				}
				break;
			}
		}
		break;
	}
}
//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
