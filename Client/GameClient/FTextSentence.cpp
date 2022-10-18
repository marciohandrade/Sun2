#include "SunClientPrecompiledHeader.h"
#include "ftextsentence.h"
#include "FTextParam.h"
#include "GlobalFunc.h"

FTextSentence::FTextSentence(void)
{
}

FTextSentence::FTextSentence( FTextSentence & rSentence )
{
	int iSize = rSentence.Size();
	for ( int i = 0 ; i < iSize; ++i )
	{					
		const FTextParam & rParam = rSentence.GetParam(i);			 
		this->Pushback( rParam );
	}
}

FTextSentence::~FTextSentence(void)
{
	Release();
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
FTextSentence::Release()
{
	int iSize = VecParam.size();
	if ( 0 == iSize )
	{
		return;
	}

	for (int i = 0; i < iSize; ++i)
	{
		SAFE_DELETE( (FTextParam *)VecParam[i] );
	}

	VecParam.clear();
}

//-------------------------------------------------------------------------------------------
/**
*/
void 
FTextSentence::Pushback( const FTextParam & rParam )
{
	FTextParam * pParam = new FTextParam(rParam);
	
	VecParam.push_back( pParam );
}

//-------------------------------------------------------------------------------------------
/**
*/
int	 
FTextSentence::Size() const
{
	return this->VecParam.size();
}

//-------------------------------------------------------------------------------------------
/**
*/
const FTextParam & 
FTextSentence::GetParam(int index) const
{
	assert( index >= 0 && index < (int)VecParam.size() );
	return *VecParam[index];
}

//-------------------------------------------------------------------------------------------
/**
*/
FTextSentence & 
FTextSentence::operator =( FTextSentence & rSentence )
{
	Release();

	int iSize = rSentence.Size();
	for ( int i = 0 ; i < iSize; ++i )
	{					
		const FTextParam & rParam = rSentence.GetParam(i);			 
		this->Pushback( rParam );
	}

	return *this;
}
