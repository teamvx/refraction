#ifndef USTDLIB_RANDOM_H
#define USTDLIB_RANDOM_H
#ifdef _WIN32
#pragma once
#endif
#include <stdint.h>
#include <vstdlib/random.h>

#define MT_NN 312
#define MT_MM 156

class CMT19937UniformRandomStream : public IUniformRandomStream {
public:
    CMT19937UniformRandomStream();

    void SetSeed( int iSeed = 5489 );
    void Twist( void );
    uint64_t Generate( void );

    virtual int     RandomInt( int iMinVal, int iMaxVal );
    virtual float   RandomFloat( float flMinVal = 0.0f, float flMaxVal = 1.0f );
    virtual float   RandomFloatExp( float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f );

private:
    CThreadFastMutex m_mutex;
    uint64_t m_iIndex;
    uint64_t m_aState[MT_NN];
};
extern IUniformRandomStream * g_pMt19937;

#endif
