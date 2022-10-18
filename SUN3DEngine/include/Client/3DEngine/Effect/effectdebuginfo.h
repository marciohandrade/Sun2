// effectdebuginfo.h

#ifndef _EFFECT_EFFECTDEBUGINFO_H_
#define _EFFECT_EFFECTDEBUGINFO_H_

// µð¹ö±ë Á¤º¸
struct St_EffDbgInfo
{
    int numSkippedEffect;

    int numRenderedEffect;
    int maxNumRenderedEffect;

    int numRenderedSprite;
    int maxNumRenderedSprite;

    int numRenderedMesh;
    int maxNumRenderedMesh;

    int numRenderedChain;
    int maxNumRenderedChain;

    int numRenderedFlatChain;
    int maxNumRenderedFlatChain;

    int numRenderedDecal;
    int maxNumRenderedDecal;

    int numRenderedWeather;
    int maxNumRenderedWeather;

    St_EffDbgInfo()
    {
        memset( this, 0, sizeof( St_EffDbgInfo ) );
    }

    void ClearRenderInfo()
    {
        numRenderedEffect = 0;
        numRenderedSprite = 0;
        numRenderedMesh = 0;
        numRenderedChain = 0;
        numRenderedFlatChain = 0;
        numRenderedDecal = 0;
        numRenderedWeather = 0;
    }

    void IncNumRenderedEffect()
    {
        if( ++numRenderedEffect > maxNumRenderedEffect )
        {
            maxNumRenderedEffect = numRenderedEffect;
        }
    }

    void IncNumRenderedSprite()
    {
        if( ++numRenderedSprite > maxNumRenderedSprite )
        {
            maxNumRenderedSprite = numRenderedSprite;
        }
    }

    void IncNumRenderedMesh()
    {
        if( ++numRenderedMesh > maxNumRenderedMesh )
        {
            maxNumRenderedMesh = numRenderedMesh;
        }
    }

    void IncNumRenderedChain()
    {
        if( ++numRenderedChain > maxNumRenderedChain )
        {
            maxNumRenderedChain = numRenderedChain;
        }
    }

    void IncNumRenderedFlatChain()
    {
        if( ++numRenderedFlatChain > maxNumRenderedFlatChain )
        {
            maxNumRenderedFlatChain = numRenderedFlatChain;
        }
    }

    void IncNumRenderedDecal()
    {
        if( ++numRenderedDecal > maxNumRenderedDecal )
        {
            maxNumRenderedDecal = numRenderedDecal;
        }
    }

    void IncNumRenderedWeather()
    {
        if( ++numRenderedWeather > maxNumRenderedWeather )
        {
            maxNumRenderedWeather = numRenderedWeather;
        }
    }
};

#endif // _EFFECT_EFFECTDEBUGINFO_H_