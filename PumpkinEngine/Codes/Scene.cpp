#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Scene.h"
#include "..\Headers\Layer.h"
#include "..\Headers\GameObject.h"


USING(Engine)
USING(std)

CScene::CScene()
	: m_tag(0), m_DataPath(""), m_ObjListFileName(""), m_LightListFileName("")
{
	m_tag = 0;
	m_vecLayer.clear();
}

CScene::~CScene()
{
}

void CScene::Update(const _float& dt)
{
	vector<CLayer*>::iterator iter;
 	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter)
			(*iter)->Update(dt);
	}
}

void CScene::Render()
{
}

void CScene::Destroy()
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
		SafeDestroy(*iter);

	m_vecLayer.clear();
}

void CScene::SetSceneTag(_uint sceneTag)
{
	m_tag = sceneTag;
}

RESULT CScene::ToggleLayer(_uint layerTag, _bool turn)
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter && (*iter)->GetTag() == layerTag)
		{
			if (turn)
				(*iter)->Enable();
			else
				(*iter)->Disable();

			return PK_NOERROR;
		}
	}

	return PK_LAYER_CANNOT_FIND;
}

CLayer* CScene::GetLayer(_uint layerTag)
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter && (*iter)->GetTag() == layerTag)
				return (*iter);
	}

	CLayer* pLayer = CLayer::Create(layerTag);
	m_vecLayer.push_back(pLayer);

	return pLayer;
}

RESULT CScene::AddGameObjectToLayer(_uint layerTag, CGameObject* pInstance)
{
	vector<CLayer*>::iterator iter;
	for (iter = m_vecLayer.begin(); iter != m_vecLayer.end(); ++iter)
	{
		if (nullptr != *iter && (*iter)->GetTag() == layerTag)
		{
			(*iter)->AddGameObject(pInstance);
			return PK_NOERROR;
		}
	}
	return PK_LAYER_CANNOT_FIND;
}

void CScene::SaveBackgroundObjects()
{
}

void CScene::LoadBackgroundObjects()
{
}