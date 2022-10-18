#ifndef N_NODE_H
#define N_NODE_H
//------------------------------------------------------------------------------
/**
    @brief Implement a node in a doubly linked list.

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/

#include "../ProgramCommon/Define.h"


//------------------------------------------------------------------------------
class nNode 
{
public:
    /// the default constructor
    nNode();
    /// constructor providing user data pointer
    nNode(void *ptr);
    /// the destructor
    ~nNode();
    /// get the next node in the list
    nNode *GetSucc() const;
    /// get the previous node in the list
    nNode *GetPred() const;
    /// insert this node before 'succ' node into list
    void InsertBefore(nNode *succ);
    /// insert this node after 'pred' node into list
    void InsertAfter(nNode *pred);
    /// remove node from list
    void Remove();
    /// set user data pointer
    void SetPtr(void *p);
    /// get user data pointer
    void *GetPtr() const;
    /// check if node is currently linked into a list
    bool IsLinked() const;

private:
    friend class nList;
    nNode *succ;
    nNode *pred;
    void *ptr;
};

//-----------------------------------------------------------------------------
/**
*/
inline 
nNode::nNode(void)
: succ(0),
  pred(0),
  ptr(0)
{
    // empty
}

//-----------------------------------------------------------------------------
/**
*/
inline 
nNode::nNode(void *_ptr)
: succ(0),
  pred(0),
  ptr(_ptr)
{
    // empty
}

//-----------------------------------------------------------------------------
/**
    The destructor will throw an assertion if the node is still linked
    into a list!
*/
inline 
nNode::~nNode(void)
{
    ASSERT(!this->succ);
}

//-----------------------------------------------------------------------------
/**
    Get the node after this node in the list, return 0 if there is no
    next node.

    @return the next node or 0
*/
inline
nNode*
nNode::GetSucc(void) const
{
    ASSERT(this->succ);
    if (this->succ->succ) 
        return this->succ;
    else                  
        return 0;
}

//-----------------------------------------------------------------------------
/**
    Get the node before this node in the list, return 0 if there is no
    previous node.
    
    @return the previous node or 0
*/
inline
nNode*
nNode::GetPred(void) const
{
    ASSERT(this->pred);
    if (this->pred->pred) 
        return this->pred;
    else                  
        return NULL;
}

//-----------------------------------------------------------------------------
/**
    @param succ    node in front of which this node should be inserted
*/
inline 
void 
nNode::InsertBefore(nNode *succ)
{
    ASSERT(succ->pred);
    ASSERT(!this->succ);
    nNode *pred = succ->pred;
    this->pred = pred;
    this->succ = succ;
    pred->succ = this;
    succ->pred = this;
}

//-----------------------------------------------------------------------------
/**
    @param pred     the node after which this node should be inserted
*/
inline 
void 
nNode::InsertAfter(nNode *pred)
{
    ASSERT(pred->succ);
    ASSERT(!this->succ);
    nNode *succ = pred->succ;
    this->pred = pred;
    this->succ = succ;
    pred->succ = this;
    succ->pred = this;
}

//-----------------------------------------------------------------------------
/**
*/
inline 
void 
nNode::Remove(void)
{
    ASSERT(this->succ);
    nNode *succ = this->succ;
    nNode *pred = this->pred;
    succ->pred = pred;
    pred->succ = succ;
    this->succ = NULL;
    this->pred = NULL;
}

//-----------------------------------------------------------------------------
/**
    @param p       the new user data pointer
*/
inline 
void 
nNode::SetPtr(void *p)
{
    this->ptr = p;
}

//-----------------------------------------------------------------------------
/**
    @return the user data pointer
*/
inline
void*
nNode::GetPtr() const
{
    return this->ptr;
}

//-----------------------------------------------------------------------------
/**
    @return true if node is currently linked into a list
*/
inline 
bool 
nNode::IsLinked(void) const
{
    if (this->succ) 
    {
        return true;
    }
    else
    {
        return false;
    }
};

//--------------------------------------------------------------------
#endif
