#pragma once

namespace WGPL
{
	inline void Encrypt(unsigned int seed, char* plainBuf, int len)
	{
		switch (len)
		{
		case 1:
			{
				BYTE mask = (BYTE)(seed >> 1);
				plainBuf[0] ^= mask;
				return;
			}
		case 2:
			{
				unsigned short mask = (unsigned short)(seed >> 3);
				unsigned short& val = *(unsigned short *)plainBuf;
				val ^= mask;
				return;
			}
		case 4:
			{
				unsigned int mask = (unsigned int)(seed >> 5);
				unsigned int& val = *(unsigned int *)plainBuf;
				val ^= mask;
				return;
			}
		case 8:
			{
				unsigned int mask = (unsigned int)(seed >> 13);
				unsigned int& highval = *(unsigned int *)plainBuf;
				highval ^= mask;
				unsigned int& lowval = *(unsigned int *)(plainBuf + 4);
				lowval ^= mask;
				return;
			}
		default:
			assert(false);
		}
	}

	inline void Decrypt(unsigned int seed, char* cipherBuf, int len)
	{
		switch (len)
		{
		case 1:
			{
				BYTE mask = (BYTE)(seed >> 1);
				cipherBuf[0] ^= mask;
				return;
			}
		case 2:
			{
				unsigned short mask = (unsigned short)(seed >> 3);
				unsigned short& val = *(unsigned short *)cipherBuf;
				val ^= mask;
				return;
			}
		case 4:
			{
				unsigned int mask = (unsigned int)(seed >> 5);
				unsigned int& val = *(unsigned int *)cipherBuf;
				val ^= mask;
				return;
			}
		case 8:
			{
				unsigned int mask = (unsigned int)(seed >> 13);
				unsigned int& highval = *(unsigned int *)cipherBuf;
				highval ^= mask;
				unsigned int& lowval = *(unsigned int *)(cipherBuf + 4);
				lowval ^= mask;
				return;
			}
		default:
			assert(false);
		}
	}
}
