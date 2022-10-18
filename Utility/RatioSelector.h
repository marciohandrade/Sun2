#pragma once

#include <hash_map>
#include <map>
#include <ctime>
#include <boost/random.hpp>

/*
// 확률(DWORD)에 따라서 Action을 하나 선택한다.

RatioSelector<int>	rs();
rs.Add( 1, 20 );
rs.Add( 2, 10 );
rs.Add( 3, 50 );
rs.Add( 4, 30 );

//rs.GenerateRatios();	// 안해도 Select()나 GetTotalRatio()에서 자동으로 됨.

// m_Ratios의 값들			// Action	Ratio		지정된 범위
// [19] = 1					// 1		20			0~19
// [29] = 2					// 2		10			20~29
// [79] = 3					// 3		50			30~79
// [109] = 4				// 4		30			80~109

int a;
assert( rs.Select( 0, a ) ); assert( a == 1 );
assert( rs.Select( 19, a ) ); assert( a == 1 );
assert( rs.Select( 20, a ) ); assert( a == 2 );
assert( rs.Select( 21, a ) ); assert( a == 2 );
assert( rs.Select( 81, a ) ); assert( a == 4 );
assert( rs.Select( 109, a ) ); assert( a == 4 );
assert( !rs.Select( 110, a ) );  // 110은 범위를 벗어나므로 실패. a값은 의미없다.
*/

// 확률 선택기 접근 방식
struct RatioAccessMode
{
    enum Value
    {
        kMutable = 0, // 
        kConst = 1 // 상수로만 접근
    };
};

// GenerateRatios()가 자동으로 호출되는 내부 멤버 변경이 일어나는 버전
template <
    typename ActionT, 
    typename RatioT = DWORD, 
    RatioT kMaxTotalRatio = 0, 
    RatioAccessMode::Value kRatioAccessMode = RatioAccessMode::kMutable
>
class RatioSelector
{
public:
    typedef stdext::hash_map<ActionT, RatioT> ACTIONS;
    
    RatioSelector() : is_generate_(true), total_ratio_(0)
    {
    }
    ~RatioSelector() {}

    void Release()
    {
        is_generate_ = true;
        total_ratio_ = 0;
        actions_.clear();
        ratios_.clear();
    }

    BOOL Add(const ActionT action, const RatioT ratio)
    {
        ACTIONS::const_iterator it = actions_.find(action);
        if (it != actions_.end())
        {
            return FALSE;
        }
        actions_[action] = ratio;
        is_generate_ = false;
        return TRUE;
    }

    BOOL Remove(const ActionT action)
    {
        ACTIONS::iterator it = actions_.find(action);
        if (it != actions_.end())
        {
            actions_.erase(it);
            is_generate_ = false;
            return TRUE;
        }
        return FALSE;
    }

    void GenerateRatios()
    {
        ratios_.clear();
        total_ratio_ = 0;
        ACTIONS::const_iterator it = actions_.begin();
        for (; it != actions_.end(); it++)
        {
            const ActionT& action = it->first;
            const RatioT ratio = it->second;
            total_ratio_ += ratio;	
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
            ratios_[total_ratio_] = action;
#else
            // 실제로는 -1까지가 action이 선택되는 영역이다.
            // ex) ratio=20이면, 0~19까지.
            ratios_[total_ratio_-1] = action;
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
        }
        is_generate_ = true;
    }

    BOOL Select(const RatioT ratio, ActionT& selectedAction)
    {
        if (is_generate_ == false)
        {
            GenerateRatios();
        }
        Ratios::const_iterator it = ratios_.lower_bound(ratio);
        if (it == ratios_.end())
        {
            return FALSE;
        }
        selectedAction = it->second;
        return TRUE;
    }

    RatioT GetTotalRatio()
    {
        if (is_generate_ == false)
        {
            GenerateRatios();
        }
        return total_ratio_; 
    }

    std::size_t GetCount() const { return actions_.size(); }
    ACTIONS& GetActionData() { return actions_; }

private:
    typedef std::map<RatioT, ActionT> Ratios;
    bool is_generate_;
    RatioT total_ratio_;
    ACTIONS actions_;
    Ratios ratios_;
};

// 초기화 시점에 확률 정보를 추가하고 이후 상수 접근으로만 사용하는 버전
template <typename ActionT, typename RatioT, RatioT kMaxTotalRatio>
class RatioSelector<ActionT, RatioT, kMaxTotalRatio, RatioAccessMode::kConst>
{
public:
    RatioSelector() : 
        total_ratio_(0), 
        distribution_(1, kMaxTotalRatio), 
        generator_(RandomEngine(std::time(NULL)), distribution_)
    {
    }
    ~RatioSelector() {}

    bool Add(const ActionT action, const RatioT ratio)
    {
        if (ratio <= 0)
        {
            return false;
        }
        if (total_ratio_ >= kMaxTotalRatio)
        {
            return false;
        }
        Actions::const_iterator it = actions_.find(action);
        if (it != actions_.end())
        {
            return false;
        }
        actions_[action] = ratio;
        total_ratio_ += ratio;
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        const RatioT ratio_key = total_ratio_;
#else
        const RatioT ratio_key = total_ratio_-1;
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
        ratios_[ratio_key] = action;
        return true;
    }

    bool Select(const RatioT ratio, ActionT& OUT selected_action) const
    {
        Ratios::const_iterator it = ratios_.lower_bound(ratio);
        if (it == ratios_.end())
        {
            return false;
        }
        selected_action = it->second;
        return true;
    }

    bool RandomSelect(ActionT& OUT selected_action) const
    {
        RandomGenerator& generate = const_cast<RandomGenerator&>(generator_);
        const RatioT ratio = generate();
        Ratios::const_iterator it = ratios_.lower_bound(ratio);
        if (it == ratios_.end())
        {
            return false;
        }
        selected_action = it->second;
        return true;
    }

    RatioT GetTotalRatio() const
    {
        return total_ratio_; 
    }

    std::size_t GetCount() const { return actions_.size(); }

private:
    typedef stdext::hash_map<ActionT, RatioT> Actions;
    typedef std::map<RatioT, ActionT> Ratios;
    typedef boost::lagged_fibonacci607 RandomEngine;
    typedef boost::uniform_int<RatioT> RandomDistribution;
    typedef boost::variate_generator<RandomEngine, RandomDistribution> RandomGenerator;
    RatioT total_ratio_;
    Actions actions_;
    Ratios ratios_;
    RandomDistribution distribution_;
    RandomGenerator generator_;

};