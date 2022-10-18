#pragma once

#include <Singleton.h>
#include <functional_custom.h>
#include "SSQInfoStruct.h"

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SSQInfoParser>
//
class SSQInfoParser : public util::Singleton<SSQInfoParser>, public IParser
{
private:	__DISABLE_COPY(SSQInfoParser);
private:
	//─────────────────────────────────────────
	//	<TYPE DEFINTIONs>
	template< eSCRIPT_CODE _TAG > struct ssq_tag {};

	template< eSCRIPT_CODE _TAG, typename _SSQ_NODET >
	struct ssq_info : sun_type_info<_SSQ_NODET>
	{
		static const eSCRIPT_CODE						value = _TAG;
		typedef ssq_tag<value>							category;
		typedef _SSQ_NODET								type;
		typedef ssq_info								type_info;
	};

	typedef ssq_info<SCRIPT_SSQINFO, SSQInfo>			SSQInfo_Info;
	typedef ssq_info<SCRIPT_SSQPATTERN, SSQPattern>		SSQPattern_Info;
public:
	typedef STLX_MAP<MAPCODE, SSQInfo*>					SSQINFO_TABLE;	// static dungeon은 하나의 인스턴스만 존재할 수 있다.
	typedef STLX_MAP<SSQPattern::INDEX_T, SSQPattern*>	SSQPATTERN_TABLE;
	typedef STLX_MAP<INT, _AREA_TILE_INFO>				SSQTILEINFO_TABLE;

	//─────────────────────────────────────────
	//	<METHODs>
public:
    SSQInfoParser();
    ~SSQInfoParser();

public:
    VOID    Init(const TCHAR* pszPackFileName);
    VOID    Release();
public:
    virtual BOOL    LoadScript(eSCRIPT_CODE scriptCode, BOOL bReload);
private:
    BOOLEAN _Load(const SSQInfo_Info::category&, BOOLEAN bReload);
    BOOLEAN _Load(const SSQPattern_Info::category&, BOOLEAN bReload);
			
#ifdef _SERVER	// 일단 클라는 따로 로드중이다. 필요하다면 통일하고, 일단은 서버만 작업한다.
public:
    BOOLEAN _LoadWat(const TCHAR* pszWatFilePath, const BOOLEAN isPack);
#endif

private:
    BOOLEAN _Validate(SSQInfo* const pSSQInfo);
    BOOLEAN __ReadHuntingPoint(const DWORD nRow, TCHAR INOUT string_buffer[32],
                               SSQInfo::HUNTING_POINT_NODE OUT pNODE[eCHAR_TYPE_MAX]);
    BOOLEAN __ReadWinnerRewardInfo(const DWORD nRow, TCHAR INOUT string_buffer[32],
                                   SSQInfo::CHAR_REWARD_NODE& OUT rNODE);

//─────────────────────────────────────────
//	<INTERFACEs>
public:
    inline const SSQINFO_TABLE&     GetRefSSQInfo() const { return m_SSQInfoTable; }
    inline const SSQPATTERN_TABLE&  GetRefSSQPattern() const { return m_SSQPatternTable; }
#ifdef _SERVER
    inline const SSQTILEINFO_TABLE& GetRefSSQPatternInfo() const { return m_SSQTileInfoTable; }
    BOOLEAN GM_SpecialControl_OpenStart(const MAPCODE CodeOfSSQ, const SSQInfo::WzTmFmt retape,
                                        BOOLEAN isFastTrack);
#endif
    inline const SSQInfo* const     FindSSQInfo(const MAPCODE CodeOfSSQ) const;
    inline const SSQPattern* const  FindSSQPattern(const SSQPattern::INDEX_T key) const;
public:
    const SSQInfo* const            FindSSQInfoEnterableLevel(const LEVELTYPE level) const;

public:
//	template< typename _OPR >
//		VOID		ForeachSSQInfo(const _OPR& opr);
//	template< typename _FUNC, typename _OBJ >
//		VOID		ForeachSSQInfoBin(const _FUNC& _func, _OBJ& _obj);
//	template< typename _OPR >
//		VOID		ForeachSSQPattern(const _OPR& opr);
//	template< typename _FUNC, typename _OBJ >
//		VOID		ForeachSSQPatternBin(const _FUNC& _func, _OBJ& _obj);
//
//	template< typename _OPR >
//		SSQInfo*	ForeachFindSSQInfo(const _OPR& opr);
//	template< typename _FUNC, typename _OBJ >
//		SSQInfo*	ForeachFindSSQInfoBin(const _FUNC& _func, _OBJ& _obj);
//
//#ifdef _SERVER
//	template< typename _OPR >
//		VOID		ForeachSSQTiles(const _OPR& opr, const INT iSSQTileID);
//	template< typename _FUNC, typename _OBJ >
//		VOID		ForeachSSQTilesBin(const _FUNC& _func, _OBJ& _obj, const INT iSSQTileID);
//	template< typename _OPR >
//		VOID		ForeachSSQPatternStepTiles(const _OPR& opr, const SSQPattern::INDEX_T idx, const SSQPattern::STEP_T step);
//	template< typename _FUNC, typename _OBJ >
//		VOID		ForeachSSQPatternStepTilesBin(const _FUNC& _func, _OBJ& _obj, const SSQPattern::INDEX_T idx, const SSQPattern::STEP_T step);
//#endif


//─────────────────────────────────────────
//	<FIELDs>
private:
    SSQINFO_TABLE           m_SSQInfoTable;
    SSQPATTERN_TABLE        m_SSQPatternTable;
#ifdef _SERVER
#pragma warning(push)
#pragma warning(disable : 4201)
	struct {	// SSQPattern ID 에 매칭되는 타일 정보 관리
	    SSQTILEINFO_TABLE   m_SSQTileInfoTable;
	    INT*                m_pSSQTiles;
	};
#pragma warning(pop)
#endif
    friend class EventManager;	// for TimeControl
};

//─────────────────────────────────────────
//

inline const SSQInfo* const
SSQInfoParser::FindSSQInfo(const MAPCODE CodeOfSSQ) const
{
    SSQINFO_TABLE::const_iterator itFind = m_SSQInfoTable.find(CodeOfSSQ);
    return itFind != m_SSQInfoTable.end() ? itFind->second : NULL;
}

inline const SSQPattern* const
SSQInfoParser::FindSSQPattern(const SSQPattern::INDEX_T key) const
{
    SSQPATTERN_TABLE::const_iterator itFind = m_SSQPatternTable.find(key);
    return itFind != m_SSQPatternTable.end() ? itFind->second : NULL;
}

//template< typename _OPR >
//VOID	SSQInfoParser::ForeachSSQInfo(const _OPR& opr)
//{
//	Foreach_Default(m_SSQInfoTable, m_SSQInfoTable.begin(), m_SSQInfoTable.end()
//		, boost::bind(&_OPR::operator()
//			, &opr, boost::bind(select2nd<SSQINFO_PAIR>(), _1)));
//}
//
//template< typename _FUNC, typename _OBJ >
//VOID	SSQInfoParser::ForeachSSQInfoBin(const _FUNC& _func, _OBJ& _obj)
//{
//	Foreach_Default(m_SSQInfoTable, m_SSQInfoTable.begin(), m_SSQInfoTable.end()
//		, boost::bind(_func, &_obj, _1));
//}
//
//template< typename _OPR >
//VOID	SSQInfoParser::ForeachSSQPattern(const _OPR& opr)
//{
//	Foreach_Default(m_SSQPatternTable, m_SSQPatternTable.begin(), m_SSQPatternTable.end()
//		, boost::bind(&_OPR::operator()
//		, &opr, boost::bind(select2nd<SSQINFO_PAIR>(), _1)));
//}
//
//template< typename _FUNC, typename _OBJ >
//VOID	SSQInfoParser::ForeachSSQPatternBin(const _FUNC& _func, _OBJ& _obj)
//{
//	Foreach_Default(m_SSQPatternTable, m_SSQPatternTable.begin(), m_SSQPatternTable.end()
//		, boost::bind(_func, &_obj, _1));
//}
//
//template< typename _OPR >
//SSQInfo*	SSQInfoParser::ForeachFindSSQInfo(const _OPR& opr)
//{
//	return std::find_if(m_SSQInfoTable.begin(), m_SSQInfoTable.end()
//		, boost::bind(&_OPR::operator()
//		, &opr, boost::bind(select2nd<SSQINFO_PAIR>(), _1)));
//}
//
//template< typename _FUNC, typename _OBJ >
//SSQInfo*	SSQInfoParser::ForeachFindSSQInfoBin(const _FUNC& _func, _OBJ& _obj)
//{
//	return std::find_if(m_SSQInfoTable.begin(), m_SSQInfoTable.end()
//		, boost::bind(_func, &_obj, _1));
//}

#ifdef _SERVER
//template< typename _OPR >
//VOID		SSQInfoParser::ForeachSSQTiles(const _OPR& opr, const INT iSSQAreaID)
//{
//	SSQTILEINFO_TABLE_IT itFind = m_SSQTileInfoTable.find(iSSQAreaID);
//	if(itFind != m_SSQTileInfoTable.end())
//	{
//		_AREA_TILE_INFO& rNode = itFind->second;
//		std::for_each(rNode.piTileIndex, rNode.piTileIndex+rNode.wNumberOfTiles
//			, boost::bind(&_OPR::operator(), &opr, _1));
//	}
//}
//
//template< typename _FUNC, typename _OBJ >
//VOID		SSQInfoParser::ForeachSSQTilesBin(const _FUNC& _func, _OBJ& _obj, const INT iSSQAreaID)
//{
//	SSQTILEINFO_TABLE_IT itFind = m_SSQTileInfoTable.find(iSSQAreaID);
//	if(itFind != m_SSQTileInfoTable.end())
//	{
//		_AREA_TILE_INFO& rNode = itFind->second;
//		std::for_each(rNode.piTileIndex, rNode.piTileIndex+rNode.wNumberOfTiles
//			, boost::bind(_func, &_obj, _1));
//	}
//}
//
//template< typename _OPR >
//VOID	SSQInfoParser::ForeachSSQPatternStepTiles(const _OPR& opr, const SSQPattern::INDEX_T idx, const SSQPattern::STEP_T step)
//{
//	if(!(SSQPattern::MI_STEP_MAX > step))
//		return;
//
//	SSQPATTERN_TABLE_IT itFind = m_SSQPatternTable.find(id);
//	if(itFind != m_SSQPatternTable.end())
//	{
//		SSQPattern* const pSSQPattern = itFind->second;
//		SSQPattern::OBJECT_ID_ARRAY& rArray = pSSQPattern->m_StepGroup[step].m_List;
//		SSQPattern::OBJECT_ID_T *pIt=&rArray[0], *pEnd=&rArray[SSQPattern::MI_OBJECT_COUNT_MAX];
//		for(; pIt!=pEnd ; ++pIt)
//			if(*pIt != 0)
//				opr(*pIt);
//	}
//}
//
//template< typename _FUNC, typename _OBJ >
//VOID	SSQInfoParser::ForeachSSQPatternStepTilesBin(const _FUNC& _func, _OBJ& _obj, const SSQPattern::INDEX_T idx, const SSQPattern::STEP_T step)
//{
//	if(!(SSQPattern::MI_STEP_MAX > step))
//		return;
//
//	SSQPATTERN_TABLE_IT itFind = m_SSQPatternTable.find(idx);
//	if(itFind != m_SSQPatternTable.end())
//	{
//		SSQPattern* const pSSQPattern = itFind->second;
//		SSQPattern::OBJECT_ID_ARRAY& rArray = pSSQPattern->m_StepGroup[step].m_List;
//		SSQPattern::OBJECT_ID_T *pIt=&rArray[0], *pEnd=&rArray[SSQPattern::MI_OBJECT_COUNT_MAX];
//		for(; pIt!=pEnd ; ++pIt)
//			if(*pIt != 0)
//				boost::bind(_func, &_obj, _1)(*pIt);
//	}
//}
#endif

//////////////////////////////////////////////////////////////////////////
//	<Sample>
//struct OpSSQInfoRegister
//{
//	typedef SSQInfoParser::SSQINFO_PAIR		SSQINFO_PAIR;
//	VOID		Process(const SSQINFO_PAIR& rPair) const 
//	{
//	}
//
//	VOID		operator() (const SSQInfo* pSSQInfo) const
//	{
//	}
//};
//////////////////////////////////////////////////////////////////////////
//	<Sample><Implementation>
//	SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
//	OpSSQInfoRegister opr;
//	pSSQInfoParser->ForeachSSQInfoBin(&OpSSQInfoRegister::Process, opr);
//	--or--
//	pSSQInfoParser->ForeachSSQInfo(OpSSQInfoRegister());
//////////////////////////////////////////////////////////////////////////


