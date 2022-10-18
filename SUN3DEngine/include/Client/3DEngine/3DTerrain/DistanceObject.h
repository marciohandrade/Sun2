// distanceobject.h

#ifndef _3DTERRAIN_DISTANCEOBJECT_H_
#define _3DTERRAIN_DISTANCEOBJECT_H_

template <class T>
class tDistanceObject
{
	//------------------------------------------------
	// Interface
	//
public:
	tDistanceObject()
	{
		m_fDistance = 0.f;
	}
	tDistanceObject(T objectArg)
	{
		m_fDistance = 0.f;
		m_Object = objectArg;
	}
	tDistanceObject(const tDistanceObject &copy)
	{
		operator=(copy);
	}
	tDistanceObject &operator =(const tDistanceObject &copy)
	{
		if(this != &copy)
		{
			m_fDistance = copy.m_fDistance;
			m_Object = copy.m_Object;
		}
		return *this;
	}

	bool operator ==(const tDistanceObject &copy)
	{
		return (m_fDistance == copy.m_fDistace);
	}
	bool operator >=(const tDistanceObject &copy)
	{
		return (m_fDistance >= copy.m_fDistace);
	}

	bool operator <=(const tDistanceObject &copy)
	{
		return (m_fDistance <= copy.m_fDistance);
	}

	bool operator >(const tDistanceObject &copy)
	{
		return (m_fDistance > copy.m_fDistance);
	}
	bool operator <(const tDistanceObject &copy)
	{
		return (m_fDistance < copy.m_fDistance);
	}

	void Set(float dist, T obj)
	{
		m_fDistance = dist;
		m_Object = obj;
	}
	void SetDistace(float dist)
	{
		m_fDistance = dist;
	}

	float GetDistance()
	{
		return m_fDistance;
	}

	void SetObject(T objectArg)
	{
		object = objectArg;
	}
	T &GetObject()
	{
		return m_Object;
	}


	//------------------------------------------------
	// Data
	//
private:
	float	m_fDistance;	///< 거리값
	T		m_Object;		///< 거리의 주체가 되는 객체
};

#endif // _3DTERRAIN_DISTANCEOBJECT_H_