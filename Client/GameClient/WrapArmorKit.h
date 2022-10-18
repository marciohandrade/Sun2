#ifndef __WRAP_ARMORKIT_H__
#define __WRAP_ARMORKIT_H__

bool ArmorKit_Init(HWND hWnd, HINSTANCE hInst);
void ArmorKit_Update();
bool ArmorKit_Suspended();
void ArmorKit_Shut();



#ifdef TEMP_WORK_ARMORKIT_CONVERT_ASCII
INT	 ArmorKit_SetPatternList(char *list, int length);
#else //TEMP_WORK_ARMORKIT_CONVERT_ASCII
INT	 ArmorKit_SetPatternList(TCHAR *list, int length);
#endif // TEMP_WORK_ARMORKIT_CONVERT_ASCII



INT ArmorKit_GenerateSid(int seed, char *sid );

#endif
