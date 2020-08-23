#include "cbase.h"
#include "mt19937_random.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CMT19937UniformRandomStream s_Mt19937;
IUniformRandomStream * g_pMt19937 = (IUniformRandomStream *)&s_Mt19937;

CMT19937UniformRandomStream::CMT19937UniformRandomStream() : m_mutex(), m_iIndex(0)
{
    SetSeed( (int)Plat_FloatTime() );
}

void CMT19937UniformRandomStream::SetSeed( int iSeed )
{
    AUTO_LOCK( m_mutex );
    m_iIndex = MT_NN;
    m_aState[0] = iSeed;
    for( uint64_t i = 1; i < MT_NN; i++ )
        m_aState[i] = ( 0x5851F42D4C957F2D * ( m_aState[i - 1] ^ ( m_aState[i - 1] >> 62 ) ) + i );
}

void CMT19937UniformRandomStream::Twist( void )
{
    AUTO_LOCK( m_mutex );
    m_iIndex = 0;
    for( uint64_t i = 0; i < MT_NN; i++ ) {
        const uint64_t y = ( m_aState[i] & 0xFFFFFFFF80000000 ) | ( m_aState[( i + 1 ) % MT_NN] & 0x000000007FFFFFFF );
        if( i < ( MT_NN - MT_MM ) )
            m_aState[i] = m_aState[i + MT_MM] ^ ( y >> 1 ) ^ ( ( y & 1 ) * 0xB5026F5AA96619E9 );
        else if( i < ( MT_NN - 1 ) )
            m_aState[i] = m_aState[i + ( MT_MM - MT_NN )] ^ ( y >> 1 ) ^ ( ( y & 1 ) * 0xB5026F5AA96619E9 );
        else
            m_aState[i] = m_aState[MT_MM - 1] ^ ( y >> 1 ) ^ ( ( y & 1 ) * 0xB5026F5AA96619E9 );
    }
}

uint64_t CMT19937UniformRandomStream::Generate( void )
{
    AUTO_LOCK( m_mutex );
    if( m_iIndex >= MT_NN )
        Twist();
    uint64_t y = m_aState[m_iIndex++];
    y ^= ( y >> 29 ) & 0x5555555555555555;
    y ^= ( y << 17 ) & 0x71D67FFFEDA60000;
    y ^= ( y << 37 ) & 0xFFF7EEE000000000;
    y ^= ( y >> 43 );
    return y;
}

int CMT19937UniformRandomStream::RandomInt( int iLow, int iHigh )
{
    if( iLow > iHigh ) {
        int save = iLow;
        iLow = iHigh;
        iHigh = save;
    }
    int result = (int)( (unsigned int)Generate() % ( (unsigned int)iHigh - (unsigned int)iLow + 1 ) + (unsigned int)iLow );
    return result;
}

float CMT19937UniformRandomStream::RandomFloat( float flLow, float flHigh )
{
    return ( ( ( Generate() & 0xFFFF ) / (float)0x10000 ) * ( flHigh - flLow ) ) + flLow;
}

float CMT19937UniformRandomStream::RandomFloatExp( float flMinVal, float flMaxVal, float flExponent )
{
    float result = ( Generate() & 0xFFFF ) / (float)0x10000;
    if( flExponent != 1.0 ) {
        result = powf( result, flExponent );
    }
    return ( result * ( flMaxVal - flMinVal ) ) + flMinVal;
}
