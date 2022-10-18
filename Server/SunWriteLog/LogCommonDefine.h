#pragma once

#include "LogCategoryCode.h"
#include <boost/mpl/if.hpp>
/************************************************************************/
/* ��α� ���� �⺻ ����                                                */
/************************************************************************/

#define __LOG_PROPERTY( DataType, name )											\
		public :																\
		void		Set##name( DataType value )		{ m_##name = value; }	\
		DataType	Get##name() const				{ return m_##name; }	\
		protected :																\
		DataType	m_##name;

#if 1 // code renewal

// to support log size, type 1byte and 2byte
#define ASSERT_CHECKED_LOG_SIZE_TYPE(c_size, data) \
    { \
        typedef boost::mpl::if_c<sizeof(c_size) == 1, boost::mpl::integral_c<int, (1 <<  8) - 1>, \
                boost::mpl::if_c<sizeof(c_size) == 2, boost::mpl::integral_c<int, (1 << 16) - 1>, \
                boost::mpl::integral_c<int, 0> > >::type _check_type_; \
        BOOST_STATIC_ASSERT(_check_type_::type::value >= sizeof(data)); \
    }

#define _CHECK_LOG_SIZE_AND_WRITE(c_size, data, Archive) \
    { \
        ASSERT_CHECKED_LOG_SIZE_TYPE(c_size, data); \
        c_size = sizeof(data); \
        (Archive) << c_size; \
        (Archive).Write(&(data), c_size); \
    }

#else // old code

#define _CHECK_LOG_SIZE_AND_WRITE(c_size, data, Archive)	if(pow(2, sizeof(c_size)*8) - 1 < sizeof(data))\
														{\
															ASSERT(FALSE);\
														}\
														else\
														{\
															c_size = sizeof(data);\
															Archive << c_size;\
															Archive.Write( &data, c_size );\
														}
#endif // old code

#define LOG_READ(size, data, Archive)	Archive >> size;\
										rArchive.Read( &data, size);

// ���� �α� �Ӽ� �׸��� �ִ� ����
#define LOG_PROP_MAX_LEN 255 // 1-byte char ����
// �ִ�� �ٷ� �� �ִ� ���� ������ �α� �Ӽ��� ��
#define MAX_DYNAMIC_LOG_PROPS 32 // 1-byte char, 32-bit integer ����

// ������ ������ �α� �Ӽ� ����
#define LOG_STATIC_PROP_BINARY(type, name) \
	public: \
		void Set##name(type val) { prop##name##_ = (type)val; } \
		type Get##name() const { return prop##name##_; } \
	protected: \
		type prop##name##_;

// ������ ������ �α� �Ӽ� ����
// ���� �Ӽ��� ��������� ������ ��쿡�� ���Ͽ��� ������ ������ ����Ѵ�
// index�� 0���� �����ϴ� ���� ������ �ߺ� ���� �����Ǿ�� �Ѵ�
#define LOG_PROP_BINARY(index, type, name) \
	public: \
		void Set##name(type val) \
		{ \
			ASSERT(index <= MAX_DYNAMIC_LOG_PROPS); \
			prop##name##_ = val; \
			filter_ |= (1 << index); \
		} \
		type Get##name() const { return prop##name##_; } \
		bool Is##name##Set() const { return ((filter_ & (1 << index)) != 0); } \
	protected: \
		type prop##name##_;

// ������ ���ڿ� �α� �Ӽ� ����
// ���ڿ��� ����(255 ����)�� �������� �����ǹǷ� index�� ���� �÷��� ������ �ʿ� ����
#define LOG_PROP_STRING(name, maxLen) \
	LOG_STATIC_PROP_BINARY(BYTE, name##Len) \
	public: \
		void Set##name(const TCHAR* val, size_t size) \
		{ \
			if (!val) { return; } \
			ASSERT(val != prop##name##_); \
			_tcsncpy(prop##name##_, val, min(size, maxLen)); \
			prop##name##_[maxLen] = '\0'; \
			Set##name##Len((BYTE)_tcslen(prop##name##_)); \
		} \
		const TCHAR* Get##name() const { return prop##name##_; } \
	protected: \
		TCHAR prop##name##_[maxLen + 1];

// ���� �α� �Ӽ��� �����ϴ� �α� ������ Ŭ���� ������
// MAX_DYNAMIC_LOG_PROPS���� �ʰ��ϴ� ���� �Ӽ��� �����ϱ� ���ؼ��� 
// ���� ������ Ÿ���� �����ؾ� �Ѵ�
#define LOG_DYNAMIC_PROP_CTOR(className, logCategory) \
	public: \
		className::className() \
		{ \
			SetCategory(logCategory); \
			filter_ = 0; \
		} \
	protected: \
		DWORD filter_;

// ������ �α� �Ӽ� �ʱ�ȭ
#define LOG_INIT_BINARY(name, val) \
	prop##name##_ = val;

// ���ڿ� �α� �Ӽ� �ʱ�ȭ
#define LOG_INIT_STRING(name) \
	prop##name##Len_ = 0; \
	prop##name##_[0] = '\0';

// ������ ������ �α� �Ӽ� ����
#define LOG_WRITE_STATIC_BINARY(name, archive) \
	{ \
		int size = (int)sizeof(prop##name##_); \
		ASSERT( size <= LOG_PROP_MAX_LEN ); \
		archive << (BYTE)size; \
		archive.Write(&prop##name##_, size); \
	}

// ������ ������ �α� �Ӽ� ����
#define LOG_WRITE_BINARY(name, archive) \
	if (Is##name##Set()) { \
		LOG_WRITE_STATIC_BINARY(name, archive) \
	} \
	else { \
		archive << (BYTE)0; \
	}

// ������ ���ڿ� �α� �Ӽ� ����
#define LOG_WRITE_STRING(name, archive) \
	{ \
		int size = Get##name##Len(); \
		ASSERT( size <= LOG_PROP_MAX_LEN ); \
		archive << (BYTE)size; \
		archive.Write(prop##name##_, size); \
	}

// ������ �α� �Ӽ� �б�
#define LOG_READ_BINARY(name, archive) \
	{ \
		BYTE size; \
		archive >> size; \
		if (size) { \
			archive.Read(&prop##name##_, size); \
		} \
	}

// ���ڿ� �α� �Ӽ� �б�
#define LOG_READ_STRING(name, archive) \
	archive >> prop##name##Len_; \
	if (prop##name##Len_) { \
		archive.Read(prop##name##_, prop##name##Len_); \
		prop##name##_[prop##name##Len_] = '\0'; \
	}

#define	BASEDATA_LAST_VERSION			1
#define ACTIONDATA_LAST_VERSION			1
#define ADMINDATA_LAST_VERSION			1
#define CHATDATA_LAST_VERSION			1
#define EXPDATA_LAST_VERSION			1
#define INVENTORYDATA_LAST_VERSION		1
#define ITEMDATA_LAST_VERSION			1
#define MONEYDATA_LAST_VERSION			1
#define SESSIONDATA_LAST_VERSION		1
#define SKILLDATA_LAST_VERSION			1

#define SNAPSHOTDATA_LAST_VERSION		3   // WARNING, ambiguous naming..., this relate a Quest

#define WAREHOUSEDATA_LAST_VERSION		1
#define GUILDDATA_LAST_VERSION						1

#ifdef SUNWRITELOGMNG_EXPORTS
	#define	DLL_DECLARE	__declspec(dllexport)
	#define	DLLEXPORTT_EMPLATE
#else
	#define	DLL_DECLARE	__declspec(dllimport)
	#define	DLLEXPORTT_EMPLATE extern
#endif