#ifndef N_KEYARRAY_H
#define N_KEYARRAY_H
//------------------------------------------------------------------------------
/**
    @brief Implements growable array of key-pointer pairs. The array
    is kept sorted for fast bsearch() by key  

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - ???? ?߰? 
*/

#include "../ProgramCommon/Macro.h"
#include "../ProgramCommon/Define.h"
#include "Narray.h"
//------------------------------------------------------------------------------
template<class TYPE> class nKeyArray 
{
public:
    /// constructor for non-growable array
    nKeyArray(int num);
    /// constructor for growable array    
    nKeyArray(int num, int grow);
    /// destructor
    ~nKeyArray();
    /// add key/element pair to array
    void Add(int key, const TYPE& e);
    /// find element associated with given key
    bool Find(int key, TYPE& e);
    /// find pointer to element associated with key
    bool FindPtr(int key, TYPE*& e);
    /// remove element defined by key
    void Rem(int key);
    /// remove element defined by key index
    void RemByIndex(int index);
    /// return number of elements
    int Size() const;
    /// get element at index
    TYPE& GetElementAt(int index) const;
    /// get key at index
    int GetKeyAt(int index) const;
    /// clear the array without deallocating memory!
    void Clear();

private:
    struct nKAElement
    {
        int key;
        TYPE elm;
    };

    /// allocate array
    void alloc(int num);
    /// grow array
    void grow();
    /// binary search
    nKAElement* bsearch(int key);

    int numElms;
    int maxElms;
    int growElms;
    nKAElement* curElm;
    nKAElement* elmArray;    
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nKeyArray<TYPE>::alloc(int num)
{
    ASSERT(0 == this->elmArray);
    ASSERT(0 == this->curElm);
    this->elmArray = n_new nKAElement[num];
    this->maxElms  = num;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nKeyArray<TYPE>::grow()
{
    ASSERT(this->elmArray);
    ASSERT(this->growElms > 0);
    
    int newNum = this->maxElms + this->growElms;
    nKAElement* newArray = n_new nKAElement[newNum];
    
    memcpy(newArray, this->elmArray, this->numElms * sizeof(nKAElement));
    n_delete[] this->elmArray;
    this->elmArray = newArray;
    this->maxElms  = newNum;
    this->curElm   = 0;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
typename nKeyArray<TYPE>::nKAElement*
nKeyArray<TYPE>::bsearch(int key)
{
    ASSERT(this->numElms > 0);

    int num = this->numElms;
    int half;

    nKAElement* lo = &(this->elmArray[0]);
    nKAElement* hi = &(this->elmArray[num-1]);
    nKAElement* mid;
    while (lo <= hi) 
    {
        if ((half = num/2)) 
        {
            mid = lo + ((num & 1) ? half : (half - 1));
            int diff = key - mid->key;
            if (diff < 0) 
            {
                hi = mid - 1;
                num = num & 1 ? half : half-1;
            } 
            else if (diff > 0) 
            {
                lo = mid + 1;
                num = half;
            } 
            else
            {
                return mid;
            }
        } 
        else if (num) 
        {
            int diff = key - lo->key;
            if (diff) 
            {
                return 0;
            }
            else      
            {
                return lo;
            }
        } 
        else 
        {
            break;
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nKeyArray<TYPE>::nKeyArray(int num) :
    numElms(0),
    maxElms(0),
    growElms(0),
    curElm(0),
    elmArray(0)
{
    this->alloc(num);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nKeyArray<TYPE>::nKeyArray(int num, int grow) :
    numElms(0),
    maxElms(0),
    growElms(grow),
    curElm(0),
    elmArray(0)
{
    this->alloc(num);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nKeyArray<TYPE>::~nKeyArray() 
{
    if (this->elmArray) 
    {
        n_delete[] this->elmArray;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void 
nKeyArray<TYPE>::Add(int key, const TYPE& e) 
{
    // need to grow array?
    if (this->numElms == this->maxElms) 
    {
        ASSERT(this->growElms > 0);
        this->grow();
    }

    // insert key into array, keep array sorted by key
    int i;
    for (i = 0; i < this->numElms; i++) 
    {
        nKAElement* kae = &(this->elmArray[i]);
        if (key < kae->key) 
        {
            // insert in front of 'e'
            nKAElement* kaeSucc = kae + 1;
            int numMove = this->numElms - i;
            if (numMove > 0) 
            {
                memmove(kaeSucc, kae, numMove * sizeof(nKAElement));
            }
            kae->key = key;
            kae->elm = e;
            this->numElms++;
            this->curElm = 0;
            return;
        }
    }

    // fallthrough: add element to end of array
    this->elmArray[this->numElms].key = key;
    this->elmArray[this->numElms].elm = e;
    this->numElms++;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool 
nKeyArray<TYPE>::Find(int key, TYPE& e) 
{
    if (this->numElms == 0) 
    {
        return false;
    }
    if (this->curElm && (this->curElm->key == key)) 
    {
        e = this->curElm->elm;
        return true;
    } 
    else 
    {
        nKAElement* p = this->bsearch(key);
        if (p) 
        {
            this->curElm = p;
            e = this->curElm->elm;
            return true;
        } 
        else 
        {
            return false;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool 
nKeyArray<TYPE>::FindPtr(int key, TYPE*& e) 
{
    if (this->numElms == 0) 
    {
        return false;
    }
    if (this->curElm && (this->curElm->key == key)) 
    {
        e = &(this->curElm->elm);
        return true;
    } 
    else 
    {
        nKAElement* p = this->bsearch(key);
        if (p) 
        {
            this->curElm = p;
            e = &(this->curElm->elm);
            return true;
        } 
        else 
        {
            return false;
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void 
nKeyArray<TYPE>::Rem(int key) 
{
    nKAElement* e = this->bsearch(key);
    if (e) 
    {
        this->curElm = NULL;
        this->numElms--;
        nKAElement* eSucc = e + 1;
        int i = e - this->elmArray;
        int numMove = this->numElms - i;
        if (numMove > 0) 
        {
            memmove(e, eSucc, numMove * sizeof(nKAElement));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void 
nKeyArray<TYPE>::RemByIndex(int index) 
{
    ASSERT((index >= 0) && (index < this->numElms));
    nKAElement* e = &(this->elmArray[index]);
    nKAElement* eSucc = e + 1;
    this->curElm = 0;
    this->numElms--;
    int numMove = this->numElms - index;
    if (numMove > 0) 
    {
        memmove(e, eSucc, numMove * sizeof(nKAElement));
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
int 
nKeyArray<TYPE>::Size() const
{
    return this->numElms;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE& 
nKeyArray<TYPE>::GetElementAt(int index) const
{
    ASSERT((index >= 0) && (index < this->numElms));
    return this->elmArray[index].elm;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
int 
nKeyArray<TYPE>::GetKeyAt(int index) const
{
    ASSERT((index >= 0) && (index < this->numElms));
    return this->elmArray[index].key;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void 
nKeyArray<TYPE>::Clear() 
{
    this->numElms = 0;
    this->curElm = 0;
}

//------------------------------------------------------------------------------
#endif
