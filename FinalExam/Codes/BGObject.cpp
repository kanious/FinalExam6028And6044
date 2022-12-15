#include "BGObject.h"
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
#include "TorchEffect.h"
#include "LightMaster.h"
#include "Light.h"


USING(Engine)
USING(glm)
USING(std)

BGObject::BGObject()
	: m_pMesh(nullptr), m_pTorchObj(nullptr), m_bTransparent(false)
{
	m_bDebug = false;
}

BGObject::~BGObject()
{
}

void BGObject::Update(const _float& dt)
{
	if (m_bEnable)
	{
		if (nullptr != m_pTorchObj)
			m_pTorchObj->Update(dt);

		CGameObject::Update(dt);

		if (nullptr != m_pRenderer)
			m_pRenderer->AddRenderObj(this, m_bTransparent);
	}
}

void BGObject::Render()
{
	if (nullptr != m_pTorchObj)
		m_pTorchObj->Render();

	CGameObject::Render();
}

void BGObject::Destroy()
{
	SafeDestroy(m_pTorchObj);

	CGameObject::Destroy();
}

RESULT BGObject::Ready(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale, int dir)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;
	m_meshName = meshID;

	if ("Crystal1" == meshID || "Crystal2" == meshID || "Crystal3" == meshID ||
		"Water" == meshID || "VisionCone" == meshID)
		m_bTransparent = true;
	else
		m_bTransparent = false;

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
		m_pMesh->SetTransparency(m_bTransparent);

		if ("Torch" == meshID || "Torch2" == meshID)
		{
			vec3 vTorchPos = vPos;
			if ("Torch" == meshID)
				vTorchPos.y += 0.6f;
			else
				vTorchPos.y += 0.45f;

			if (0 == dir)
				vTorchPos.z += 0.15f;
			else if (1 == dir)
				vTorchPos.x += 0.15f;
			else if (2 == dir)
				vTorchPos.x -= 0.15f;
			else if (3 == dir)
				vTorchPos.z -= 0.15f;

			m_pTorchObj = TorchEffect::Create(m_sceneTag, m_layerTag, m_objTag, pLayer
				, "Effect", vTorchPos, vec3(0.f), vec3(1.f));

			CLight::cLightInfo* pInfo = new CLight::cLightInfo();
			pInfo->name = "PointLight";
			if (0 == dir)
				vTorchPos.z += 1.f;
			else if (1 == dir)
				vTorchPos.x += 1.f;
			else if (2 == dir)
				vTorchPos.x -= 1.f;
			else if (3 == dir)
				vTorchPos.z -= 1.f;
			pInfo->position = vec4(vTorchPos.x, vTorchPos.y, vTorchPos.z, 1.f);
			pInfo->direction = vec4(0.f);
			pInfo->diffuse = vec4(1.f);
			pInfo->specular = vec4(1.f, 0.5f, 0.f, 1.f);
			pInfo->ambient = vec4(0.f);
			pInfo->atten = vec4(1.f, 0.09f, 0.032f, 1.f);
			pInfo->param1 = vec4(1.f, 0.f, 0.f, 1.f);
			pInfo->param2 = vec4(1.f, 0.f, 0.f, 0.f);

			CLightMaster::GetInstance()->AddLight(pInfo);
		}
	}

	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosRotScale(vPos, vRot, vScale);
		m_pTransform->Update(0);
	}

	return PK_NOERROR;
}

BGObject* BGObject::Create(_uint sTag, _uint lTag, _uint oTag, CLayer* pLayer, string meshID, vec3 vPos, vec3 vRot, vec3 vScale, int dir)
{
	BGObject* pInstance = new BGObject();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, meshID, vPos, vRot, vScale, dir))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}
