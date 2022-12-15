#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Component.h"
#include "..\Headers\GameObject.h"
#include "..\Headers\Renderer.h"
#include "..\Headers\Layer.h"
#include "..\Headers\Transform.h"
#include "..\Headers\ComponentMaster.h"
#include <Rpc.h> // For.UUID


USING(Engine)
USING(glm)
USING(std)

CGameObject::CGameObject()
{
	m_UUID = UUIDGenerate();
	m_meshName = "";
	m_objName = "";
	m_objTag = 0;
	m_layerTag = 0;
	m_sceneTag = 0;
	m_bEnable = true;
	m_bLock = false;
	m_bDebug = true;
	m_bWireFrame = false;
	m_mapComponent.clear();
	m_pRenderer = CRenderer::GetInstance(); m_pRenderer->AddRefCnt();
	m_pLayer = nullptr;
	m_pBoundingBox = nullptr;

	m_pTransform = CloneComponent<CTransform*>("Transform");
	AttachComponent("Transform", m_pTransform);
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update(const _float& dt)
{
	COMPONENT_MAP::iterator iter;
	for (iter = m_mapComponent.begin(); iter != m_mapComponent.end(); iter++)
	{
		if (nullptr != iter->second)
		{
			if (iter->second->GetEnable())
				iter->second->Update(dt);
		}
	}
}

void CGameObject::Render()
{
	COMPONENT_MAP::iterator iter;
	for (iter = m_mapComponent.begin(); iter != m_mapComponent.end(); ++iter)
	{
		if (nullptr != iter->second)
		{
			if (iter->second->GetEnable())
				iter->second->Render();
		}
	}
}

void CGameObject::Enable()
{
	m_bEnable = true;
}

void CGameObject::Disable()
{
	m_bEnable = false;
}

void CGameObject::Destroy()
{
	COMPONENT_MAP::iterator iter;
	for (iter = m_mapComponent.begin(); iter != m_mapComponent.end(); ++iter)
		SafeDestroy(iter->second);

	m_mapComponent.clear();

	SafeDestroy(m_pRenderer);

	::RpcStringFreeW((RPC_WSTR*)&m_UUID);
	m_UUID = NULL;
}

void CGameObject::SetupGameObject(_uint sceneTag, _uint layerTag, _uint objTag)
{
	SetSceneTag(sceneTag);
	SetLayerTag(layerTag);
	SetObjectTag(objTag);
}

vec3 CGameObject::GetPosition()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetPosition();
}

vec3 CGameObject::GetRotation()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetRotation();
}

_float CGameObject::GetRotationY()
{
	if (nullptr == m_pTransform)
		return 0.f;

	return m_pTransform->GetRotationY();
}

vec3 CGameObject::GetScale()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetScale();
}

const mat4x4* CGameObject::GetWorldMatrix()
{
	if (nullptr == m_pTransform)
		return nullptr;

	return m_pTransform->GetWorldMatrix();
}

void CGameObject::SetMeshName(string name)
{
	m_meshName = name;
}

void CGameObject::SetObjectName(std::string name)
{
	m_objName = name;
}

void CGameObject::SetObjectTag(_uint objTag)
{
	m_objTag = objTag;
}

void CGameObject::SetLayerTag(_uint layerTag)
{
	m_layerTag = layerTag;
}

void CGameObject::SetSceneTag(_uint sceneTag)
{
	m_sceneTag = sceneTag;
}

void CGameObject::SetParentLayer(CLayer* pLayer)
{
	m_pLayer = pLayer;
}

void CGameObject::SetDead(_bool dead)
{
	m_bDead = dead;
}

void CGameObject::SetPosition(vec3 vPos)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetPosition(vPos);
}

void CGameObject::SetRotation(vec3 vRot)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotation(vRot);
}

void CGameObject::SetRotationX(_float fAngle)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotationX(fAngle);
}

void CGameObject::SetRotationY(_float fAngle)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotationY(fAngle);
}

void CGameObject::SetRotationZ(_float fAngle)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotationZ(fAngle);
}

void CGameObject::SetScale(glm::vec3 vScale)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetScale(vScale);
}

void CGameObject::SetParentTransform(CTransform* pTransform)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetParent(pTransform);
}

void CGameObject::AttachComponent(std::string componentTag, CComponent* pInstance)
{
	if (nullptr == pInstance)
		return;

	COMPONENT_MAP::iterator iter = m_mapComponent.find(componentTag);
	if (iter != m_mapComponent.end())
	{
		SafeDestroy(iter->second);
		m_mapComponent.erase(iter);
	}
	m_mapComponent.insert(COMPONENT_MAP::value_type(componentTag, pInstance));
}

RESULT CGameObject::RemoveComponent(std::string componentTag)
{
	COMPONENT_MAP::iterator iter = m_mapComponent.find(componentTag);
	if (iter == m_mapComponent.end())
		return PK_COMPONENT_CANNOT_FIND;
	else
	{
		SafeDestroy(iter->second);
		m_mapComponent.erase(iter);
	}

	return PK_NOERROR;
}

CComponent* CGameObject::GetComponent(std::string componentTag)
{
	COMPONENT_MAP::iterator iter = m_mapComponent.find(componentTag);
	if (iter == m_mapComponent.end())
		return nullptr;
	else
		return iter->second;
}

_wchar_t* CGameObject::UUIDGenerate()
{
	UUID uuid;
	RPC_STATUS ret_val = ::I_UuidCreate(&uuid);

	_wchar_t* wUuid = NULL;
	if (ret_val == RPC_S_OK)
		::UuidToStringW(&uuid, (RPC_WSTR*)&wUuid);
	return wUuid;
}