#ifndef __UTIL_AUDIOMIXER_H__
#define __UTIL_AUDIOMIXER_H__
// util_audiomixer.h


#define MAX_SRC_MICROPHONE	5

BOOL amdInitialize(LPHMIXER phmx_speaker, LPHMIXER phmx_microphone);
BOOL amdUninitialize(HMIXER hMixer);


//////////////////////////////////////////////////////////////////////////
// master mixer

BOOL amdGetMasterVolumeControl(HMIXER hMixer, int* pnID, DWORD* pdwMin, DWORD* pdwMax);
BOOL amdGetMasterVolumeValue(HMIXER hMixer, int nID, DWORD* pdwVal);
BOOL amdSetMasterVolumeValue(HMIXER hMixer, int nID, DWORD dwVal);

BOOL amdGetMasterPanControl(HMIXER hMixer, int* pnID, LONG* plMin, LONG* plMax);
BOOL amdGetMasterPanValue(HMIXER hMixer, int nID, LONG* plVal);
BOOL amdSetMasterPanValue(HMIXER hMixer, int nID, LONG lVal);

BOOL amdGetMasterMuteControl(HMIXER hMixer, int* pnID);
BOOL amdGetMasterMuteValue(HMIXER hMixer, int nID, LONG* lVal);
BOOL amdSetMasterMuteValue(HMIXER hMixer, int nID, LONG lVal);

void amdGetVolume(HMIXER hMixer, int nID, DWORD dwMin, DWORD dwMax, int* pLeft, int* pRight);
void amdSetVolume(HMIXER hMixer, int nID, DWORD dwMin, DWORD dwMax, int left, int right);

//////////////////////////////////////////////////////////////////////////
// wav mixer

BOOL amdGetWavVolumeControl(HMIXER hMixer, int* pnID, DWORD* pdwMin, DWORD* pdwMax);
BOOL amdGetWavVolumeValue(HMIXER hMixer, int nID, DWORD* pdwVal);
BOOL amdSetWavVolumeValue(HMIXER hMixer, int nID, DWORD dwVal);

BOOL amdGetWavPanControl(HMIXER hMixer, int* pnID, LONG* plMin, LONG* plMax);
BOOL amdGetWavPanValue(HMIXER hMixer, int nID, LONG* plVal);
BOOL amdSetWavPanValue(HMIXER hMixer, int nID, LONG lVal);

BOOL amdGetWavMuteControl(HMIXER hMixer, int* pnID);
BOOL amdGetWavMuteValue(HMIXER hMixer, int nID, LONG* lVal);
BOOL amdSetWavMuteValue(HMIXER hMixer, int nID, LONG lVal);

//////////////////////////////////////////////////////////////////////////
// mic

BOOL amdGetMicInputVolumeControl(HMIXER hMixer, int* pnID, DWORD* pdwMin, DWORD* pdwMax, DWORD* pdwCount);
BOOL amdGetMicInputVolumeValue(HMIXER hMixer, int nID, DWORD* pdwVal, DWORD dwCount);
BOOL amdSetMicInputVolumeValue(HMIXER hMixer, int* pnID, DWORD dwVal, DWORD dwCount);

BOOL amdGetMicOutputVolumeControl(HMIXER hMixer, int* pnID, DWORD* pdwMin, DWORD* pdwMax);
BOOL amdGetMicOutputVolumeValue(HMIXER hMixer, int nID, DWORD* pdwVal);
BOOL amdSetMicOutputVolumeValue(HMIXER hMixer, int nID, DWORD dwVal);

BOOL amdGetMicPanControl(HMIXER hMixer, int* pnID, LONG* plMin, LONG* plMax);
BOOL amdGetMicPanValue(HMIXER hMixer, int nID, LONG* plVal);
BOOL amdSetMicPanValue(HMIXER hMixer, int nID, LONG lVal);

BOOL amdGetMicOutputMuteControl(HMIXER hMixer, int* pnID);
BOOL amdGetMicOutputMuteValue(HMIXER hMixer, int nID, LONG* lVal);
BOOL amdSetMicOutputMuteValue(HMIXER hMixer, int nID, LONG lVal);


void SelectMic(HMIXER hmx);


#endif //__UTIL_AUDIOMIXER_H__