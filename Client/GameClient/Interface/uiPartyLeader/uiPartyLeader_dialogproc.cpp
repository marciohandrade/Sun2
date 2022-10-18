//------------------------------------------------------------------------------
//  uiPartyLeader_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiPartyLeader.h"
#include "Player.h"
#include "ObjectManager.h"
#include "TargetDialog.h"
#include "Monster.h"
#include "StateInfoParser.h"
#include "Mouse.h"
#include "IconImage.h"

//------------------------------------------------------------------------------
WzID uiPartyLeader::m_wzId[DIALOG_MAX] = 
{ 
        StrToWzID("P000"),    // DLGPOS_HPBAR,
        StrToWzID("B000"),    // DLGPOS_SLIDE,
        StrToWzID("B777"),    // DLGPOS_OPTION,
        StrToWzID("P001"),    // DLGPOS_BUFF01,
        StrToWzID("P002"),    // DLGPOS_BUFF02,
        StrToWzID("P003"),    // DLGPOS_BUFF03,
        StrToWzID("P004"),    // DLGPOS_BUFF04,
        StrToWzID("P005"),    // DLGPOS_BUFF05,
        StrToWzID("P006"),    // DLGPOS_BUFF06,
        StrToWzID("P007"),    // DLGPOS_BUFF07,
        StrToWzID("P008"),    // DLGPOS_BUFF08,
        StrToWzID("P009"),    // DLGPOS_BUFF09,
        StrToWzID("P010"),    // DLGPOS_BUFF10,
        StrToWzID("PI01"),    // DLGPOS_FACE00, // ¹ö¼­Ä¿
        StrToWzID("PI02"),    // DLGPOS_FACE01, // µå¶ó°ï ³ªÀÌÆ®
        StrToWzID("PI03"),    // DLGPOS_FACE02, // ¼¨µµ¿ì
        StrToWzID("PI04"),    // DLGPOS_FACE03, // ¹ßÅ°¸®
        StrToWzID("PI05"),    // DLGPOS_FACE04, // ¿¤¸®¸àÅ»
        StrToWzID("T000"),    // DLGPOS_TXT,
        StrToWzID("PI10"),    // DLGPOS_MINE,
        StrToWzID("B222"),    // DLGPOS_TARGETSHARE,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiPartyLeader::showWindow( BOOL val )
{
    this->m_bShow = val;    // for uiBase::IsVisable()

    if (val)
    {
        MoveWindowWZ( 10.0f, 117.0f );

        this->OffLock();
        this->OffPending();

        this->m_fGaugeSize = (float)this->m_rcInit.right;

        this->m_idxCurrentFreeTargetAssistMember = 0;
        this->m_bAssistPartyLeader = false;
        this->m_dwPartyLeaderTargetKey = 0;

        this->m_bShowBuffInfos = true;

        CCtrlStaticWZ* pControl =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MINE));
        if (pControl)
        {
            pControl->ShowWindowWZ(WZ_HIDE);
        }
    
        pControl =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TARGETSHARE));
        if (pControl)
        {
            pControl->ShowWindowWZ(WZ_HIDE);
        }

        this->m_bAssistHelpOnce = false;
    }
    else
    {
        this->m_bShowBuffInfos = false;
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            POSTYPE pos = getCtrlIDToPos( pMessage->dwCtrlID );

            if( pos <= DIALOG_MAX ) 
            {
                if (pos != DLGPOS_OPTION && pos != DLGPOS_SLIDE && pos != DLGPOS_TARGETSHARE)
                {
                    this->SetTargeting();
                }
            }

            switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
            {
            case DLGPOS_OPTION:
                {
                    uiBase* partyOption =
                        this->GetUIMan()->GetUserInterface(UI_PARTY_OPTION);

                    if (partyOption)
                        partyOption->ShowInterface(!partyOption->IsVisiable());
                }
                break;

            case DLGPOS_SLIDE:
                {
                    this->m_bShowBuffInfos = !this->m_bShowBuffInfos;
                }
                break;

            case DLGPOS_TARGETSHARE:
                {
                    if (this->Is_AssistPartyLeader())
                    {
                        this->Assist_SetTargetToLeader();
                    }
                }
                break;
            }
        }
        break;


	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::update()
{
    if (this->Is_AssistPartyLeader())
    {
        Monster *pMonster = (Monster *)g_ObjectManager.GetObject(this->Get_PartyLeaderTargetKey());
        if (pMonster)
        {
            if (pMonster->GetHP() <= 0)
            {
                this->Set_AssistPartyLeader(false);
                CCtrlStaticWZ* pControl =
                    static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TARGETSHARE));
                if (pControl)
                {
                    pControl->ShowWindowWZ(WZ_HIDE);
                }
            }
            else
            {
                CCtrlStaticWZ* pControl =
                    static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_TARGETSHARE));
                if (pControl)
                {
                    pControl->ShowWindowWZ(WZ_SHOW);
                }
            }
        }
    }

    //if (this->Is_AssistPartyLeader())
    //{
    //    TargetDialog * pTargetDlg =
    //        static_cast<TargetDialog *>(this->GetUIMan()->GetDialog(InterfaceManager::DIALOG_TARGET));

    //    if (pTargetDlg)
    //    {
    //        if (pTargetDlg->GetTargetKey() != this->Get_PartyLeaderTargetKey())
    //        {
    //            this->Set_AssistPartyLeader(false);
    //        }
    //    }
    //    else
    //    {
    //        this->Set_AssistPartyLeader(false);
    //    }
    //}

    if (this->IsLeader())
    {
        if (this->GetCtrlBtnOption()->GetState() == WZ_HIDE)
            this->GetCtrlBtnOption()->ShowWindowWZ(WZ_SHOW);
    }
    else
    {
        if (this->GetCtrlBtnOption()->GetState() == WZ_SHOW)
            this->GetCtrlBtnOption()->ShowWindowWZ(WZ_HIDE);
    }

    this->updateInfos();
    this->updateHP();
    this->updateBuff();

    this->SetObjectInfo();
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::updateInfos()
{
    //POSTYPE pos;
    //CCtrlStaticWZ * pControl;

    //pos = getCtrlIDToPos( m_wzId[DLGPOS_TXT] );
    //pControl = (CCtrlStaticWZ*)getControl(pos);

    //if (pControl)
    //{
    //    pControl->SetTextColor(RGBA(255,255,255,255));
    //    pControl->SetFontID(StrToWzID("st10"));

    //    pControl->SetCaption("??"); 
    //}

    if(!this->m_memInfo.IsFill())
        return;

    if (g_pHero && g_pHero->GetObjectKey() == this->m_memInfo.GetInfo().m_dwObjKey)
    {
        CCtrlStaticWZ* pControlMine =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_MINE));
        if (pControlMine)
        {
            pControlMine->ShowWindowWZ(WZ_SHOW);
        }
    }

    for (int i=0; i<6; ++i)
    {
        CCtrlStaticWZ* pControlFaces =
            static_cast<CCtrlStaticWZ *>(this->getControl(DLGPOS_FACE00 + i));
        if (pControlFaces)
        {
            pControlFaces->ShowWindowWZ(WZ_HIDE);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::updateHP()
{
    /* nothing */
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::updateBuff()
{
    /* TODO */
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::render()
{
	char	szMBString[INTERFACE_STRING_LENGTH];

    if (this->m_bShowBuffInfos)
    {
        RECT rc;		

        int iMouseX = Mouse::Instance()->GetMouseX();
        int iMouseY = Mouse::Instance()->GetMouseY();

        for (int a = 0; a < MAX_PARTYDLG_SKILL_EFFECT; a++) 
        {
            int iControlIndex = DLGPOS_BUFF01 + a;

            if (getControl(iControlIndex))
            {
                rc = getControl(iControlIndex)->GetSizeRect();
            }
            else
            {
                continue;
            }

            if (m_arEffectImage[a].pImage)
            {

				
				m_arEffectImage[a].RenderOwnerStatusIcon(&rc);


                if (rc.left <= iMouseX && rc.right >= iMouseX)
                {
                    if (rc.top <= iMouseY && rc.bottom >= iMouseY)
                    {

                        g_InterfaceManager.RegisterSkillEffectInfoTooltip(&m_arEffectImage[a]);					
                    }
                }
            }
        }
    }

    TCHAR buf[INTERFACE_STRING_LENGTH];

    Sprintf (buf, _T("%d  %s"), this->m_memInfo.GetInfo().m_wLevel, this->m_memInfo.GetInfo().m_szCharName);

    if(!this->m_memInfo.IsFill())
    {
        this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("st10"));

        RECT rc, rc2;

        POSTYPE pos;
        CCtrlStaticWZ * pControl;

        pos = getCtrlIDToPos( m_wzId[DLGPOS_TXT] );
        pControl = (CCtrlStaticWZ*)getControl(pos);

        if (pControl)
        {
            rc2 = pControl->GetSizeRect();

            SetRect(&rc,
                rc2.left + 5,
                rc2.top + 2,
                rc2.right - 10,
                rc2.top + 2 + 14);

            WzColor _color;

            //if (pPlayer->IsDead())
            //    _color = COLORDEF_UIPARTY_LEADER_AT_TIME_DEAD;
            //else
                _color = COLORDEF_UIPARTY_LEADER_AT_TIME_LIVE;

				this->GetDrawBase()->x_pManagerTextOut->DrawText(buf,
                &rc,
                _color,
                WzColor_RGBA(0,0,0,0),
                TP_HLEFT);
        }
        return;
    }

    WORD _level;
    TCHAR _name[64];

    if (this->m_memInfo.IsFill())
    {
        Player* pPlayer =
            static_cast<Player *>(g_ObjectManager.GetObject(this->m_memInfo.GetInfo().m_dwObjKey));

        if (pPlayer)
        {
            StrCopy (_name, this->m_memInfo.GetInfo().m_szCharName);
            _level = pPlayer->GetLevel();
            Sprintf (buf, _T("%d  %s"), _level, _name);
        }
        else
        {
            StrCopy (_name, this->m_memInfo.GetInfo().m_szCharName);
            _level = this->m_memInfo.GetInfo().m_wLevel;
            Sprintf (buf, _T("%d  %s"), _level, _name);
        }
    }

    this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("st10"));

    RECT rc, rc2;

    POSTYPE pos;
    CCtrlStaticWZ * pControl;

    pos = getCtrlIDToPos( m_wzId[DLGPOS_TXT] );
    pControl = (CCtrlStaticWZ*)getControl(pos);

    if (pControl)
    {
        rc2 = pControl->GetSizeRect();

        SetRect(&rc,
            rc2.left + 5,
            rc2.top + 2,
            rc2.right - 10,
            rc2.top + 2 + 14);

        WzColor _color;

        if (this->m_memInfo.IsFill())
        {
            Player *pPlayer = (Player *)g_ObjectManager.GetObject(this->m_memInfo.GetInfo().m_dwObjKey);
            if (pPlayer && pPlayer->IsDead())
                _color = COLORDEF_UIPARTY_LEADER_AT_TIME_DEAD;
            else
                _color = COLORDEF_UIPARTY_LEADER_AT_TIME_LIVE;
        }
        else
        {
            _color = COLORDEF_UIPARTY_LEADER_AT_TIME_LIVE;
        }

		this->GetDrawBase()->x_pManagerTextOut->DrawText(buf,
            &rc,
            _color,
            WzColor_RGBA(0,0,0,0),
            TP_HLEFT);
    }

	pos = getCtrlIDToPos( m_wzId[DLGPOS_FACE00] );
    pControl = (CCtrlStaticWZ*)getControl(pos);

	if (pControl)
	{
		RECT rc = pControl->GetSizeRect();

		HANDLE hTexture = g_InterfaceManager.GetPartyHeadTexture(0);

		if (hTexture != INVALID_HANDLE_VALUE)
		{

			g_pSunRenderer->SetAlphaBlendMode( AB_ALPHA);

			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);
			g_pSunRenderer->x_pManagerTexture->SetColorArg1(0,CManagerTexture::ETARG_TEXTURE);				
			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(1,CManagerTexture::ETOP_SELECTARG1);
			g_pSunRenderer->x_pManagerTexture->SetColorArg1(1,CManagerTexture::ETARG_CURRENT);				

			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(0,CManagerTexture::ETOP_SELECTARG1);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(0,CManagerTexture::ETARG_TEXTURE);
			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(1,CManagerTexture::ETOP_MODULATE);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(1,CManagerTexture::ETARG_CURRENT);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg2(1,CManagerTexture::ETARG_TEXTURE);


			
			g_pSunRenderer->RenderTextureBlending(hTexture,hTexture,rc.left,rc.top,64,64);


			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(0);
			g_pSunRenderer->x_pManagerTexture->SetColorBlendingOp(1);
			g_pSunRenderer->x_pManagerTexture->SetColorArg1(0);
			g_pSunRenderer->x_pManagerTexture->SetColorArg1(1);				

			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(0);
			g_pSunRenderer->x_pManagerTexture->SetAlphaBlendingOp(1);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(0);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg1(1);
			g_pSunRenderer->x_pManagerTexture->SetAlphaArg2(1);


		}

	}

}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::SetObjectInfo()
{
    if (!this->m_memInfo.IsFill())
        return;

    Object * pObject = g_ObjectManager.GetObject(this->m_memInfo.GetInfo().m_dwObjKey);
    if (pObject)
    {
        if (pObject->IsKindOfObject(PLAYER_OBJECT))
        {
            Player * pPlayer = static_cast<Player *>(pObject);

            DWORD maxhp = pPlayer->GetCharInfo()->m_dwMaxHP;
            DWORD curhp = pPlayer->GetHP();

            ASSERT(maxhp > 0);
            if (curhp > maxhp)
                curhp = maxhp;

            SetHP( ( (float)curhp / (float)maxhp ) * 100.0f);

            SetSkillEffectInfo(pPlayer);

            //this->m_memInfo->m_wLevel = pPlayer->GetLevel();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::SetHP( float fHPPercent )
{
    ASSERT( fHPPercent <= 100 );

    POSTYPE pos;
    CCtrlStaticWZ * pControl;
    pos = getCtrlIDToPos( m_wzId[DLGPOS_HPBAR] );
    pControl = (CCtrlStaticWZ*)getControl(pos);

    if (pControl)
    {
        RECT rect;
        rect = pControl->GetSizeRect();
        float width;
        float height = 0;

        RECT rcGauge = this->m_rcInit;

        height = (float)rcGauge.bottom;
        rcGauge = pControl->GetSizeRect();

        if (fHPPercent >= 0.0f )
        {
            width = (m_fGaugeSize/100.f) * fHPPercent;
            pControl->MoveWindowWZ( (float)rcGauge.left, (float)rcGauge.top, width, height );
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::GetSkillEffectArea(int iIndex,RECT *pRect)
{
    if (iIndex < 0)
    {
        iIndex = 0;
    }

    if (iIndex >= MAX_SKILLEFFECT_AREA)
    {
        iIndex = MAX_SKILLEFFECT_AREA - 1;
    }

    int iControlIndex = DLGPOS_BUFF01 + iIndex;

    *pRect = this->getControl(iControlIndex)->GetSizeRect();
}

//------------------------------------------------------------------------------
/**
*/
static DWORD GetIconIDForStatus(DWORD dwStatus)
{


    BASE_STATEINFO *pInfo = StateInfoParser::Instance()->GetStateInfo(dwStatus);
    if (pInfo)
    {
        return pInfo->m_dwIconCode;
    }
    else
    {
        return 0xFFFFFFFF;
    }


}

//------------------------------------------------------------------------------
/**
*/
void uiPartyLeader::SetSkillEffectInfo(Character *pChr)
{
    SKILL_EFFECT *pEffect = pChr->GetFirstSkillEffect();

    int index = 0;

    while (pEffect && index < MAX_PARTYDLG_SKILL_EFFECT)
    {
        // Ç¥½ÃÇØ¾ßÇÒ ÀÌÆåÆ®°¡ ´Ù¸£´Ù¸é!
        if (m_arEffectImage[index].iEffectID != pEffect->iEffectID)
        {

            if (m_arEffectImage[index].pImage)
            {
                ICONMGR()->RemoveIcon(m_arEffectImage[index].pImage);
            }


            SKILL_EFFECT_IMAGE imagedata;

            m_arEffectImage[index].iEffectID = pEffect->iEffectID;
            m_arEffectImage[index].dwOwnerID = pChr->GetObjectKey();
            m_arEffectImage[index].dwStatusID = pEffect->dwStatusID;
            DWORD dwIconID = GetIconIDForStatus(pEffect->dwStatusID);
            m_arEffectImage[index].pImage = ICONMGR()->Add(dwIconID);
            m_arEffectImage[index].dwIconID = dwIconID;



        }

        pEffect = pChr->GetNextSkillEffect();
        index++;
    }

    // ³²Àº ºóÄ­Àº ÇØÁ¦~
    for (index;index < MAX_PARTYDLG_SKILL_EFFECT; index++)
    {
        if (m_arEffectImage[index].pImage)
        {
            ICONMGR()->RemoveIcon(m_arEffectImage[index].pImage);
        }

        ZeroMemory(&m_arEffectImage[index],sizeof(SKILL_EFFECT_IMAGE));
    }

}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyLeader::ClearSkillEffectInfo()
{
    for (int a = 0; a < MAX_PARTYDLG_SKILL_EFFECT; a++)
    {
        if (m_arEffectImage[a].pImage)
        {
            ICONMGR()->RemoveIcon(m_arEffectImage[a].pImage);
        }
    }

    ZeroMemory(m_arEffectImage,sizeof(m_arEffectImage));
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
