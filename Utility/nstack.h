#ifndef N_STACK_H
#define N_STACK_H
//------------------------------------------------------------------------------
/**
    @brief A stack template class.

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/
#include "../ProgramCommon/Define.h"
#include "nlist.h"
#include "nnode.h"

//------------------------------------------------------------------------------
template<class TYPE> class nStack
{
public:
    /// destructor
    ~nStack();
    /// push element onto stack
    void Push(TYPE e);
    /// pop element from stack
    TYPE Pop();
    /// get top element of stack
    const TYPE& Top() const;
    /// check if stack is empty
    bool IsEmpty() const;

private:
    /// a private stack node class
    class nStackNode : public nNode
    {
#if defined(__WIN32__)
    #if _MSCVER < 1300
        friend nStack;
    #else
        friend class nStack;
    #endif
#else
        friend class nStack;
#endif

        /// constructor
        nStackNode(const TYPE& e)
        {
            this->elm = e;
        }

        /// get embedded element
        const TYPE& Get() const
        {
            return this->elm;
        }

        TYPE elm;
    };

    nList l;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
nStack<TYPE>::~nStack()
{
    // delete any remaining elements on stack
    nStackNode* sn;
    while (sn = (nStackNode*) l.RemHead())
    {
        delete sn;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
void
nStack<TYPE>::Push(TYPE e)
{
    nStackNode* sn = new nStackNode(e);
    l.AddHead(sn);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
TYPE
nStack<TYPE>::Pop()
{
    nStackNode* sn = (nStackNode*) l.RemHead();
    if (sn)
    {
        TYPE t = sn->Get();
        delete sn;
        return t;
    }
    else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
const TYPE&
nStack<TYPE>::Top() const
{
    nStackNode* sn = (nStackNode*) l.GetHead();
    ASSERT(sn);
    return sn->Get();
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
bool
nStack<TYPE>::IsEmpty() const
{
    return l.IsEmpty();
}

//------------------------------------------------------------------------------
#endif

