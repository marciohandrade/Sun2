// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// ��� ������Ʈ �� ���� ���� ��ġ�� �����̶�� �����Ͽ� �����Ǵ� ��� ��ġ�� ���� Include
#include "../include/Global.Define.h"

// Windows ��� �����Դϴ�.
#include <windows.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <assert.h>
#include <hash_map>
#include <SolarHashTable.h>
#include <SolarFileReader.h>
#include <Timer.h>
#include <MemoryPoolFactory.h>
#include <Define.h>
#include <Const.h>
#include "QuestDefine.h"
#include "QuestStruct.h"
#include <ScriptCode.IParser.h>
