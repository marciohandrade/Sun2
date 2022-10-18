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

    //컨텐츠의 베타키가 나의 베타키에 포함되어있는지 검사한다.
    bool CanAbilityBetaKey(UINT32 ability_key) const
    {
        if((ability_key_ & ability_key) &&
            ability_key != IMPOSSIBILE_BETAKEY)
        {
            return true;
        }
        return false;
    }

    //컨텐츠의 베타키로 해당 컨텐츠를 사용할수 있는지 검사한다.
    bool CanAbilityContent(UINT32 ability_key) const
    {  
        //0 값은 배타키와 관계 없는 컨텐츠 이다. (베타키와 관계없이 사용가능하다.)
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
        ability_key_ = static_cast<UINT32>(beta_key / DIVIDE_DIGIT); //상위 6자리만
        unique_key_  = static_cast<UINT32>(beta_key % DIVIDE_DIGIT); //하위 8자리만
    }
    void operator=(char* beta_key)
    {
        betkey_ = _atoi64(beta_key);
        ability_key_ = static_cast<UINT32>(betkey_ / DIVIDE_DIGIT); //상위 6자리만
        unique_key_  = static_cast<UINT32>(betkey_ % DIVIDE_DIGIT); //하위 8자리만
    }

private:
    BETAKEY betkey_;     //12 자리의 베타키가 들어온다.
    UINT32 ability_key_; //앞의 6자리는 사용자가 가진 베타키가 어느 기능을 사용할수 있는지 검사함다.
    UINT32 unique_key_;  //뒤의 8자리는 유일성을 보장하기 위한 값으로 사용된다.
};

#endif //_NA_20100307_BETAKEY_SYSTEM