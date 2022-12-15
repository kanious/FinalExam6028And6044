#include "Character.h"
#include "ComponentMaster.h"
#include "Layer.h"

#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Renderer.h"
#include "BoundingBox.h"
#include "OpenGLDefines.h"
#include "OpenGLDevice.h"
#include "SoundMaster.h"
#include "InputDevice.h"
#include "BGObject.h"


USING(Engine)
USING(glm)
USING(std)

Character::Character()
	: m_pMesh(nullptr), m_vVelocity(vec3(0.f)), m_vAccel(vec3(0.f)), m_vForce(vec3(0.f))
	, m_vRotForce(vec3(0.f)), m_pTerrain(nullptr), m_pTerrainOctree(nullptr), m_bMove(false)
	, m_fSpeed(3.f), m_fRotSpeed(50.f), m_bCollision(false)
{
	m_bDebug = false;
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
}

Character::~Character()
{
}

void Character::SetTerrain(BGObject* terrain)
{
	if (nullptr == terrain)
		return;

	m_pTerrain = terrain;
	m_pTerrainOctree = m_pTerrain->GetOctreeFromMesh();
}

TRIANGLE* Character::GetTrianglesFromMesh()
{
	if (nullptr != m_pMesh)
		return m_pMesh->GetTriangleArray();

	return nullptr;
}

_uint Character::GetTriangleNumber()
{
	if (nullptr != m_pMesh)
		return m_pMesh->GetTriangleNumber();

	return 0;
}

COctree* Character::GetOctreeFromMesh()
{
	if (nullptr != m_pMesh)
		return m_pMesh->GetOctree();

	return nullptr;
}

void Character::KeyCheck(const _float& dt)
{
	if (nullptr == m_pTransform || nullptr == m_pInputDevice)
		return;

	m_bMove = false;

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_T))
	{
		m_bMove = true;
		AddForce(vec3(0.f, 0.f, -1.f) * m_fSpeed * dt);
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_G))
	{
		m_bMove = true;
		AddForce(vec3(0.f, 0.f, 1.f) * m_fSpeed * dt);
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_F))
	{
		m_bMove = true;
		AddForce(vec3(-1.f, 0.f, 0.f) * m_fSpeed * dt);
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_H))
	{
		m_bMove = true;
		AddForce(vec3(1.f, 0.f, 0.f) * m_fSpeed * dt);
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_U))
	{
		m_bMove = true;
		AddForce(vec3(0.f, 1.f, 0.f) * m_fSpeed * dt);
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_J))
	{
		m_bMove = true;
		AddForce(vec3(0.f, -1.f, 0.f) * m_fSpeed * dt);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_R))
	{
		m_bMove = true;
		m_vRotForce.y += m_fRotSpeed * dt;
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_Y))
	{
		m_bMove = true;
		m_vRotForce.y -= m_fRotSpeed * dt;
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_LEFT))
	{
		m_bMove = true;
		m_vRotForce.z += m_fRotSpeed * dt;
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_RIGHT))
	{
		m_bMove = true;
		m_vRotForce.z -= m_fRotSpeed * dt;
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_UP))
	{
		m_bMove = true;
		m_vRotForce.x += m_fRotSpeed * dt;
	}
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_DOWN))
	{
		m_bMove = true;
		m_vRotForce.x -= m_fRotSpeed * dt;
	}

	//static _bool isF1Down = false;
	//if (m_pInputDevice->IsKeyDown(GLFW_KEY_F1))
	//{
	//	if (!isF1Down)
	//	{
	//		isF1Down = true;

	//		if (nullptr != m_pTerrainOctree)
	//		{
	//			m_pTerrainOctree->SetDebug(!m_pTerrainOctree->GetDebug());
	//		}
	//	}
	//}
	//else
	//	isF1Down = false;
}


void Character::Update(const _float& dt)
{
	if (m_bEnable)
	{
		KeyCheck(dt);

		m_vAccel = m_vForce;
		m_vVelocity = (m_vAccel);
		m_pTransform->AddPosition(m_vVelocity);
		m_pTransform->AddRotation(m_vRotForce);

		m_vForce = vec3(0.f);
		m_vRotForce = vec3(0.f);

		CGameObject::Update(dt);

		if (nullptr != m_pRenderer)
			m_pRenderer->AddRenderObj(this);
	}
}

void Character::Render()
{
	CGameObject::Render();

}

void Character::Destroy()
{
	SafeDestroy(m_pInputDevice);

	CGameObject::Destroy();
}

RESULT Character::Ready(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale, string soundTag)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;
	m_meshName = meshID;

	CComponentMaster* pMaster = CComponentMaster::GetInstance();
	CComponent* pComponent = nullptr;

	//Clone.Mesh
 	m_pMesh = CloneComponent<CMesh*>(meshID);
	if (nullptr != m_pMesh)
	{
		AttachComponent("Mesh", m_pMesh);
		m_pMesh->SetTransform(m_pTransform);
		m_pBoundingBox = m_pMesh->GetBoundingBox();
		if (nullptr != m_pBoundingBox)
			m_pBoundingBox->SetTransform(m_pTransform);

		m_pMesh->SetDebugBox(false);
	}

	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosRotScale(vPos, vRot, vScale);
		m_pTransform->Update(0);
	}

	return PK_NOERROR;
}

Character* Character::Create(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale, string soundTag)
{
	Character* pInstance = new Character();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, meshID, vPos, vRot, vScale, soundTag))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
