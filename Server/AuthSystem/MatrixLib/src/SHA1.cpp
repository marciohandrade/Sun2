//====================================================
//
// SHA1.c / Generate Matrix Card 
//
// Copyright by The9 Corp. 2007.2
//
//====================================================
#include "../include/main.h"
#include "../include/SHA1.h"

const unsigned long K1 = 0x5A827999;
const unsigned long K2 = 0x6ED9EBA1;
const unsigned long K3 = 0x8F1BBCDC;
const unsigned long K4 = 0xCA62C1D6;
const unsigned long MASK = SHA1_BLOCK - 1;

static unsigned long long m_size;
static unsigned long m_digest[SHA1_SIZE / sizeof (unsigned long)];
static unsigned char m_block[SHA1_BLOCK];

//-----------------------------------------------------------------------------
unsigned long S(unsigned long x, int n) 
{ 
    return ((x << n) | (x >> (32 - n)));
}

void R1(unsigned long a, unsigned long *b, unsigned long c, unsigned long d, unsigned long *e, unsigned long *w) 
{
    *e += S(a, 5) + ((*b & c) | ((~*b) & d)) + *w + K1;
    *b = S(*b, 30);
    *w = 0;
}

void R2(unsigned long a, unsigned long *b, unsigned long c, unsigned long d, unsigned long *e, unsigned long *w) 
{
    *e += S(a, 5) + (*b ^ c ^ d) + *w + K2;
    *b = S(*b, 30);
    *w = 0;
}

void R3(unsigned long a, unsigned long *b, unsigned long c, unsigned long d, unsigned long *e, unsigned long *w) 
{
    *e += S(a, 5) + ((*b & c) | (*b & d) | (c & d)) + *w + K3;
    *b = S(*b, 30);
    *w = 0;
}

void R4(unsigned long a, unsigned long *b, unsigned long c, unsigned long d, unsigned long *e, unsigned long *w) 
{
    *e += S(a, 5) + (*b ^ c ^ d) + *w + K4;
    *b = S(*b, 30);
    *w = 0;
}

//-----------------------------------------------------------------------------
void Load(register unsigned long *a, register const unsigned char b[])
{   
    *a = b[0] << 24 | b[1] << 16 | b[2] << 8 | b[3];
}


void SaveULL(unsigned long long a, unsigned char b[])
{
    int i;
    for (i = sizeof (unsigned long long) - 1; i >= 0; i--)
    {
        b[i] = (unsigned char)(a); a >>= 8; 
    }
}

void SaveUL(unsigned long a, unsigned char b[])
{
    int i;
    for (i = sizeof (unsigned long) - 1; i >= 0; i--)
    {
        b[i] = (unsigned char)(a); a >>= 8; 
    }
}

//---------------------------------------------------------------------------
void Process(unsigned long hash[], const unsigned char data[])
{
    int i;
    unsigned long w[80];
    unsigned long a,b,c,d,e;

    for (i = 0; i < SHA1_BLOCK / 4; i++, data += 4)
    {
        Load(&(w[i]), data);
    }

    do 
    {
        w[i] = S(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }
    while (++i < 80);

    a = hash[0];
    b = hash[1];
    c = hash[2];
    d = hash[3];
    e = hash[4];

    i = 0;

    // round 1
    do {
        R1(a, &b, c, d, &e, &(w[i + 0]));
        R1(e, &a, b, c, &d, &(w[i + 1]));
        R1(d, &e, a, b, &c, &(w[i + 2]));
        R1(c, &d, e, a, &b, &(w[i + 3]));
        R1(b, &c, d, e, &a, &(w[i + 4]));
        i += 5;
    } while (i < 20);

    // round 2
    do {
        R2(a, &b, c, d, &e, &(w[i + 0]));
        R2(e, &a, b, c, &d, &(w[i + 1]));
        R2(d, &e, a, b, &c, &(w[i + 2]));
        R2(c, &d, e, a, &b, &(w[i + 3]));
        R2(b, &c, d, e, &a, &(w[i + 4]));
        i += 5;
    } while (i < 40);

    // round 3
    do {
        R3(a, &b, c, d, &e, &(w[i + 0]));
        R3(e, &a, b, c, &d, &(w[i + 1]));
        R3(d, &e, a, b, &c, &(w[i + 2]));
        R3(c, &d, e, a, &b, &(w[i + 3]));
        R3(b, &c, d, e, &a, &(w[i + 4]));
        i += 5;
    } while (i < 60);

    // round 4
    do {
        R4(a, &b, c, d, &e, &(w[i + 0]));
        R4(e, &a, b, c, &d, &(w[i + 1]));
        R4(d, &e, a, b, &c, &(w[i + 2]));
        R4(c, &d, e, a, &b, &(w[i + 3]));
        R4(b, &c, d, e, &a, &(w[i + 4]));
        i += 5;
    } while (i < 80);

    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
    hash[4] += e;
}

//---------------------------------------------------------------------------
void SHA1_Prepare()
{
    m_size = 0;
    m_digest[0] = 0x67452301;
    m_digest[1] = 0xEFCDAB89;
    m_digest[2] = 0x98BADCFE;
    m_digest[3] = 0x10325476;
    m_digest[4] = 0xC3D2E1F0;
}

//-----------------------------------------------------------------------------
void SHA1_ProcessVoidSizet(const void* buffer, size_t size)
{
    const unsigned char *data = (const unsigned char*)(buffer);
    unsigned long used = ((unsigned long)(m_size) / 8) & MASK;
    m_size += (unsigned long long)(size) * 8;

    if (used != 0) 
    {
        size += used;
        data -= used;

        if (size >= SHA1_BLOCK) 
        {
            for (; used < SHA1_BLOCK; used++)
            {
                m_block[used] = data[used];
            }
            Process(m_digest, m_block);
            data += SHA1_BLOCK;
            size -= SHA1_BLOCK;
            used = 0;
        }
    }

    for (; size >= SHA1_BLOCK; data += SHA1_BLOCK, size -= SHA1_BLOCK) 
    {
        Process(m_digest, data);
    }
    for (; used < size; used++)
    {
        m_block[used] = data[used];
    }
}

//---------------------------------------------------------------------------
void SHA1_ProcessString(const char* string)
{
    SHA1_ProcessVoidSizet(string, strlen(string));
}

//-----------------------------------------------------------------------------
void SHA1_Finish(SHA1_Digest digest)
{
    size_t i;
    static const unsigned char pad[] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    unsigned char size[8];
    SaveULL(m_size, size);
    SHA1_ProcessVoidSizet(pad, 1 + (((SHA1_BLOCK - 9) - ((unsigned long)(m_size) / 8)) & MASK));
    SHA1_ProcessVoidSizet(size, sizeof (size));

    for ( i = 0; i < SHA1_SIZE / 4; i++) 
    {
        SaveUL(m_digest[i], &digest[i * 4]);
    }
}

//-----------------------------------------------------------------------------
void SHA1_HashDigestVoidSizet(SHA1_Digest *digest, const void *data, size_t size)
{
    SHA1_Prepare();
    SHA1_ProcessVoidSizet(data, size);
    SHA1_Finish(*digest);
}

//-----------------------------------------------------------------------------
void SHA1_Hash_DigestString(SHA1_Digest *digest, const char* string)
{
    SHA1_Prepare();
    SHA1_ProcessVoidSizet(string, strlen(string));
    SHA1_Finish(*digest);
}
