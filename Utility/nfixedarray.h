#ifndef N_FIXEDARRAY_H
#define N_FIXEDARRAY_H
//------------------------------------------------------------------------------
/**
    @briefA  fixed size, bounds checked array.

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/
#include "../ProgramCommon/Define.h"

//------------------------------------------------------------------------------
template<class TYPE> class nFixedArray
{
public:
    /// default constructor
    nFixedArray();
    /// constructor with given size
    nFixedArray(int s);
    /// copy constructor
    nFixedArray(const nFixedArray<TYPE>& rhs);
    /// destructor
    ~nFixedArray();
    /// set size of array (deletes previous contents!)
    void SetSize(int s);
    /// get array size
    int Size() const;
    /// fill array with a given value
    void Clear(TYPE elm);
    /// assignment operator
    void operator=(const nFixedArray<TYPE>& rhs);
    /// [] operator
    TYPE& operator[](int index) const;

private:
    /// delete content
    void Delete();
    /// allocate array for given size
    void Allocate(int s);
    /// copy content
    void Copy(const nFixedArray<TYPE>& src);

    int size;
    TYPE* elements;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nFixedArray<TYPE>::nFixedArray() :
    size(0),
    elements(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nFixedArray<TYPE>::Delete()
{
    if (this->elements)
    {
        delete[] this->elements;
        this->elements = 0;
    }
    this->size = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nFixedArray<TYPE>::Allocate(int s)
{
    this->Delete();
    if (s > 0)
    {
        this->elements = new TYPE[s];
        this->size = s;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nFixedArray<TYPE>::Copy(const nFixedArray<TYPE>& rhs)
{
    if (rhs.size != this->size)
    {
        this->Allocate(rhs.size);
    }
    int i;
    for (i = 0; i < this->size; i++)
    {
        this->elements[i] = rhs.elements[i];
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nFixedArray<TYPE>::nFixedArray(int s) :
    size(0),
    elements(0)
{
    this->Allocate(s);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nFixedArray<TYPE>::nFixedArray(const nFixedArray<TYPE>& rhs) :
    size(0),
    elements(0)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nFixedArray<TYPE>::~nFixedArray()
{
    if (0 != this->elements)
    {
        delete[] this->elements;
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nFixedArray<TYPE>::operator=(const nFixedArray<TYPE>& rhs)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
nFixedArray<TYPE>::operator[](int index) const
{
    ASSERT(this->elements && (index >= 0) && (index < this->size));
    return this->elements[index];
}

//------------------------------------------------------------------------------
/**
    Set the size of the array.
*/
template<class TYPE>
void
nFixedArray<TYPE>::SetSize(int s)
{
    this->Allocate(s);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
int
nFixedArray<TYPE>::Size() const
{
    return this->size;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nFixedArray<TYPE>::Clear(TYPE elm)
{
    ASSERT(this->elements);
    int i;
    for (i = 0; i < this->size; i++)
    {
        this->elements[i] = elm;
    }
}

//------------------------------------------------------------------------------
#endif
    
