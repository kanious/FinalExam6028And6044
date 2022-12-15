#include "pch.h"
#include "..\Headers\Timer.h"

USING(Engine)
USING(std)
SINGLETON_FUNCTION(CTimer)

CTimer::CTimer()
{
    m_TimeLastUpdate = chrono::steady_clock::now();
    m_TimeCurrentUpdate = chrono::steady_clock::now();
    m_fCurrentTime = 0.f;
    m_fTimeDelta = 0.f;
    m_fTimeDefault = 0.f;
    m_fFrameRate = 0.f;
}

CTimer::~CTimer()
{
    
}

void CTimer::Destroy()
{
}

void CTimer::Update()
{
    m_TimeLastUpdate = m_TimeCurrentUpdate;
    m_TimeCurrentUpdate = chrono::steady_clock::now();

    std::chrono::duration<_float> elapsed = m_TimeCurrentUpdate - m_TimeLastUpdate;
    m_fTimeDefault = elapsed.count();
    m_fCurrentTime += m_fTimeDefault;
}

void CTimer::SetFrameRate(_int frameRate)
{
    m_fFrameRate = 1.f / frameRate;
}

_bool CTimer::IsUpdateAvailable()
{
    if (m_fCurrentTime > m_fFrameRate)
    {
        m_fTimeDelta = m_fCurrentTime;
        m_fCurrentTime = 0.f;
        return true;
    }

    return false;
}
