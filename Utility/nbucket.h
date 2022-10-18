#ifndef N_BUCKET_H
#define N_BUCKET_H
//------------------------------------------------------------------------------
/**
    @brief A bucket contains a fixed-size array of nArray objects,
    each initialized with a size of 0, and a grow size. Handy for bucket sorts. 

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/

#include "Narray.h"

//------------------------------------------------------------------------------
template<class TYPE, uint NUMBUCKETS> class nBucket
{
public:
    /// constructor
    nBucket(int initialSize, int growSize);
    /// destructor
    ~nBucket();
    /// access to bucket array
    nArray<TYPE>& operator[](uint bucketIndex);
    /// clear all arrays
    void Clear();
    /// reset all contained arrays
    void Reset();
    /// get number of bucket arrays
    int Size() const;

private:
    /// default constructor is private
    nBucket();
    /// assignment operator is private (FIXME)
    nBucket& operator=(const nBucket& rhs);

    nArray<TYPE> arrays[NUMBUCKETS];
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE, uint NUMBUCKETS>
nBucket<TYPE, NUMBUCKETS>::nBucket(int initialSize, int growSize)
{
    uint i;
    for (i = 0; i < NUMBUCKETS; i++)
    {
        this->arrays[i].Reallocate(initialSize, growSize);
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE, uint NUMBUCKETS>
nBucket<TYPE, NUMBUCKETS>::~nBucket()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    The default constructor is illegal.
*/
template<class TYPE, uint NUMBUCKETS>
nBucket<TYPE, NUMBUCKETS>::nBucket()
{
}

//------------------------------------------------------------------------------
/**
    The assignment operator is illegal (FIXME).
*/
template<class TYPE, uint NUMBUCKETS>
nBucket<TYPE, NUMBUCKETS>&
nBucket<TYPE, NUMBUCKETS>::operator=(const nBucket<TYPE, NUMBUCKETS>& rhs)
{
    
    return *this;
}

//------------------------------------------------------------------------------
/**
    Access to embedded arrays.
*/
template<class TYPE, uint NUMBUCKETS>
nArray<TYPE>&
nBucket<TYPE, NUMBUCKETS>::operator[](uint bucketIndex)
{
    return this->arrays[bucketIndex];
}

//------------------------------------------------------------------------------
/**
    Clear all contained arrays (does apply element destructor).
*/
template<class TYPE, uint NUMBUCKETS>
void
nBucket<TYPE, NUMBUCKETS>::Clear()
{
    uint i;
    for (i = 0; i < NUMBUCKETS; i++)
    {
        this->arrays[i].Clear();
    }
}

//------------------------------------------------------------------------------
/**
    Reset all contained arrays (does not apply element destructor).
*/
template<class TYPE, uint NUMBUCKETS>
void
nBucket<TYPE, NUMBUCKETS>::Reset()
{
    uint i;
    for (i = 0; i < NUMBUCKETS; i++)
    {
        this->arrays[i].Reset();
    }
}

//------------------------------------------------------------------------------
/**
    Returns number of bucket arrays.
*/
template<class TYPE, uint NUMBUCKETS>
int
nBucket<TYPE, NUMBUCKETS>::Size() const
{
    return NUMBUCKETS;
}
 
//------------------------------------------------------------------------------
#endif

