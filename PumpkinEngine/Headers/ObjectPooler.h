#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include "Base.h"

NAMESPACE_BEGIN(Engine)

class CGameObject;

// 오브젝트 풀러로 교체

class ENGINE_API CObjectPooler : public CBase
{
	SINGLETON(CObjectPooler)

private:
	std::list<CGameObject*> m_listGameObjects;

private:
	explicit CObjectPooler();
	virtual ~CObjectPooler();
	void Destroy();
};

NAMESPACE_END

#endif //_OBJECTMANAGER_H_