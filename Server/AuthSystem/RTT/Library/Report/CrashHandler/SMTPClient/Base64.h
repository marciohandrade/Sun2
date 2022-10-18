#ifndef  __Base64_H
#define  __Base64_H

void		Base64Encode( LPCTSTR data, int size, LPTSTR output, int bufLen );
void		Base64Decode( LPCTSTR data, int size, LPTSTR output, int bufLen );

#endif