{
// Src Name 
BYTE	byTempArray[4];
BYTE*	pbySrcArray = (BYTE*)BINGLEBANGLE_POINTER_VARIABLE;

#define FI_ENCODE_LIST_NODE(arr1, arrIdx1, arr2, arrIdx2, apply)		arr2[arrIdx2] = apply ^ arr1[arrIdx1]

#include BINGLEBANGLE_RND_LIST_FILE

memcpy( pbySrcArray, byTempArray, sizeof(byTempArray) );
};
#undef FI_ENCODE_LIST_NODE
#undef BINGLEBANGLE_RND_LIST_FILE
#undef BINGLEBANGLE_POINTER_VARIABLE
