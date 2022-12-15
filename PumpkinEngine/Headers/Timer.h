#ifndef _TIMEMANAGER_H_
#define _TIMEMANAGER_H_

#include "Base.h"
#include <chrono>

NAMESPACE_BEGIN(Engine)

class ENGINE_API CTimer : public CBase
{
	SINGLETON(CTimer)

private:
	std::chrono::steady_clock::time_point		m_TimeLastUpdate;
	std::chrono::steady_clock::time_point		m_TimeCurrentUpdate;
	_float										m_fCurrentTime;
	_float										m_fTimeDelta;
	_float										m_fTimeDefault;
	_float										m_fFrameRate;

private:
	explicit CTimer();
	virtual ~CTimer();
	void Destroy();
public:
	void Update();

public:
	_float GetTimeDelta() { return m_fTimeDelta > 0.1 ? 0 : m_fTimeDelta; }
	_float GetTimeDefault() { return m_fTimeDefault; }
	void SetFrameRate(_int frameRate);
	_bool IsUpdateAvailable();

};

NAMESPACE_END

#endif //_TIMEMANAGER_H_