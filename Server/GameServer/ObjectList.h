#ifndef __OBJECT_LIST_H__
#define __OBJECT_LIST_H__

#pragma once

#include <Object.h>


class ObjectList
{
public:		
    typedef DWORD ObjectKey;

public:		
    ObjectList();
    virtual ~ObjectList(); 
    
public:		
    void Add(Object* const object)
    {
        if (object == NULL)
        {
            return;
        }
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
        if (object->IsDeleted())
        {
            const ObjectKey key = object->GetObjectKey();
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                __FUNCTION__, 
                __LINE__, 
                key
            );
            return;
        }
//#endif
        const ObjectKey key = object->GetObjectKey();
        objects_[key] = object;
    }

    void Remove(const ObjectKey object_key)	
    {	
        objects_.erase(object_key);	
    }
    void RemoveAll() 
    {	
        objects_.clear();	
    }

    size_t GetSize() const
    { 
        return objects_.size(); 
    }

    template <typename OPERATOR_T>
    void Foreach(OPERATOR_T& op)
    {
        for (Objects::iterator it = objects_.begin(); it != objects_.end();)
        {
            Object* const object = it->second;
            if (object == NULL)
            {
                objects_.erase(it++);
            }
            else
            {
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
                if (object->IsDeleted())
                {
                    const ObjectKey key = object->GetObjectKey();
                    SUNLOG(
                        eCRITICAL_LOG, 
                        _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                        __FUNCTION__, 
                        __LINE__, 
                        key
                    );
                    objects_.erase(it++);
                    continue;
                }
//#endif
                op(object);
                ++it;
            }
        }
    }

private:
    typedef STLX_MAP<ObjectKey, Object*> Objects;
    Objects	objects_;
};

#endif // __OBJECT_LIST_H__