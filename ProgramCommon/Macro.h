
#ifndef __MACRO_H__
#define __MACRO_H__

// 비트 연산 지완 추가 
#define BIT_CHECK(x,b)		(((x) & (b)) ? true:false)
#define BIT_REMOVE(x,b)		((x) = ((x) & ~(b)))
#define BIT_ADD(x,b)		((x) = ((x) | (b)))	

#endif // __MACRO_H__