#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Layer.h"
#include "..\Headers\GameObject.h"


USING(Engine)
USING(std)
_bool DeadFinder(CGameObject* pObj);

CLayer::CLayer()
{
	m_tag = 0;
	m_bEnable = true;
	m_listGameObjects.clear();
}

CLayer::~CLayer()
{
}

void CLayer::Update(const _float& dt)
{
	m_listGameObjects.erase(
		std::remove_if(m_listGameObjects.begin(), m_listGameObjects.end(), DeadFinder),
		m_listGameObjects.end());

	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
	{
		if (nullptr != (*iter))
			(*iter)->Update(dt);
	}

	vector<int> newVector;
	newVector.push_back(10);
	newVector.erase(newVector.begin());
}

void CLayer::Render()
{
}

void CLayer::Enable()
{
	m_bEnable = true;
}

void CLayer::Disable()
{
	m_bEnable = false;
}

void CLayer::Destroy()
{
	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
		SafeDestroy(*iter);

	m_listGameObjects.clear();
}

void CLayer::SetTag(_uint layerTag)
{
	m_tag = layerTag;
}

RESULT CLayer::AddGameObject(CGameObject* obj)
{
	if (nullptr == obj)
		return PK_GAMEOBJECT_NULLPTR;

	m_listGameObjects.push_back(obj);

	return PK_NOERROR;
}

RESULT CLayer::RemoveGameObject(CGameObject* obj)
{
	if (nullptr == obj)
		return PK_GAMEOBJECT_NULLPTR;

	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
	{
		if (nullptr != (*iter))
		{
			if ((*iter) == obj)
			{
				SafeDestroy(*iter);
				m_listGameObjects.remove(*iter);
				return PK_NOERROR;
			}
		}
	}

	return PK_GAMEOBJECT_CANNOT_FIND;
}

void CLayer::RemoveAllGameObject()
{
	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
		SafeDestroy(*iter);

	m_listGameObjects.clear();
}

CGameObject* CLayer::FindGameObjectByObjName(string objName)
{
	list<CGameObject*>::iterator iter;
	for (iter = m_listGameObjects.begin(); iter != m_listGameObjects.end(); ++iter)
	{
		if (objName == (*iter)->GetObjName())
			return (*iter);
	}

	return nullptr;
}

RESULT CLayer::Ready(_uint tag)
{
	m_tag = tag;

	return PK_NOERROR;
}

CLayer* CLayer::Create(_uint tag)
{
	CLayer* pInstance = new CLayer;

	if (PK_NOERROR != pInstance->Ready(tag))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}

_bool DeadFinder(CGameObject* pObj)
{
	_bool isDead = pObj->GetDead();

	if (isDead)
		SafeDestroy(pObj);

	return isDead;
}