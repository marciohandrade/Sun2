#pragma once

#ifdef _NA_20100307_BETAKEY_SYSTEM


class BetaKeyInfo
{
public:
    enum {DIVIDE_DIGIT = 1, 
        IMPOSSIBILE_BETAKEY = 999999};

    BetaKeyInfo() : betkey_(0), ability_key_(0), unique_key_(0)
    {
        
    }

    //�������� ��ŸŰ�� ���� ��ŸŰ�� ���ԵǾ��ִ��� �˻��Ѵ�.
    bool CanAbilityBetaKey(UINT32 ability_key) const
    {
        if((ability_key_ & ability_key) &&
            ability_key != IMPOSSIBILE_BETAKEY)
        {
            return true;
        }
        return false;
    }

    //�������� ��ŸŰ�� �ش� �������� ����Ҽ� �ִ��� �˻��Ѵ�.
    bool CanAbilityContent(UINT32 ability_key) const
    {  
        //0 ���� ��ŸŰ�� ���� ���� ������ �̴�. (��ŸŰ�� ������� ��밡���ϴ�.)
        if(((ability_key_ & ability_key) || ability_key == 0) &&
           ability_key != IMPOSSIBILE_BETAKEY)
        {
            return true;
        }
        return false;
    }

    BETAKEY GetBetaKey() const
    {
        return betkey_;
    }

public:
    // NOTE: this operators are unadvisable because of implicit conversion.
    // referenced by Decision sect., 'Operator overloading', google coding style.
    void operator=(UINT64 beta_key)
    {
        betkey_ = beta_key;
        ability_key_ = static_cast<UINT32>(beta_key / DIVIDE_DIGIT); //���� 6�ڸ���
        unique_key_  = static_cast<UINT32>(beta_key % DIVIDE_DIGIT); //���� 8�ڸ���
    }
    void operator=(char* beta_key)
    {
        betkey_ = _atoi64(beta_key);
        ability_key_ = static_cast<UINT32>(betkey_ / DIVIDE_DIGIT); //���� 6�ڸ���
        unique_key_  = static_cast<UINT32>(betkey_ % DIVIDE_DIGIT); //���� 8�ڸ���
    }

private:
    BETAKEY betkey_;     //12 �ڸ��� ��ŸŰ�� ���´�.
    UINT32 ability_key_; //���� 6�ڸ��� ����ڰ� ���� ��ŸŰ�� ��� ����� ����Ҽ� �ִ��� �˻��Դ�.
    UINT32 unique_key_;  //���� 8�ڸ��� ���ϼ��� �����ϱ� ���� ������ ���ȴ�.
};

#endif //_NA_20100307_BETAKEY_SYSTEM