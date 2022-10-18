#pragma once
//=======================================================================================================================
/// DropItemRatioCalc class
/**
	@author
			Kim Min Wook < taiyo@webzen.com >	
	@since
			2004. 11. 30
	@remarks
			- 
	@note
			- 
	@history
			- 2005.12.22 : m_Key의 문제로 특정 아이템이 드랍안되는 문제 수정
			- 사용하지 않음!
*/
//=======================================================================================================================
#if(0)

#include <ItemOptionHeader.h>
#include <ReliableRatioRandomizer.h>
#include <vector>
#include <SolarHashTable.h>

class DropItemInfo
{
	friend class DropItemRatioCalc;
public:
	DropItemInfo():m_Code(0),m_Enchant(0),m_Rank((eRANK)0),m_Set(0){}
	DropItemInfo(SLOTCODE Code, BYTE Enchant, eRANK Rank, BYTE bySet)
		:m_Code(Code),m_Enchant(Enchant),m_Rank(Rank),m_Set(bySet){}
		~DropItemInfo(){}

		inline SLOTCODE	GetCode()		{ return m_Code; }
		inline BYTE		GetEnchant()	{ return m_Enchant; }
		inline eRANK	GetRank()		{ return m_Rank; }
		inline BYTE		GetSet()		{ return m_Set; }
private:
	SLOTCODE		m_Code;
	BYTE			m_Enchant;
	eRANK			m_Rank;
	BYTE			m_Set;
};

class DropItemRatioCalc
{
	class DropItemGradeRatioInfo
	{
	public:
		DropItemGradeRatioInfo():m_Key(0xff000000){}
		~DropItemGradeRatioInfo(){}
		VOID						Clear() 
		{ 
			m_EnchantValue=0;
			m_DivineValue=0;	
			m_Ratio=0;
			m_StandardRatio=0;
			for(int i=0;i<(MAX_LEVEL+1);++i) 
			{
				std::vector<DropItemInfo *>::iterator it;
				for(it=m_DropItemVector[i].begin();it!=m_DropItemVector[i].end();++it)
				{
					DropItemInfo * pd = *it;
					SAFE_DELETE(pd);
				}
				m_DropItemVector[i].clear();
			}
		}
		union 
		{
			struct{
			BYTE					m_EnchantValue;
			BYTE					m_DivineValue;
			};
			const DWORD				m_Key;
		};
		DWORD						m_Ratio;
		DWORD						m_StandardRatio;
		std::vector<DropItemInfo *>	m_DropItemVector[MAX_LEVEL+1];
	};
public:
	DropItemRatioCalc();
	~DropItemRatioCalc();

	// ItemDropPerGrade.txt 파일 로딩
	VOID				Init( TCHAR * tszFileName );
	VOID				Release();

	DropItemInfo *		PopRandomDropItemForLevel( LEVELTYPE LV );
	
	VOID				Reload();

private:
	VOID				_AnalysisDropItemInfo();
	VOID				_AddDropItem( DWORD key, LEVELTYPE DropLV, DropItemInfo * pItem );
	DropItemGradeRatioInfo * _GetDropItemGradeRatioInfo(DWORD key) { return m_WeightRatioInfoHash.GetData(key);	}
	VOID				_Load( TCHAR * tszFileName, BOOL bReload = FALSE );

	TCHAR				m_tszFileName[MAX_PATH];
	enum { MAX_ITEM_DROP_TYPE_NUM = 20, MAX_PLUS_ENCANT=4, MAX_PLUS_SET=1, MAX_PLUS_RANK=(BYTE)RANK_D, };
	util::SolarHashTable<DropItemGradeRatioInfo *>	m_WeightRatioInfoHash;
	ReliableRatioRandomizer<MAX_ITEM_DROP_TYPE_NUM,DWORD,DWORD> m_ItemGradeSelector;
};

#endif
