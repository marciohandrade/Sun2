#include "SunClientPrecompiledHeader.h"
#include "ObjectBase.h"


ObjectBase::ObjectBase()
{

}

//------------------------------------------------------------------------------
/**
*/
BOOL ObjectBase::IsKindOfObject( eOBJECT_TYPE eObjectType )
{
	return BOOL((GetSCObject().GetObjectType() & eObjectType) == eObjectType);
}


