//	GenerateRandomBlock.h

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽

//////////////////////////////////////////////////////////////////////////
// KEY BLOCK ENC/DEC CHANGE TABLE


template<DWORD _SIZE_>
struct sRANDOM_INDEX_ARRAYs
{
	BYTE	STREAM[_SIZE_];
};

#define RND_TYPES	typename BYTE

//////////////////////////////////////////////////////////////////////////
//

#if CLIENT_CODE

template<
#include "Loop256_Typenames.h"
>
class RandomClassDef
{
public:
private:
	DWORD dwIDX;
	static const sRANDOM_INDEX_ARRAYs<256>		Rnd256Nos;

public:
	RandomClassDef() {}
	~RandomClassDef() {}
	sRANDOM_INDEX_ARRAYs<256>*	GetRandomValues() { return (sRANDOM_INDEX_ARRAYs<256>*)&Rnd256Nos; }
};

//////////////////////////////////////////////////////////////////////////

template<
#include "Loop256_Typenames.h"
>
#undef RND_TYPES
#define RND_TYPES
const sRANDOM_INDEX_ARRAYs<256> RandomClassDef
<
#include "Loop256_Typenames.h"
>
::Rnd256Nos =
{
#include "Loop256_Typenames.h"
};
//

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#else //SERVER_CODE

//
template<
#include "Loop256_Typenames.h"
>
class RandomClassQuery	//in Server
{
public:
private:
	DWORD dwIDX;
	static const sRANDOM_INDEX_ARRAYs<256>		Rnd512Nos[2];

public:
	RandomClassQuery() {}
	~RandomClassQuery() {}
	const sRANDOM_INDEX_ARRAYs<256>*	GetRandomValues() { return (const sRANDOM_INDEX_ARRAYs<256>*)&Rnd512Nos[0]; }
	const sRANDOM_INDEX_ARRAYs<256>*	GetServiceValue() { return (const sRANDOM_INDEX_ARRAYs<256>*)&Rnd512Nos[1]; }
};
template<
#include "Loop256_Typenames.h"
>
#undef RND_TYPES
#define RND_TYPES
const sRANDOM_INDEX_ARRAYs<256> RandomClassQuery
<
#include "Loop256_Typenames.h"
>
::Rnd512Nos[2] =
{
#include "Loop256_Typenames.h"
};

#endif //

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽
//	
//
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收朽

