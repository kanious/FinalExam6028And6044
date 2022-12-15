#include "TorchEffect.h"
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
#include "LightMaster.h"
#include "Shader.h"


USING(Engine)
USING(glm)
USING(std)

TorchEffect::TorchEffect()
	: m_pMesh(nullptr), m_pShader(nullptr), m_fFrameIndex(0.f)
{
	m_bDebug = false;
}

TorchEffect::~TorchEffect()
{
}

void TorchEffect::Update(const _float& dt)
{
	if (m_bEnable)
	{
		m_fFrameIndex += dt * 12.f;
		if (16.f < m_fFrameIndex)
			m_fFrameIndex -= 16.f;

		CGameObject::Update(dt);

		if (nullptr != m_pRenderer)
			m_pRenderer->AddRenderObj(this, true);
	}
}

void TorchEffect::Render()
{
	if (nullptr != m_pShader)
		m_pShader->SetFrameIndex((_uint)m_fFrameIndex);
	
	CGameObject::Render();
}

void TorchEffect::Destroy()
{
	CGameObject::Destroy();
}

RESULT TorchEffect::Ready(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;
	m_meshName = meshID;

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
		m_pMesh->SetTransparency(true);
		m_pMesh->SetBillboard(true);

		m_pShader = m_pMesh->GetShader();
	}

	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosRotScale(vPos, vRot, vScale);
		m_pTransform->Update(0);
	}

	return PK_NOERROR;
}

TorchEffect* TorchEffect::Create(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale)
{
	TorchEffect* pInstance = new TorchEffect();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, meshID, vPos, vRot, vScale))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
