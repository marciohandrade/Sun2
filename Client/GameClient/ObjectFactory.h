/*!
 * \file	ObjectFactory.h
 * \brief	Ÿ�Կ� ���� �ش� ������Ʈ�� ����
 * \bug		
 * \todo	
 * \author	���ǥ (lammy@webzen.co.kr)
 * \version	2008/12/30	���ǥ - doxygen command �ۼ�
 * \version 2005.06.07 �ҽ� ����(����)
 */

#ifndef	__OBJECT_FACTORY_H__
#define	__OBJECT_FACTORY_H__

#pragma once
class Object;

/*!
 * \class	ObjectFactory class
 * \brief	Ÿ�Կ� ���� �ش� ������Ʈ�� ����
 * \author	���ǥ (lammy@webzen.co.kr)
 */

class ObjectFactory
{
public:
	/// �⺻ ������
	ObjectFactory( void );
	/// �Ҹ���
	~ObjectFactory( void );

	/*!
		������Ʈ�� �����ϴ� �Լ�
		\param[in] objType	������Ʈ Ÿ��
		\param[in] subType	���� Ÿ�� MAP_OBJECT ���� �ͷ��ΰ� �ʿ�����Ʈ ������
		\return	   ������ Object ������ ���� 
		\return	   ���н� NULL ���� 
	 */
	Object* MakeNewObject( eOBJECT_TYPE objType, OBJECT_SUB_TYPE subType = OBJECT_SUB_TYPE_NONE );
};

#endif
