#pragma once

#include <SCObject.h>


class ObjectBase
{
protected:
	ObjectBase();

protected:
	typedef std::string tstring;

	SCObject m_scObjAtt;				// 오브젝트의 키와, 종류 (서버 클라이언트 공용)
	tstring m_Name;

public:

	virtual void SetName(const tstring& value) { m_Name = value; }
	LPCTSTR GetName() const { return m_Name.c_str(); }
	//const tstring& GetName() const { return m_Name; }

	const SCObject& GetSCObject() const { return m_scObjAtt; }

	eOBJECT_TYPE GetObjectType() { return m_scObjAtt.GetObjectType(); }
	void SetObjectType( eOBJECT_TYPE Type ) { return m_scObjAtt.SetObjectType( Type ); }
	DWORD GetObjectKey() { return m_scObjAtt.GetObjectKey(); }
	void SetObjectKey( DWORD Key ) { m_scObjAtt.SetObjectKey(Key); }


	BOOL IsKindOfObject( eOBJECT_TYPE eObjectType );
};