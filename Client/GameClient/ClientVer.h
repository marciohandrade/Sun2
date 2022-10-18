/**
*	@file		ClientVer.h
*	@brief		client version header
*  @author		Unknown
*	@version	Unknown
*/

#pragma once

/////////////////////////////////////////////////////////////
// ���� ���� ��Ģ
// 1. ����������Ƚ��_���峯¥ (ex. 081701_1001  :0817������ 1��° ����_������ ���� 10�� 1��)
//	0817			// ����������
//	   + 01			// ���� Ƚ��
//	       _1001	// ������ ��¥
//
// 2. �������� �����ÿ��� "������" "���� Ƚ��"�� �� �ܰ辿 �ø���.
//	  (Ex. 081701_1001   => 081702_1001 )
//
// 3. BuildSystem�� �̿��Ͽ� ���带 �ϴ� ��쿣 ������ �ڵ����� �ö󰣴�. ��,������ ������Ʈ�� ���� �ѹ��� ��������� ����������+00���� ������ ��
//    (Ex. 0905������ ù������ ���� ����-> 090500_0000 �� ������ Commit. ����, BuildSystem�� ���� ���� ������ �ϸ� 090501_���峯¥�� �ڵ����� ���õ� )
/////////////////////////////////////////////////////////////

//client version info

#ifdef _CHINA
const char	c_tcBUILD_VERSION[] = "cn110201_0524";
#elif defined _JAPAN
const char	c_tcBUILD_VERSION[] = "jp120200_0000";
#elif defined _GLOBAL
const char	c_tcBUILD_VERSION[] = "gsp090402_0322";
#elif defined (_RUSSIA)
const char	c_tcBUILD_VERSION[] = "ru100402_0106";
#else // _KOREA
const char	c_tcBUILD_VERSION[] = "160115_0419";
#endif




