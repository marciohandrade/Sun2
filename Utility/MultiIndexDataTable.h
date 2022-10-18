#pragma once

#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <algorithm>
#include <functional>
using namespace boost::multi_index;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//. template for Complex Key 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	class KeyExtractor1,
	class KeyExtractor2
>
struct key_from_key
{
public:
	typedef typename KeyExtractor1::result_type result_type;

	key_from_key(
		const KeyExtractor1& key1_=KeyExtractor1(),
		const KeyExtractor2& key2_=KeyExtractor2()):
	key1(key1_),key2(key2_)
	{}

	template<typename Arg>
		result_type operator()(Arg& arg)const
	{
		return key1(key2(arg));
	}

private:
	KeyExtractor1 key1;
	KeyExtractor2 key2;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T, typename ElemKeyType>
class SingleLayerDataPool: protected boost::object_pool<T>
{
public:
	typedef STLX_LIST<T> DataClassList;

public:
	SingleLayerDataPool(){};
	~SingleLayerDataPool(){};

public:
	T* Create(ElemKeyType elemKey, const T& p_data)
	{        
		T* data = Get(elemKey);
		if(data)
		{
			*data = p_data;
			return data;
		}
		else
		{
			data = this->construct();
			if(data)
			{                
				*data = p_data;
				dataPool_.insert(std::make_pair(elemKey, data));
			}
			return data;
		}
	}

	bool Get(ElemKeyType elemKey, T& p_data)
	{
		T* data = Get(elemKey);
		if(data)
		{
			p_data = *data;
			return true;
		}
		return false;
	}

	bool GetList(DataClassList& copyList)
	{
		for(DataPoolIterator it = dataPool_.begin(); it != dataPool_.end(); ++it)
		{
			T* data = (*it).second;
			copyList.push_back(*data);
		}

		return true;
	}

	void Close(ElemKeyType elemKey)
	{
		T* data = Get(elemKey);
		if(data)
		{
			destroy(data);
			dataPool_.erase(elemKey);
		}
	}
private:
	T* Get(ElemKeyType elemKey)
	{
		DataPoolIterator it = dataPool_.find(elemKey);;
		if(it != dataPool_.end())
		{            
			return it->second;
		}
		return NULL;
	}

private:
	STLX_MAP<ElemKeyType, T*> dataPool_;
	typedef typename STLX_MAP<ElemKeyType, T*>::iterator DataPoolIterator;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	class DataClass,     
	typename ElemDetailKeyType, ElemDetailKeyType DataClass::*PtrToElemDetailKey,
	typename ElemSubKeyType, ElemSubKeyType DataClass::*PtrToElemSubKey
>
class GenUnique2KeyDataPool: protected boost::object_pool<DataClass>
{
public:
	typedef STLX_LIST<DataClass> DataClassList;

private:
	struct TagElemSubKey{};
	struct TagElemDetailKey{};    

	typedef multi_index_container<
		DataClass*, 
		indexed_by<
		ordered_unique<tag<TagElemSubKey>, 
			member<DataClass, ElemSubKeyType, PtrToElemSubKey> >,
		ordered_unique<tag<TagElemDetailKey>, 
			member<DataClass, ElemDetailKeyType, PtrToElemDetailKey> >
		>
	> DataClassSet;

	typedef typename boost::multi_index::index<DataClassSet, TagElemDetailKey>::type DataClassSet_ByElemDetailKey;        
	typedef typename boost::multi_index::index<DataClassSet, TagElemSubKey>::type DataClassSet_ByElemSubKey;

	typedef typename DataClassSet_ByElemDetailKey::iterator ByElemDetailKeyIterator;    
	typedef typename DataClassSet_ByElemSubKey::iterator ByElemSubKeyIterator;

public:
	GenUnique2KeyDataPool()
		: indexedListByElemSubKey_(get<TagElemSubKey>(dataClassSet_)),
		indexedListByElemDetailKey_(get<TagElemDetailKey>(dataClassSet_))
	{}
	~GenUnique2KeyDataPool(){}

	bool Create(const DataClass& p_dataClass, bool DoReplaceIfExist = true)
	{
		bool replaced = false;

		if(DoReplaceIfExist)
		{
			replaced = ReplaceIfExist(p_dataClass);
		}

		if(replaced == false)
		{
			DataClass* dataClass = this->construct();
			if(dataClass)
			{
				*dataClass = p_dataClass;
				dataClassSet_.insert(dataClass);
				return true;
			}
		}
		return false;
	}
	bool ReplaceIfExist(const DataClass& p_dataClass)
	{
		ElemDetailKeyType detailKey = p_dataClass.*PtrToElemDetailKey;
		ByElemDetailKeyIterator it = indexedListByElemDetailKey_.find(detailKey);
		if(it != indexedListByElemDetailKey_.end())
		{
			DataClass* dataClass = *it;            
			*dataClass = p_dataClass;
			dataClass->*PtrToElemDetailKey = detailKey;
			indexedListByElemDetailKey_.replace(it, dataClass);
			return true;
		}
		return false;
	}
	bool Get(ElemDetailKeyType elemDetailKey, DataClass& p_dataClass)
	{
		DataClass* dataClass = Get(elemDetailKey);
		if(dataClass)
		{
			p_dataClass = *dataClass;
			return true;
		}
		return false;
	}
	bool GetList(ElemSubKeyType elemSubKey, DataClassList& copyList)
	{
		pair<ByElemSubKeyIterator, ByElemSubKeyIterator> p = 
			indexedListByElemSubKey_.equal_range(elemSubKey);        
		for(ByElemSubKeyIterator it = p.first; it != p.second; ++it)
		{
			DataClass* dataClass = *it;
			copyList.push_back(*dataClass);
		}
		return (p.first != p.second);
	}
	bool HasList(ElemSubKeyType elemSubKey)
	{
		pair<ByElemSubKeyIterator, ByElemSubKeyIterator> p = 
			indexedListByElemSubKey_.equal_range(elemSubKey);
		return (p.first != p.second);
	}
	void CloseBunch(ElemSubKeyType elemSubKey)
	{
		pair<ByElemSubKeyIterator, ByElemSubKeyIterator> p = 
			indexedListByElemSubKey_.equal_range(elemSubKey);        
		if(p.first != p.second)
		{
			STLX_LIST<DataClass*> toBeDestroyed;
			for(ByElemSubKeyIterator it = p.first; it != p.second; ++it)
			{
				toBeDestroyed.push_back(*it);
			}
			indexedListByElemSubKey_.erase(p.first, p.second);
			
			typedef typename STLX_LIST<DataClass*>::iterator DataClassIterator;            
			for(DataClassIterator it2 = toBeDestroyed.begin(); 
				it2 != toBeDestroyed.end(); ++it2)
			{
				destroy(*it2);
			}
			//for_each(toBeDestroyed.begin(), toBeDestroyed.end(), boost::lambda::bind(&DoubleLayerDataPool::free, this, boost::lambda::_1));
		}        
	}
	size_t Count()
	{
		return indexedListByElemDetailKey_.size();
	}
	void Close(ElemDetailKeyType elemDetailKey)
	{
		ByElemDetailKeyIterator it = indexedListByElemDetailKey_.find(elemDetailKey);
		if(it != indexedListByElemDetailKey_.end())
		{
			DataClass* dataClass = *it;
			indexedListByElemDetailKey_.erase(it);
			destroy(dataClass);
		}
	}
private:
	DataClass* Get(ElemDetailKeyType elemDetailKey)
	{
		ByElemDetailKeyIterator it = indexedListByElemDetailKey_.find(elemDetailKey);
		if(it != indexedListByElemDetailKey_.end())
		{
			return *it;
		}
		return NULL;
	}

private:
	DataClassSet dataClassSet_;
	DataClassSet_ByElemSubKey& indexedListByElemSubKey_;
	DataClassSet_ByElemDetailKey& indexedListByElemDetailKey_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	class DataClass,     
	typename ElemGroupKeyType, ElemGroupKeyType DataClass::*PtrToElemGroupKey,
	typename ElemDetailKeyType, ElemDetailKeyType DataClass::*PtrToElemDetailKey,
	typename ElemKeyType, ElemKeyType DataClass::*PtrToElemKey
>
class DoubleLayerNonUniqueDataPool: protected boost::object_pool<DataClass>
{
public:
	typedef STLX_LIST<DataClass> DataClassList;

private:
	struct TagElemKey{};
	struct TagElemDetailKey{};    
	struct TagElemGroupKey{};

	typedef multi_index_container<
		DataClass*, 
		indexed_by<
		ordered_non_unique<tag<TagElemKey>, 
		member<DataClass, ElemKeyType, PtrToElemKey> >,
		ordered_non_unique<tag<TagElemDetailKey>, 
		member<DataClass, ElemDetailKeyType, PtrToElemDetailKey> >,
		ordered_non_unique<tag<TagElemGroupKey>, 
		member<DataClass, ElemGroupKeyType, PtrToElemGroupKey> >
		>
	> DataClassSet;

	typedef typename boost::multi_index::index<DataClassSet, TagElemKey>::type DataClassSet_ByElemKey;
	typedef typename boost::multi_index::index<DataClassSet, TagElemDetailKey>::type DataClassSet_ByElemDetailKey;        
	typedef typename boost::multi_index::index<DataClassSet, TagElemGroupKey>::type DataClassSet_ByElemGroupKey;

	typedef typename DataClassSet_ByElemKey::iterator ByElemKeyIterator;
	typedef typename DataClassSet_ByElemDetailKey::iterator ByElemDetailKeyIterator;    
	typedef typename DataClassSet_ByElemGroupKey::iterator ByElemGroupKeyIterator;    

public:
	static UINT64 MakeElementKey(ElemGroupKeyType elemGroupKey, ElemDetailKeyType elemDetailKey)
	{
		return ((static_cast<UINT64>(elemGroupKey)) << 32) + elemDetailKey;
	}

public:
	DoubleLayerNonUniqueDataPool()
		: indexedListByElemKey_(get<TagElemKey>(dataClassSet_)),
		indexedListByElemDetailKey_(get<TagElemDetailKey>(dataClassSet_)),
		indexedListByElemGroupKey_(get<TagElemGroupKey>(dataClassSet_))

	{}
	~DoubleLayerNonUniqueDataPool(){}

	bool Create(const DataClass& p_dataClass, bool DoReplaceIfExist = true)
	{
		bool replaced = false;

		if(DoReplaceIfExist)
		{
			replaced = ReplaceIfExist(p_dataClass);
		}

		if(replaced == false)
		{
			DataClass* dataClass = this->construct();
			if(dataClass)
			{
				UINT64 elemKey = MakeElementKey(p_dataClass.*PtrToElemGroupKey, 
					p_dataClass.*PtrToElemDetailKey);
				*dataClass = p_dataClass;
				dataClass->*PtrToElemKey = elemKey;
				dataClassSet_.insert(dataClass);
				return true;
			}
		}
		return false;
	}
	bool ReplaceIfExist(const DataClass& p_dataClass)
	{
		UINT64 elemKey = MakeElementKey(p_dataClass.*PtrToElemGroupKey, 
			p_dataClass.*PtrToElemDetailKey);
		ByElemKeyIterator it = indexedListByElemKey_.find(elemKey);
		if(it != indexedListByElemKey_.end())
		{
			DataClass* dataClass = *it;            
			*dataClass = p_dataClass;
			dataClass->*PtrToElemKey = elemKey;
			indexedListByElemKey_.replace(it, dataClass);
			return true;
		}
		return false;
	}
	bool Get(ElemGroupKeyType elemGroupKey, ElemDetailKeyType elemDetailKey, DataClass& p_dataClass)
	{
		DataClass* dataClass = Get(elemGroupKey, elemDetailKey);
		if(dataClass)
		{
			p_dataClass = *dataClass;
			return true;
		}
		return false;
	}
	bool GetList(ElemGroupKeyType elemGroupKey, DataClassList& copyList)
	{
		pair<ByElemGroupKeyIterator, ByElemGroupKeyIterator> p = 
			indexedListByElemGroupKey_.equal_range(elemGroupKey);        
		for(ByElemGroupKeyIterator it = p.first; it != p.second; ++it)
		{
			DataClass* dataClass = *it;
			copyList.push_back(*dataClass);
		}
		return (p.first != p.second);
	}
	bool GetDetailElemList(ElemDetailKeyType elemDetailKey, DataClassList& copyList)
	{
		pair<ByElemDetailKeyIterator, ByElemDetailKeyIterator> p = 
			indexedListByElemDetailKey_.equal_range(elemDetailKey);        
		for(ByElemDetailKeyIterator it = p.first; it != p.second; ++it)
		{
			DataClass* dataClass = *it;
			copyList.push_back(*dataClass);
		}
		return (p.first != p.second);
	}    
	bool HasList(ElemGroupKeyType elemGroupKey)
	{
		pair<ByElemGroupKeyIterator, ByElemGroupKeyIterator> p = 
			indexedListByElemGroupKey_.equal_range(elemGroupKey);        
		return (p.first != p.second);
	}
	void CloseBunch(ElemGroupKeyType elemGroupKey)
	{        
		pair<ByElemGroupKeyIterator, ByElemGroupKeyIterator> p = 
			indexedListByElemGroupKey_.equal_range(elemGroupKey);        
		if(p.first != p.second)
		{
			STLX_LIST<DataClass*> toBeDestroyed;
			for(ByElemGroupKeyIterator it = p.first; it != p.second; ++it)
			{
				toBeDestroyed.push_back(*it);
			}
			indexedListByElemGroupKey_.erase(p.first, p.second);
			typedef typename STLX_LIST<DataClass*>::iterator DataClassIterator;            
			for(DataClassIterator it2 = toBeDestroyed.begin(); 
				it2 != toBeDestroyed.end(); ++it2)
			{
				destroy(*it2);
			}
			//for_each(toBeDestroyed.begin(), toBeDestroyed.end(), boost::lambda::bind(&DoubleLayerNonUniqueDataPool::free, this, boost::lambda::_1));
		}
	}
	void Close(ElemGroupKeyType elemGroupKey, ElemDetailKeyType elemDetailKey)
	{
		UINT64 elemKey = MakeElementKey(elemGroupKey, elemDetailKey);
		ByElemKeyIterator it = indexedListByElemKey_.find(elemKey);
		if(it != indexedListByElemKey_.end())
		{
			DataClass* dataClass = *it;
			indexedListByElemKey_.erase(it);
			destroy(dataClass);
		}
	}

private:
	DataClass* Get(ElemGroupKeyType elemGroupKey, ElemDetailKeyType elemDetailKey)
	{
		UINT64 elemKey = MakeElementKey(elemGroupKey, elemDetailKey);
		ByElemKeyIterator it = indexedListByElemKey_.find(elemKey);
		if(it != indexedListByElemKey_.end())
		{
			return *it;
		}
		return NULL;
	}

private:
	DataClassSet dataClassSet_;
	DataClassSet_ByElemKey& indexedListByElemKey_;
	DataClassSet_ByElemDetailKey& indexedListByElemDetailKey_;    
	DataClassSet_ByElemGroupKey& indexedListByElemGroupKey_;    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multi_index wrapper template for Complex DataStruct
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	class DataClass,
	/* Member Class */
	typename MiddleKeyDataClass, MiddleKeyDataClass DataClass::*MiddleKeyData,
	typename RightKeyDataClass, RightKeyDataClass DataClass::*RightKeyData,
	typename LeftKeyDataClass, LeftKeyDataClass DataClass::*LeftKeyData,
	/* Member Value */
	typename ElemMiddleKeyType, ElemMiddleKeyType MiddleKeyDataClass::*PtrToElemMiddleKey,
	typename ElemRightKeyType, ElemRightKeyType RightKeyDataClass::*PtrToRightElemKey,
	typename ElemLeftKeyType, ElemLeftKeyType LeftKeyDataClass::*PtrToLeftElemKey
>
class GenComplexUnique3KeyDataPool : protected boost::object_pool<DataClass>
{
public:
	static struct TagFindByMiddleKey{};
	static struct TagFindByRightKey{};
	static struct TagFindByLeftKey{};
	typedef STLX_LIST<DataClass> DataClassList;

private:
	struct TagElemMiddleKey{};
	struct TagElemRightKey{};
	struct TagElemLeftKey{};

	typedef multi_index_container<
		DataClass*,
		indexed_by<
			ordered_unique<tag<TagElemMiddleKey>,
				key_from_key<member<MiddleKeyDataClass, ElemMiddleKeyType, PtrToElemMiddleKey>,
				member<DataClass, MiddleKeyDataClass, MiddleKeyData> > >,
			ordered_unique<tag<TagElemRightKey>,
				key_from_key<member<RightKeyDataClass, ElemRightKeyType, PtrToRightElemKey>,
				member<DataClass, RightKeyDataClass, RightKeyData> > >,
			ordered_unique<tag<TagElemLeftKey>,
				key_from_key<member<LeftKeyDataClass, ElemLeftKeyType, PtrToLeftElemKey>,
				member<DataClass, LeftKeyDataClass, LeftKeyData> > >
		>
	> DataClassSet;

	typedef typename boost::multi_index::index<DataClassSet, TagElemMiddleKey>::type DataClassSet_ByElemMiddleKey;
	typedef typename boost::multi_index::index<DataClassSet, TagElemRightKey>::type DataClassSet_ByElemRightKey;
	typedef typename boost::multi_index::index<DataClassSet, TagElemLeftKey>::type DataClassSet_ByElemLeftKey;

	typedef typename DataClassSet_ByElemMiddleKey::iterator ByElemMidlleKeyIterator;
	typedef typename DataClassSet_ByElemRightKey::iterator ByElemRightKeyIterator;
	typedef typename DataClassSet_ByElemLeftKey::iterator ByElemLeftKeyIterator;

public:
	GenComplexUnique3KeyDataPool()
		: indexedListByElemMiddleKey_(get<TagElemMiddleKey>(dataClassSet_)),
		indexedListByElemRightKey_(get<TagElemRightKey>(dataClassSet_)),
		indexedListByElemLeftKey_(get<TagElemLeftKey>(dataClassSet_))
	{};
	~GenComplexUnique3KeyDataPool(){};

	DataClass* Create(const DataClass& p_dataClass, bool DoReplaceIfExist = true)
	{
		bool replaced = false;
		if(DoReplaceIfExist)
		{
			replaced = ReplaceIfExist(p_dataClass);
		}

		if(replaced == false)
		{
			DataClass* dataClass = this->construct();
			if(dataClass)
			{
				*dataClass = p_dataClass;
				dataClassSet_.insert(dataClass);
				return dataClass;
			}
		}
		return NULL;
	}
	bool ReplaceIfExist(const DataClass& p_dataClass)
	{               
		ElemMiddleKeyType elemKey = p_dataClass.*MiddleKeyData.*PtrToElemMiddleKey;
		ByElemMidlleKeyIterator it = indexedListByElemMiddleKey_.find(elemKey);
		if(it != indexedListByElemMiddleKey_.end())
		{
			DataClass* dataClass = *it;
			*dataClass = p_dataClass;
			indexedListByElemMiddleKey_.replace(it, dataClass);
			return true;
		}
		return false;    
	}
	bool GetListByKey(ElemMiddleKeyType elemGroupKey, DataClassList& copyList)
	{
		pair<ByElemMidlleKeyIterator, ByElemMidlleKeyIterator> p = 
			indexedListByElemGroupKey_.equal_range(elemGroupKey);        
		for(ByElemMidlleKeyIterator it = p.first; it != p.second; ++it)
		{
			DataClass* dataClass = *it;
			copyList.push_back(*dataClass);
		}
		return (p.first != p.second);
	}
	template<class ElemType, class FindKeyType> 
	bool Get(ElemType elemKey, DataClass& p_dataClass, FindKeyType)
	{
		DataClass* dataClass = Get(elemKey, FindKeyType());
		if(dataClass)
		{
			p_dataClass = *dataClass;
			return true;
		}
		return false;
	}
	bool GetList(DataClassList& copyList)
	{
		for(ByElemMidlleKeyIterator it = 
			indexedListByElemMiddleKey_.begin(); 
			it != indexedListByElemMiddleKey_.end(); ++it)
		{
			DataClass* dataClass = *it;
			copyList.push_back(*dataClass);
		}
		return (indexedListByElemMiddleKey_.begin() != indexedListByElemMiddleKey_.end());
	}
	bool HasList(ElemMiddleKeyType elemGroupKey)
	{
		pair<ByElemMidlleKeyIterator, ByElemMidlleKeyIterator> p = 
			indexedListByElemMiddleKey_.equal_range(elemGroupKey);
		return (p.first != p.second);
	}
	bool Close(ElemMiddleKeyType elemKey)
	{
		ByElemMidlleKeyIterator it = indexedListByElemMiddleKey_.find(elemKey);
		if(it != indexedListByElemMiddleKey_.end())
		{
			DataClass* dataClass = *it;
			indexedListByElemMiddleKey_.erase(it);
			destroy(dataClass);
			return true;
		}

		return false;
	}

private:
	DataClass* Get(ElemMiddleKeyType elemKey, TagFindByMiddleKey)
	{
		ByElemMidlleKeyIterator it = indexedListByElemMiddleKey_.find(elemKey);
		if(it != indexedListByElemMiddleKey_.end())
		{
			return *it;
		}
		return NULL;
	}
	DataClass* Get(ElemRightKeyType elemKey, TagFindByRightKey)
	{
		ByElemRightKeyIterator it = indexedListByElemRightKey_.find(elemKey);
		if(it != indexedListByElemRightKey_.end())
		{
			return *it;
		}
		return NULL;
	}
	DataClass* Get(ElemLeftKeyType elemKey, TagFindByLeftKey)
	{
		ByElemLeftKeyIterator it = indexedListByElemLeftKey_.find(elemKey);
		if(it != indexedListByElemLeftKey_.end())
		{
			return *it;
		}
		return NULL;
	}
private:
	DataClassSet dataClassSet_;
	DataClassSet_ByElemMiddleKey& indexedListByElemMiddleKey_;
	DataClassSet_ByElemRightKey& indexedListByElemRightKey_;
	DataClassSet_ByElemLeftKey& indexedListByElemLeftKey_;  
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multi_index wrapper template for Complex DataStruct - 3 key all unique
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	class DataClass,
		/* Member Class */
		typename MiddleKeyDataClass, MiddleKeyDataClass DataClass::*MiddleKeyData,
		typename RightKeyDataClass, RightKeyDataClass DataClass::*RightKeyData,
		typename LeftKeyDataClass, LeftKeyDataClass DataClass::*LeftKeyData,
		/* Member Value */
		typename ElemMiddleKeyType, ElemMiddleKeyType MiddleKeyDataClass::*PtrToElemMiddleKey,
		typename ElemRightKeyType, ElemRightKeyType RightKeyDataClass::*PtrToRightElemKey,
		typename ElemLeftKeyType, ElemLeftKeyType LeftKeyDataClass::*PtrToLeftElemKey
>
class ComplexUniqueIndex3KeyTable : protected boost::object_pool<DataClass>
{

protected:
	struct TagElemMiddleKey{};
	struct TagElemRightKey{};
	struct TagElemLeftKey{};

	typedef multi_index_container<
		DataClass*,
		indexed_by<
		ordered_unique<tag<TagElemMiddleKey>,
			key_from_key<member<MiddleKeyDataClass, ElemMiddleKeyType, PtrToElemMiddleKey>,
			member<DataClass, MiddleKeyDataClass, MiddleKeyData> > >,
		ordered_unique<tag<TagElemRightKey>,
			key_from_key<member<RightKeyDataClass, ElemRightKeyType, PtrToRightElemKey>,
			member<DataClass, RightKeyDataClass, RightKeyData> > >,
		ordered_unique<tag<TagElemLeftKey>,
			key_from_key<member<LeftKeyDataClass, ElemLeftKeyType, PtrToLeftElemKey>,
			member<DataClass, LeftKeyDataClass, LeftKeyData> > >
		>
	> DataClassSet;

public:
	ComplexUniqueIndex3KeyTable() {};
	~ComplexUniqueIndex3KeyTable(){}

protected:
	typedef typename boost::multi_index::index<DataClassSet, TagElemMiddleKey>::type DataClassSet_ByElemMiddleKey;
	typedef typename boost::multi_index::index<DataClassSet, TagElemRightKey>::type DataClassSet_ByElemRightKey;
	typedef typename boost::multi_index::index<DataClassSet, TagElemLeftKey>::type DataClassSet_ByElemLeftKey;

	typedef typename DataClassSet_ByElemMiddleKey::iterator ByElemMidlleKeyIterator;
	typedef typename DataClassSet_ByElemRightKey::iterator ByElemRightKeyIterator;
	typedef typename DataClassSet_ByElemLeftKey::iterator ByElemLeftKeyIterator;

protected:
	typedef STLX_LIST<DataClass> DataClassList;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Multi_index wrapper template for Complex DataStruct - 2 key  unique or 1 key non unique
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template
<
	class DataClass,
		/* Member Class */
		typename FirstKeyDataClass, FirstKeyDataClass DataClass::*FirstKeyData,
		typename SecondKeyDataClass, SecondKeyDataClass DataClass::*SecondKeyData,
		typename ThirdKeyDataClass, ThirdKeyDataClass DataClass::*ThirdKeyData,
		/* Member Value */
		typename ElemFirstKeyType, ElemFirstKeyType FirstKeyDataClass::*PtrToElemFirstKey,
		typename ElemSecondKeyType, ElemSecondKeyType SecondKeyDataClass::*PtrToSecondElemKey,
		typename ElemThirdKeyType, ElemThirdKeyType ThirdKeyDataClass::*PtrToThirdElemKey
>
class ComplexIndex3KeyTable : protected boost::object_pool<DataClass>
{

protected:
	struct TagElemFirstKey{};
	struct TagElemSecondKey{};
	struct TagElemThirdKey{};

	typedef multi_index_container<
		DataClass*,
		indexed_by<
		ordered_unique<tag<TagElemFirstKey>,
			key_from_key<member<FirstKeyDataClass, ElemFirstKeyType, PtrToElemFirstKey>,
			member<DataClass, FirstKeyDataClass, FirstKeyData> > >,
		ordered_non_unique<tag<TagElemSecondKey>,
			key_from_key<member<SecondKeyDataClass, ElemSecondKeyType, PtrToSecondElemKey>,
			member<DataClass, SecondKeyDataClass, SecondKeyData> > >,
		ordered_non_unique<tag<TagElemThirdKey>,
			key_from_key<member<ThirdKeyDataClass, ElemThirdKeyType, PtrToThirdElemKey>,
			member<DataClass, ThirdKeyDataClass, ThirdKeyData> > >
		>
	> DataClassSet;

public:
	ComplexIndex3KeyTable() {};
	~ComplexIndex3KeyTable(){}

protected:
	typedef typename boost::multi_index::index<DataClassSet, TagElemFirstKey>::type DataClassSet_ByElemFirstKey;
	typedef typename boost::multi_index::index<DataClassSet, TagElemSecondKey>::type DataClassSet_ByElemSecondKey;
	typedef typename boost::multi_index::index<DataClassSet, TagElemThirdKey>::type DataClassSet_ByElemThirdKey;

	typedef typename DataClassSet_ByElemFirstKey::iterator ByElemFirstKeyIterator;
	typedef typename DataClassSet_ByElemSecondKey::iterator ByElemSecondKeyIterator;
	typedef typename DataClassSet_ByElemThirdKey::iterator ByElemThirdKeyIterator;

protected:
	typedef STLX_LIST<DataClass> DataClassList;
};
#endif //#ifdef _NA_0_20080612_WORLDSERVER_REFACTORING