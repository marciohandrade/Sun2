#ifndef __CRYPTO_SHA1_H
#define __CRYPTO_SHA1_H

enum 
{ 
    SHA1_SIZE = 20,
    SHA1_BLOCK = 64
};

typedef unsigned char SHA1_Digest[SHA1_SIZE];

void SHA1_Prepare();
void SHA1_ProcessVoidSizet(const void* buffer, size_t size);
void SHA1_Finish(SHA1_Digest digest);
void SHA1_HashDigestVoidSizet(SHA1_Digest *digest, const void *data, size_t size);

#endif

