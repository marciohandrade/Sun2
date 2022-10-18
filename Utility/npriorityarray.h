#ifndef N_PRIORITYARRAY_H
#define N_PRIORITYARRAY_H
//------------------------------------------------------------------------------
/**
    @briefA fixed size priority array. Elements are associated with a priority.

    New Elements are added to the end of the array until the array is full.
    In a full array, new elements replace the current lowest priority element
    (if the priority of the new element is greater of course). 

    NOTE: The current implementation uses linear search and thus is slow
    for large arrays.

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/
#include "../ProgramCommon/Define.h"

//------------------------------------------------------------------------------
template<class TYPE> class nPriorityArray
{
public:
    /// constructor
    nPriorityArray(int size);
    /// copy constructor
    nPriorityArray(const nPriorityArray<TYPE>& rhs);
    /// destructor
    ~nPriorityArray();
    /// assignment operator
    nPriorityArray<TYPE>& operator=(const nPriorityArray<TYPE>& rhs);
    /// [] operator
    TYPE& operator[](int index) const;
    /// add element to array
    void Add(const TYPE& elm, float pri);
    /// get number of elements in array
    int Size() const;
    /// return n'th array element
    TYPE& At(int index);

private:
    /// update the min pri element index
    void UpdateMinPriElementIndex();
    /// copy content
    void Copy(const nPriorityArray<TYPE>& src);
    /// delete content
    void Delete();

    /// an element class
    struct Element
    {
        TYPE element;
        float priority;
    };

    int numElements;
    int maxElements;
    int minPriElementIndex;
    Element* elements;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nPriorityArray<TYPE>::nPriorityArray(int size) :
    numElements(0),
    maxElements(size),
    minPriElementIndex(0)
{
    ASSERT(size > 0);
    this->elements = new Element[size];
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nPriorityArray<TYPE>::Copy(const nPriorityArray<TYPE>& src) :
    numElements(src.numElements),
    maxElements(src.maxElements),
    minPriElementIndex(src.minPriElementIndex)
{
    ASSERT(0 == this->elements);
    this->elements = new Element[this->maxElements];
    int i;
    for (i = 0; i < this->numElements; i++)
    {
        this->elements[i] = src.elements[i];
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nPriorityArray<TYPE>::Delete()
{
    this->numElements = 0;
    this->maxElements = 0;
    this->minPriElementIndex = 0;
    if (this->elements)
    {
        delete[] this->elements;
        this->elements = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nPriorityArray<TYPE>::nPriorityArray(const nPriorityArray<TYPE>& rhs) :
    numElements(0),
    maxElements(0),
    minPriElementIndex(0),
    elements(0)
{
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nPriorityArray<TYPE>::~nPriorityArray()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nPriorityArray<TYPE>::UpdateMinPriElementIndex()
{
    int i;
    this->minPriElementIndex = 0;
    float minPri = this->elements[0].priority;
    for (i = 1; i < this->numElements; i++)
    {
        if (this->elements[i].priority < minPri)
        {
            minPri = this->elements[i].priority;
            this->minPriElementIndex = i;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nPriorityArray<TYPE>::Add(const TYPE& elm, float pri)
{
    if (this->numElements < this->maxElements)
    {
        this->elements[this->numElements].element = elm;
        this->elements[this->numElements].priority = pri;
        this->numElements++;
        if (this->numElements == this->maxElements)
        {
            this->UpdateMinPriElementIndex();
        }
    }
    else
    {
        if (pri > this->elements[this->minPriElementIndex].priority)
        {
            this->elements[this->minPriElementIndex].element = elm;
            this->elements[this->minPriElementIndex].priority = pri;
            this->UpdateMinPriElementIndex();
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
int
nPriorityArray<TYPE>::Size() const
{
    return this->numElements;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
nPriorityArray<TYPE>::At(int index)
{
    ASSERT((index >= 0) && (index < this->numElements));
    return this->elements[index].element;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE&
nPriorityArray<TYPE>::operator[](int index) const
{
    ASSERT((index >= 0) && (index < this->numElements));
    return this->elements[index].element;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nPriorityArray<TYPE>& 
nPriorityArray<TYPE>::operator=(const nPriorityArray<TYPE>& rhs)
{
    this->Delete();
    this->Copy(rhs);
    return *this;
}

//------------------------------------------------------------------------------
#endif

   
    