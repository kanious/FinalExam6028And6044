#ifndef _GAMEMASTER_H_
#define _GAMEMASTER_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

class CScene;
class CRenderer;

class ENGINE_API CGameMaster : public CBase
{
	SINGLETON(CGameMaster)

private:
	CScene*				m_pCurrentScene;
	CRenderer*			m_pRenderer;
	_bool				m_bGameClose;


private:
	explicit CGameMaster();
	virtual ~CGameMaster();
public:
	void Update(const _float& dt);
	void Render();
private:
	void Destroy();

public:
	CScene* GetCurrentScene()			{ return m_pCurrentScene; }
	_bool GetGameClose()				{ return m_bGameClose; }
	void SetCurrentScene(CScene* pScene);
	void SetGameClose(_bool value)		{ m_bGameClose = value; }
};

NAMESPACE_END

#endif //_GAMEMASTER_H_