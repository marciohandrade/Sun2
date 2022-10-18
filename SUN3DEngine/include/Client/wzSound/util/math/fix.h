
#include "PreProcDefines.h"
#ifndef _FIX_H
#define _FIX_H


#define F1_0 65536
#define f1_0 65536

fix fixmul(fix a, fix b);
fix fixdiv(fix a, fix b);
fix fixmuldiv(fix a, fix b, fix c);

#define f2i(a) ((int)((a)>>16))
#define i2f(a) ((fix)((a)<<16))

#endif
