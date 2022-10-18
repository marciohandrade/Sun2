#pragma once
#ifndef PROGRAMCOMMON_SERIALIZE_STREAM_H
#define PROGRAMCOMMON_SERIALIZE_STREAM_H

//==================================================================================================
/// SerializeStream class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 2. 13
    @purpose
    - SCStream를 상속받아 부모의 인터페이스를 유지하면서 여러 스트림 데이터를 처리하기위해서 template class로 제작
    @note
    - 
    @history 
    - 10/08/04|waverix|change to no template to support easy debugging
                       the use case of a SCStream has a SerializeStream only based on a case study
                       (f100804.5L)
*/
//==================================================================================================

#include "SCStream.h"

template<class STREAM_TYPE_, int STREAM_NUM_>
class SerializeStream : public SCStream
{
    //BOOST_STATIC_ASSERT((boost::is_same<STREAM_TYPE_, ITEMSTREAMEX>::value));
public:
    typedef STREAM_TYPE_ ReferenceArray[STREAM_NUM_];
    SerializeStream()
        : SCStream(STREAM_NUM_, reinterpret_cast<STREAM_TYPE_*>(0))
        , kReferenceArray(reinterpret_cast<ReferenceArray&>(*stream_))
    {
    }
    virtual ~SerializeStream()
    {
    }
    //
    //                  0 <= position < number_of_items
    virtual bool IsEmpty(int position) const;
    //
    const ReferenceArray& kReferenceArray; 
};

//==================================================================================================

template<class STREAM_TYPE_, int STREAM_NUM_>
bool SerializeStream<STREAM_TYPE_, STREAM_NUM_>::IsEmpty(int position) const
{
    if (&kReferenceArray != reinterpret_cast<ReferenceArray*>(stream_)) {
        __debugbreak(); // invalid reference access
        return true; // problem point, a custom logic returns true.
    };
    if (position >= 0 && position < number_of_items_) {
        return kReferenceArray[position].Part.IsValid() == false;
    }
    assert(!"Index out of range");
    return true;
}

/*
// (f100804.5L) removed
template< class STREAM_TYPE, BYTE STREAM_NUM >
class SerializeStream : public SCStream
{
public:
	SerializeStream(){}
	~SerializeStream(void){}

	virtual  BOOL IsEmpty( POSTYPE AtPos ) { if( AtPos>STREAM_NUM ) return TRUE; return ( FALSE == m_pStream[AtPos].Part.IsValid() ); }
	virtual  VOID Clear() { memset( m_pStream, 0, sizeof(STREAM_TYPE)*STREAM_NUM ); }

	virtual void Serialize( POSTYPE pos, BYTE * pStream, eSERIALIZE eType )
	{
		if( eType == SERIALIZE_STORE )
		{
			ASSERT( !IsBadReadPtr( pStream, sizeof(STREAM_TYPE) ) );
			memcpy( &m_pStream[pos], pStream, sizeof(STREAM_TYPE) );
		}
		else if( eType == SERIALIZE_LOAD )
		{
			ASSERT( !IsBadWritePtr( pStream, sizeof(STREAM_TYPE) ) );
			memcpy( pStream, &m_pStream[pos], sizeof(STREAM_TYPE) );
		}
	}
	virtual  VOID Serialize( BYTE * pStream, WORD wSize, eSERIALIZE eType )
	{
		ASSERT( wSize == sizeof(STREAM_TYPE)*STREAM_NUM );
		if( eType == SERIALIZE_STORE )
			memcpy( m_pStream, pStream, wSize );
		else if( eType == SERIALIZE_LOAD )
			memcpy( pStream, m_pStream, wSize );
	}
	virtual BYTE GetMaxSlotNum() { return STREAM_NUM; }
	virtual BYTE * GetStreamPtr() { return (BYTE *)m_pStream; }
	virtual BYTE * GetStreamPtr(BYTE pos) { return (BYTE *)&m_pStream[pos]; }
private:
	STREAM_TYPE m_pStream[STREAM_NUM];
};

*/

#endif //PROGRAMCOMMON_SERIALIZE_STREAM_H
