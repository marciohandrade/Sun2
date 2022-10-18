#include "stdafx.h"
#include "SteelStatus.h"
#include "StatusManager.h"


VOID SteelStatus::Start()
{
	EtcStatus::Start();
}

VOID SteelStatus::End()
{
	EtcStatus::End();
    SendStatusDelBRD();
}