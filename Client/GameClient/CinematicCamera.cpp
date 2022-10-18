#include "SunClientPrecompiledHeader.h"
#include "cinematiccamera.h"
#include "character.h"
#include "object.h"
#include "appearancemanager.h"

CinematicCamera::CinematicCamera(void)
{
	SetObjectType(CAMERA_OBJECT);
}

CinematicCamera::~CinematicCamera(void)
{
}


BOOL CinematicCamera::Create( DWORD dwObjectKey, DWORD dwID )
{
	if (!Object::Create(dwObjectKey, dwID))
	{
		return FALSE;
	}

	

	APPEARANCEINFO info;
	info.ObjectType = APPEARANCEINFO::CAMERA;

	InitRender(1,0,&info,0,FALSE);


	return TRUE;
}