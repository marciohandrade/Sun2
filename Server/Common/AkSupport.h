#ifndef __AKSUPPORT_H__
#define __AKSUPPORT_H__

#define AKS_SUCCESS		0
#define	AKS_FAIL		-1

#define AKS_ERR_INVALIDPARAMETER	101
#define AKS_ERR_NOTMATCHED			102

#define	AKS_KEYLENGTH	32

int AKS_MakeSEED(int *seed);
int AKS_GenerateSID(int seed, char *sid, int length);
int AKS_VerifySID(int seed, char *sid, int length);

#endif