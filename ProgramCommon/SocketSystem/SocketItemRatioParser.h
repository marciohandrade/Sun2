#pragma once

#ifndef PROGRAMCOMMON_SOCKETSYSTEM_SOCKETITEMRATIOPARSER_H_
#define PROGRAMCOMMON_SOCKETSYSTEM_SOCKETITEMRATIOPARSER_H_

#include <ItemOptionHeader.h>
#include <Singleton.h>
#include <ScriptCode.IParser.h>

struct SocketItemRatioInfo
{
    enum { kMaxRatio = 1000 };
    struct RatioType
    {
        enum 
        { 
            kWeapon = 0, // ����
            kArmor = 1, // ��
            kRecreateWeapon = 2, // ���� �ٽ� ���� ��û�� �� ����
            kRecreateArmor = 3, // ���� �ٽ� ���� ��û�� �� ��
            kMax = 0xFF, // �ִ밪
            kInvalid
        };
        typedef WORD Value;
    };
    typedef WORD ItemType;
    typedef LEVELTYPE ItemLevel;
    typedef WORD Ratio;
    
    RatioType::Value ratio_type_; // ������ Ÿ�� (�� ������ �� ����)
    ItemLevel item_level_; // ������ ����
    Ratio socket_creation_ratio_; // ���� �������� �� Ȯ�� (õ����)
    Ratio socket_ratio_[SOCKET_MAX]; // �� ���� ���� Ȯ�� (�� ���Ϻ� Ȯ�� ���ļ� 1000)

    SocketItemRatioInfo() : 
        ratio_type_(RatioType::kInvalid), 
        item_level_(0), 
        socket_creation_ratio_(0)
    {
        ::ZeroMemory(socket_ratio_, sizeof(socket_ratio_));
    }
private:
    __DISABLE_COPY(SocketItemRatioInfo);
};

class SocketItemRatioParser : public util::Singleton<SocketItemRatioParser>, public IParser
{
    typedef WORD SocketRatioKey;
    typedef stdext::hash_map<SocketRatioKey, SocketItemRatioInfo*> SocketRatioMap;  
    
public:
    SocketItemRatioParser();
    ~SocketItemRatioParser();

public:
    virtual void Release();
    virtual	BOOL LoadScript(eSCRIPT_CODE script_code, BOOL is_reload);

public:
    SocketItemRatioInfo::RatioType::Value GetRatioType(
        const SocketItemRatioInfo::ItemType item_type, const bool is_recreate) const;
    
    const SocketItemRatioInfo* FindSocketRatio(
        const SocketItemRatioInfo::RatioType::Value ratio_type, 
        const SocketItemRatioInfo::ItemLevel item_level) const; 
    
private:
    bool IsValid(const SocketItemRatioInfo& socket_ratio) const;
    bool LoadSocketItemRatio(const bool is_reload);

private:
    SocketRatioMap socket_ratio_map_;
    __DISABLE_COPY(SocketItemRatioParser);
};

#endif // PROGRAMCOMMON_SOCKETSYSTEM_SOCKETITEMRATIOPARSER_H_