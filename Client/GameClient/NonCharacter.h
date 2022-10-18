

#ifndef		_NONCHARACTER_H
#define		_NONCHARACTER_H

#include "Object.h"
#include "SCNonCharacter.h"


class NonCharacter : public Object, public SCNonCharacter
{

public:
	NonCharacter( void );
	~NonCharacter( void );

	virtual BOOL Create( DWORD dwObjectKey, DWORD dwID = 0 ) { return TRUE; }
	virtual void Destroy( void ) {}
};





#endif	//	_NONCHARACTER_H


