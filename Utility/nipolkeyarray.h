#ifndef N_IPOLKEYARRAY_H
#define N_IPOLKEYARRAY_H

//------------------------------------------------------------------------------
/**
    @brief A Array for interpolation between keys, used by the animators
    for a linear interpolation the data type must have the function
    lerp(TYPE val0, TYPE val1, float lerp)
     

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.30
    @remarks
    - 지완 추가 
*/

#include "../ProgramCommon/Macro.h"
#include "../ProgramCommon/Define.h"


//------------------------------------------------------------------------------
template<class TYPE>
class nIpolKeyArray
{
public:
    enum LoopType
    {
        Loop,           // looping
        OneShot,        // one shot (clamping)
    };

    nIpolKeyArray()
    {
        //empty
    }
    
    /// constuctor with initial size and grow size of the internal elements nArray
    nIpolKeyArray(int initialSize, int grow) :
        keyArray(initialSize, grow)
    {
        //empty
    }

    ~nIpolKeyArray()
    {
        //empty
    }

    ///get interpolated key value at time            
    bool SampleKey(float time, TYPE& result, const LoopType loopType);
    
    /// add a key
    void AddKey(float t, const TYPE& key)
    {
        Key newKey(t, key);
        this->keyArray.Append(newKey);
    }
    
    /// get number of keys
    int  GetNumKeys() const
    {
        return this->keyArray.Size();
    }
    
    /// get key at index
    void GetKeyAt(const int index, float& time, TYPE& key) const
    {
        const Key& k = this->keyArray[index];
        time = k.time;
        key  = k.value;
    }
    
private:
    class Key
    {
    public:
        Key() :
            time(0.0f)
        {
            //empty
        }
        Key(float t, const TYPE& v) :
            time(t), value(v)
        {
            //empty
        }

        ~Key()
        {
            //empty
        }
        
        float time;
        TYPE value;
    };
    
    nArray<Key> keyArray;
};

//------------------------------------------------------------------------------
/**
    Compute an interpolated key value from a time and a key array.

    @param  sampleTime  [in] time position at which to sample
    @param  result      [out] the result
    @return             true if the result is valid, false otherwise
*/
template<class TYPE>
inline bool
nIpolKeyArray<TYPE>::SampleKey(float sampleTime, TYPE& result, const LoopType loopType)
{
    if (keyArray.Size() > 1)
    {
        float minTime = keyArray.Front().time;
        float maxTime = keyArray.Back().time;
        if (maxTime > 0.0f)
        {
            if (loopType == Loop)
            {
                // in loop mode, wrap time into loop time
                sampleTime = sampleTime - (float(floor(sampleTime / maxTime)) * maxTime);
            }

            // clamp time to range
            if (sampleTime < minTime)       sampleTime = minTime;
            else if (sampleTime >= maxTime) sampleTime = maxTime - 0.001f;

            // find the surrounding keys
            ASSERT(keyArray.Front().time == 0.0f);
            int i = 0;;
            while (keyArray[i].time <= sampleTime)
            {
                i++;
            }
            ASSERT((i > 0) && (i < keyArray.Size()));

            const Key& key0 = keyArray[i - 1];
            const Key& key1 = keyArray[i];
            float time0 = key0.time;
            float time1 = key1.time;

            // compute the actual interpolated values
            float lerp;
            if (time1 > time0) lerp = (float) ((sampleTime - time0) / (time1 - time0));
            else               lerp = 1.0f;
            
            result.lerp(key0.value, key1.value, lerp);
            return true;
        }
    }
    return false;
}

#endif

