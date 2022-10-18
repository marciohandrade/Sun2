#ifndef STL2STRING_H
	#define STL2STRING_H

#include "STLXMLParser.h"

namespace markup
{

template <class T>
class CSTL2String
{
public:
	static void ToString(const std::vector<T>& v, std::string& str);
	static void ToString(const std::valarray<T>& v, std::string& str);
	static void ToString(const std::list<T>& v, std::string& str);

	static void ToSTL(LPCTSTR str, std::vector<T>& v);
	static void ToSTL(LPCTSTR str, std::valarray<T>& v);
	static void ToSTL(LPCTSTR str, std::list<T>& v);

	static void ByteToSTL(LPCTSTR str, std::vector<BYTE>& v);
	static void ByteToSTL(LPCTSTR str, std::valarray<BYTE>& v);
	static void ByteToSTL(LPCTSTR str, std::list<BYTE>& v);
};


template<class T>
void CSTL2String<T>::ByteToSTL(LPCTSTR str, std::vector<BYTE>& v)
{
	std::_tstring stdString=str;
	std::_tistringstream in(stdString);
	std::list<BYTE> lVi;
	BYTE vi;
	short s;

	// reading data...
	while ( !in.eof() && in.good() )
	{
		in>>s;
		vi=(BYTE)s;

		if (in.good())
			lVi.push_back(vi);
	}

	// ouputting to vector
	v.resize(lVi.size());
	for (UINT i =0;i<v.size();i++)
	{
		ASSERT(!lVi.empty());
		v[i]=lVi.front();
		lVi.pop_front();
	}
	ASSERT(lVi.empty());
};

template<class T>
void CSTL2String<T>::ByteToSTL(LPCTSTR str, std::valarray<BYTE>& v)
{
	std::_tstring stdString=str;
	std::_tistringstream in(stdString);
	std::list<BYTE> lVi;
	BYTE vi;
	short s;

	// reading data...
	while ( !in.eof() && in.good() )
	{
		in>>s;
		vi=(BYTE)s;

		if (in.good())
			lVi.push_back(vi);
	}

	// ouputting to vector
	v.resize(lVi.size());
	for (UINT i =0;i<v.size();i++)
	{
		ASSERT(!lVi.empty());
		v[i]=lVi.front();
		lVi.pop_front();
	}
	ASSERT(lVi.empty());
};

template<class T>
void CSTL2String<T>::ByteToSTL(LPCTSTR str, std::list<BYTE>& _l)
{
	std::_tstring stdString=str;
	std::_tistringstream in(stdString);
	BYTE vi;
	short s;

	_l.clear();
	// reading data...
	while ( !in.eof() && in.good() )
	{
		in>>s;
		
		vi=(BYTE)s;

		if (in.good())
			_l.push_back(vi);
	}
};

template<class T>
void CSTL2String<T>::ToSTL(LPCTSTR str, std::vector<T>& v)
{
	std::_tstring stdString=str;
	std::_tistringstream in(stdString);
	std::list<T> lVi;
	T vi;

	// reading data...
	while ( !in.eof() && in.good() )
	{
		in>>vi;

		if (in.good())
			lVi.push_back(vi);
	}

	// ouputting to vector
	v.resize(lVi.size());
	for (UINT i =0;i<v.size();i++)
	{
		ASSERT(!lVi.empty());
		v[i]=lVi.front();
		lVi.pop_front();
	}
	ASSERT(lVi.empty());
};

template<class T>
void CSTL2String<T>::ToSTL(LPCTSTR str, std::valarray<T>& v)
{
	std::_tstring stdString=str;
	std::_tistringstream in(stdString);
	std::list<T> lVi;
	T vi;

	// reading data...
	while ( !in.eof() && in.good() )
	{
		in>>vi;

		if (in.good())
			lVi.push_back(vi);
	}

	// ouputting to vector
	v.resize(lVi.size());
	for (UINT i =0;i<v.size();i++)
	{
		ASSERT(!lVi.empty());
		v[i]=lVi.front();
		lVi.pop_front();
	}
	ASSERT(lVi.empty());
};

template<class T>
void CSTL2String<T>::ToSTL(LPCTSTR str, std::list<T>& _l)
{
	std::_tstring stdString=str;
	std::_tistringstream in(stdString);
	T vi;

	_l.clear();
	// reading data...
	while ( !in.eof() && in.good() )
	{
		in>>vi;

		if (in.good())
			_l.push_back(vi);
	}
};

template<class T>
void CSTL2String<T>::ToString(const std::valarray<T>& v, std::string& str)
{
	std::_tostringstream out;

	for (UINT i = 0;i<v.size();i++)
	{
		out<<v[i]<<_T(" ");
		if ((i+1)%100==0)
			out<<std::endl;
	}

	str=out.str().c_str();
} ;

template<class T> 
void CSTL2String<T>::ToString(const std::vector<T>& v, std::string& str)
{
	std::_tostringstream out;

	for (UINT i = 0;i<v.size();i++)
	{
		out<<v[i]<<_T(" ");
		if ((i+1)%100==0)
			out<<std::endl;
	}

	str=out.str().c_str();
};

template<class T>
void CSTL2String<T>::ToString(const std::list<T>& _l, std::string& str)
{
	std::_tostringstream out;
	int j;
	std::list<T>::const_iterator i;
	for (i = _l.begin(),j=0;i!=_l.end();i++,j++)
	{
		out<<*i<<_T(" ");
		if ((j+1)%100==0)
			out<<std::endl;
	}

	str=out.str().c_str();
};

};

#endif