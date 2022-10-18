/*!
 * \file	ObjectFactory.h
 * \brief	타입에 따라 해당 오브젝트를 생성
 * \bug		
 * \todo	
 * \author	김경표 (lammy@webzen.co.kr)
 * \version	2008/12/30	김경표 - doxygen command 작성
 * \version 2005.06.07 소스 정리(지완)
 */

#ifndef	__OBJECT_FACTORY_H__
#define	__OBJECT_FACTORY_H__

#pragma once
class Object;

/*!
 * \class	ObjectFactory class
 * \brief	타입에 따라 해당 오브젝트를 생성
 * \author	김경표 (lammy@webzen.co.kr)
 */

class ObjectFactory
{
public:
	/// 기본 생성자
	ObjectFactory( void );
	/// 소멸자
	~ObjectFactory( void );

	/*!
		오브젝트를 생성하는 함수
		\param[in] objType	오브젝트 타입
		\param[in] subType	서브 타입 MAP_OBJECT 사용시 터레인과 맵오브젝트 구별용
		\return	   성공시 Object 포인터 리턴 
		\return	   실패시 NULL 리턴 
	 */
	Object* MakeNewObject( eOBJECT_TYPE objType, OBJECT_SUB_TYPE subType = OBJECT_SUB_TYPE_NONE );
};

#endif
