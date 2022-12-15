#include "Beholder.h"
#include "ComponentMaster.h"
#include "Layer.h"
#include "LightMaster.h"
#include "Light.h"

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BoundingBox.h"
#include "OpenGLDefines.h"
#include "OpenGLDevice.h"
#include "BGObject.h"
#include "MapManager.h"


USING(Engine)
USING(glm)
USING(std)

Beholder::Beholder()
	: m_pMesh(nullptr), m_pConeObj(nullptr), m_eMode(ROAMING), m_eState(ARRIVED)
	, m_vTargetPos(vec3(0.f)), m_vTargetDir(vec3(0.f)), m_fRotSpeed(2.f), m_fMoveSpeed(2.f)
	, m_fElaspedTime(0.f), m_pTarget(nullptr), m_bDeathEffect(false)
{
	m_bDebug = false;
	m_pMapManager = MapManager::GetInstance(); m_pMapManager->AddRefCnt();
}

Beholder::~Beholder()
{
}

void Beholder::SetBeholderIndex(_uint index, _uint roomIndex)
{
	m_iIndex = index;
	m_iRoomIdx = roomIndex;
}

void Beholder::SetBattleMode(_bool value)
{
	if (value)
	{
		m_eMode = BATTLE;
		m_eState = ARRIVED;
		m_fRotSpeed = 4.f;
		m_fMoveSpeed = 4.f;
	}
	else
	{
		m_eMode = ROAMING;
		m_pTransform->SetPosition(m_vOriginPos);
		m_pTransform->SetRotation(m_vOriginRot);
		m_eState = ARRIVED;
		m_pTarget = nullptr;
		m_bDeathEffect = false;
		m_bGameEnd = false;
		m_bEnable = true;

		m_fRotSpeed = 2.f;
		m_fMoveSpeed = 2.f;

		m_pLightInfo[0]->param2.x = 1.f;
		m_pLightInfo[1]->param2.x = 1.f;
		m_pLightInfo[2]->param2.x = 1.f;

		m_pTransform->SetScale(vec3(1.f));
	}
}

void Beholder::DeadByEnemy()
{
	m_bDeathEffect = true;

	m_pLightInfo[0]->param2.x = 0.f;
	m_pLightInfo[1]->param2.x = 0.f;
	m_pLightInfo[2]->param2.x = 0.f;
}

void Beholder::Roaming(const _float& dt)
{
	m_fElaspedTime += dt;
	_float posY = m_pTransform->GetPositionY();
	posY = 2.5f + (sin(m_fElaspedTime) * 0.5f);
	m_pTransform->SetPositionY(posY);

	if (m_fElaspedTime > radians(360.f))
		m_fElaspedTime -= radians(360.f);

	if (ARRIVED == m_eState)
	{
		if (ROAMING == m_eMode)
			m_vTargetPos = m_pMapManager->GetNextTargetPos(m_iIndex);
		else
			m_vTargetPos = m_pMapManager->GetNextPathPos(this, m_pTarget);

		vec3 vPos = m_pTransform->GetPosition();
		vPos.y = 0.f;
		m_vTargetDir = m_vTargetPos - vPos;
		m_vTargetDir = normalize(m_vTargetDir);
		m_eState = ROTATING;
	}
	else if (ROTATING == m_eState)
	{
		vec3 vLook = m_pTransform->GetLookVector();
		_float fAngleGap = safe_acos(dot(m_vTargetDir, vLook));

		fAngleGap = degrees(fAngleGap);
		vec3 vRight = m_pTransform->GetRightVector();
		_float fDir = dot(m_vTargetDir, vRight);
		_float fAngleY = m_pTransform->GetRotationY();

		if (1.f < fAngleGap)
		{
			if (0.f <= fDir)
				fAngleY += fAngleGap * dt * m_fRotSpeed;
			else
				fAngleY -= fAngleGap * dt * m_fRotSpeed;

			if (fAngleY > 360.f)
				fAngleY -= 360.f;
			else if (fAngleY < 0.f)
				fAngleY += 360.f;

			m_pTransform->SetRotationY(fAngleY);
		}
		else
		{
			if (0.f <= fDir)
				fAngleY += fAngleGap;
			else
				fAngleY -= fAngleGap;

			if (fAngleY > 360.f)
				fAngleY -= 360.f;
			else if (fAngleY < 0.f)
				fAngleY += 360.f;

			m_pTransform->SetRotationY(fAngleY);
			m_eState = MOVING;
		}
	}
	else
	{
 		vec3 vLook = m_pTransform->GetLookVector();
		vec3 vPos = m_pTransform->GetPosition();
		vPos.y = 0.f;
		_float dist = distance(m_vTargetPos, vPos);

		if (0.1f < dist)
		{
			m_pTransform->AddPosition(vLook * dt * m_fMoveSpeed);
		}
		else
		{
			vec3 vPos = m_pTransform->GetPosition();
			m_vTargetPos.y = vPos.y;
			m_pTransform->SetPosition(m_vTargetPos);
			m_eState = ARRIVED;
		}
	}
}

void Beholder::Battle(const _float& dt)
{
	if (nullptr == m_pTarget)
		m_pTarget = m_pMapManager->GetLockOnTarget(this);

	if (nullptr == m_pTarget)
		return;

	if (m_pTarget->GetDeadEffect())
	{
		m_pTarget = m_pMapManager->GetLockOnTarget(this);
		m_eState = ARRIVED;
	}

	if (nullptr == m_pTarget)
		return;

	vec3 vPos = m_pTransform->GetPosition();
	vec3 vTargetPos = m_pTarget->GetPosition();
	_float dist = distance(vPos, vTargetPos);

	if (10.f > dist)
	{
		// Rotate To Target
		vec3 vDir = vTargetPos - vPos;
		vDir = normalize(vDir);

		vec3 vLook = m_pTransform->GetLookVector();
		_float fAngleGap = safe_acos(dot(vDir, vLook));

		fAngleGap = degrees(fAngleGap);
		vec3 vRight = m_pTransform->GetRightVector();
		_float fDir = dot(vDir, vRight);
		_float fAngleY = m_pTransform->GetRotationY();

		if (1.f < fAngleGap)
		{
			if (0.f <= fDir)
				fAngleY += fAngleGap * dt * m_fRotSpeed;
			else
				fAngleY -= fAngleGap * dt * m_fRotSpeed;

			if (fAngleY > 360.f)
				fAngleY -= 360.f;
			else if (fAngleY < 0.f)
				fAngleY += 360.f;

			m_pTransform->SetRotationY(fAngleY);
		}
		else
		{
			if (0.f <= fDir)
				fAngleY += fAngleGap;
			else
				fAngleY -= fAngleGap;

			if (fAngleY > 360.f)
				fAngleY -= 360.f;
			else if (fAngleY < 0.f)
				fAngleY += 360.f;

			m_pTransform->SetRotationY(fAngleY);

			// KILL!!!
			m_pMapManager->KillEnemy(m_pTarget);
			m_pTarget = nullptr;
			m_eState = ARRIVED;
		}
	}
	else
	{
		Roaming(dt);
	}
}

void Beholder::RunDeathEffect(const _float& dt)
{
	vec3 vScale = m_pTransform->GetScale();
	if (0.01f > vScale.x)
	{
		m_bEnable = false;

		m_pMapManager->RemoveBeholder();
	}

	m_pTransform->AddPositionY(dt * 2.f);
	m_pTransform->AddRotationY(dt * m_fRotSpeed * 200.f);
	vScale -= dt * 0.3f;
	m_pTransform->SetScale(vScale);
}

void Beholder::UpdateLightInfo()
{
	vec3 vPos = m_pTransform->GetPosition();
	vec3 vDir = m_pTransform->GetLookVector();
	vec3 vRight = m_pTransform->GetRightVector();
	vec4 vPosFinal(0.f);

	vec3 vPosTemp = vPos + (vRight * -1.6f);
	vPosTemp.y += 0.9f;
	vPosFinal = vec4(vPosTemp.x, vPosTemp.y, vPosTemp.z, 1.f);
	m_pLightInfo[0]->position = vPosFinal;

	vPosTemp = vPos + (vRight * -0.45f);
	vPosTemp.y += 1.65f;
	vPosFinal = vec4(vPosTemp.x, vPosTemp.y, vPosTemp.z, 1.f);
	m_pLightInfo[1]->position = vPosFinal;

	vPosTemp = vPos + (vRight * 1.6f);
	vPosTemp.y += 0.75f;
	vPosFinal = vec4(vPosTemp.x, vPosTemp.y, vPosTemp.z, 1.f);
	m_pLightInfo[2]->position = vPosFinal;


	m_pLightInfo[0]->direction = vec4(vDir.x, vDir.y, vDir.z, 1.f);
	m_pLightInfo[1]->direction = vec4(vDir.x, vDir.y, vDir.z, 1.f);
	m_pLightInfo[2]->direction = vec4(vDir.x, vDir.y, vDir.z, 1.f);
}

void Beholder::Update(const _float& dt)
{
	if (m_bEnable)
	{
		if (m_bDeathEffect)
		{
			RunDeathEffect(dt);
		}
		else
		{
			if (!m_bGameEnd)
			{
				if (ROAMING == m_eMode)
					Roaming(dt);
				else
					Battle(dt);
			}
		}

		CGameObject::Update(dt);

		if (nullptr != m_pConeObj)
			m_pConeObj->Update(dt);

		if (nullptr != m_pRenderer)
		{
			m_pRenderer->AddRenderObj(this);
			m_pRenderer->AddRenderObj(m_pConeObj, true);
		}

		UpdateLightInfo();
	}
}

void Beholder::Render()
{
	CGameObject::Render();
}

void Beholder::Destroy()
{
	SafeDestroy(m_pConeObj);
	SafeDestroy(m_pMapManager);

	CGameObject::Destroy();
}

RESULT Beholder::Ready(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;
	m_meshName = meshID;
	m_vOriginPos = vPos;
	m_vOriginRot = vRot;

	//Clone.Mesh
	m_pMesh = CloneComponent<CMesh*>(meshID);
	if (nullptr != m_pMesh)
	{
		AttachComponent("Mesh", m_pMesh);
		m_pMesh->SetTransform(m_pTransform);
		m_pBoundingBox = m_pMesh->GetBoundingBox();
		if (nullptr != m_pBoundingBox)
			m_pBoundingBox->SetTransform(m_pTransform);
		m_pMesh->SetWireFrame(false);
		m_pMesh->SetDebugBox(false);
		m_pMesh->SetTransparency(false);
	}
	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosRotScale(vPos, vRot, vScale);
		m_pTransform->Update(0);
	}

	m_pConeObj = BGObject::Create(m_sceneTag, m_layerTag, m_objTag, pLayer
		, "VisionCone", vec3(0.f), vec3(0.f, 180.f, 0.f), vec3(1.f));
	if (nullptr != m_pConeObj)
		m_pConeObj->SetParentTransform(m_pTransform);

	// Add Spotlights
	vec4 position = vec4(0.f);
	vec4 direction = vec4(0.f);
	vec4 diffuse = vec4(1.f);
	vec4 specular = vec4(1.5f, 1.5, 1.5, 0.1f);
	vec4 ambient = vec4(0.f);
	vec4 atten = vec4(1.f, 0.22f, 0.2f, 1.f);
	vec4 param1 = vec4(2.f, 5.f, 7.f, 1.f);
	vec4 param2 = vec4(1.f, 0.f, 0.f, 0.f);

	for (int i = 0; i < 3; ++i)
	{
		m_pLightInfo[i] = new CLight::cLightInfo();
		m_pLightInfo[i]->name = "BeholderSpotLight" + i;
		m_pLightInfo[i]->position = position;
		m_pLightInfo[i]->direction = direction;
		m_pLightInfo[i]->diffuse = diffuse;
		m_pLightInfo[i]->specular = specular;
		m_pLightInfo[i]->ambient = ambient;
		m_pLightInfo[i]->atten = atten;
		m_pLightInfo[i]->param1 = param1;
		m_pLightInfo[i]->param2 = param2;
		CLightMaster::GetInstance()->AddLight(m_pLightInfo[i]);
	}

	return PK_NOERROR;
}

Beholder* Beholder::Create(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale)
{
	Beholder* pInstance = new Beholder();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, meshID, vPos, vRot, vScale))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}


_float Beholder::safe_acos(_float value)
{
	if (value <= -1.0f)
		return radians(180.f);
	else if (value >= 1.0f)
		return 0.f;
	else
		return acos(value);
}
