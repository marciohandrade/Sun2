//====================================================
//
// matrix.c / Generate Matrix Card 
//
// Copyright by The9 Corp. 2007.2
//
//====================================================

#include "../include/main.h"
#include "../include/SHA1.h"

extern void SHA1_Prepare();
extern void SHA1_ProcessVoidSizet(const void* buffer, size_t size);
extern void SHA1_Finish(SHA1_Digest digest);
extern void SHA1_HashDigestVoidSizet(SHA1_Digest *digest, const void *data, size_t size);

void Random_Process();

typedef SHA1_Digest DIGEST;

static size_t m_used;
static DIGEST m_data;
static DIGEST m_key0;
static DIGEST m_key1;

//------------------------------------------------------------------------------

void Random_Init(const unsigned char data[], size_t size)
{
    const unsigned char* d = (const unsigned char*)data;
    SHA1_HashDigestVoidSizet(&m_key0, d, size / 2);
    memset(&m_data, 0, sizeof (m_data));
    SHA1_HashDigestVoidSizet(&m_key1, d + (size / 2), size - (size / 2));
    Random_Process();
}

void Random_Read(unsigned long data[], size_t size)
{
    size_t i;
    for (i = 0; i < size; i++) 
    {
        if (m_used == sizeof (m_data))
        {
            Random_Process();
        }
        ((unsigned char *)(data))[i]= m_data[m_used++];
    }
}

void Random_Process()
{
    SHA1_Prepare();
    SHA1_ProcessVoidSizet(m_key0, SHA1_SIZE);
    SHA1_ProcessVoidSizet(m_data, SHA1_SIZE);
    SHA1_ProcessVoidSizet(m_key1, SHA1_SIZE);
    SHA1_Finish(m_data);
    m_used = 0;
}



