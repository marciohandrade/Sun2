#ifndef N_MSGNODE_H
#define N_MSGNODE_H
//------------------------------------------------------------------------------
/**
    @briefA doubly linked list node which can carry a msg.

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/

#include <string.h>
#include "nnode.h"

//------------------------------------------------------------------------------
class nMsgNode : public nNode 
{
public:
    /// constructor
    nMsgNode(void* buf, int size);
    /// destructor
    ~nMsgNode();
    /// get pointer to message
    void* GetMsgPtr() const;
    /// get size of message
    int GetMsgSize() const;

private:
    char *msgBuf;
    int msgSize;
};

//------------------------------------------------------------------------------
/**
*/
inline 
nMsgNode::nMsgNode(void *buf, int size)
{
    ASSERT(buf);
    ASSERT(size > 0);
    this->msgBuf  = (char*) n_malloc(size);
    this->msgSize = size;
    ASSERT(this->msgBuf);
    memcpy(this->msgBuf, buf, size);
}

//------------------------------------------------------------------------------
/**
*/
inline 
nMsgNode::~nMsgNode()
{
    n_free(this->msgBuf);
}     

//------------------------------------------------------------------------------
/**
*/
inline 
void*
nMsgNode::GetMsgPtr() const
{
    return this->msgBuf;
}

//------------------------------------------------------------------------------
/**
*/
inline 
int 
nMsgNode::GetMsgSize() const
{
    return this->msgSize;
}

//------------------------------------------------------------------------------
#endif
