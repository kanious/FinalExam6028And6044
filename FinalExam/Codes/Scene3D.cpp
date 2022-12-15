#include "Scene3D.h"
#include "Function.h"
#include "glm\vec3.hpp"
#include "InputDevice.h"
#include "DefaultCamera.h"
#include <sstream>
#include <atlconv.h>
#include "XMLParser.h"
#include "Layer.h"
#include "ComponentMaster.h"
#include "Component.h"
#include "Shader.h"
#include "LightMaster.h"
#include "BGObject.h"
#include "Define.h"
#include "UIManager.h"
#include "Enums.h"
#include "SkyBox.h"
#include "Renderer.h"
#include "TorchEffect.h"
#include "MapManager.h"
#include "Beholder.h"


USING(Engine)
USING(glm)
USING(std)

Scene3D::Scene3D()
	: m_pDefaultCamera(nullptr), m_pObjLayer(nullptr), m_pSkyBox(nullptr)
	, m_vCameraSavedPos(vec3(0.f)), m_vCameraSavedRot(vec3(0.f)), m_vCameraSavedTarget(vec3(0.f))
	, m_bFollowingMode(false)
{
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
	m_pInputDevice->SetMouseSensitivity(0.05f);
	m_pUIManager = UIManager::GetInstance(); m_pUIManager->AddRefCnt();
	m_pMapManager = MapManager::GetInstance(); m_pMapManager->AddRefCnt();
	

	wchar_t path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, MAX_PATH);
	USES_CONVERSION;
	std::string str = W2A(path);
	str = str.substr(0, str.find_last_of("\\/"));//
	stringstream ss;
	ss << str << "\\..\\";

	m_DataPath = ss.str();
	m_ObjListFileName = "FinalExam_mapObjects.xml";
	m_LightListFileName = "FinalExam_lights.xml";
}

Scene3D::~Scene3D()
{
}

void Scene3D::KeyCheck()
{
	static _bool isF5Down = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_F5))
	{
		if (!isF5Down)
		{
			isF5Down = true;
			ResetDefaultCameraPos();

			m_bFollowingMode = false;
			m_pDefaultCamera->SetFollowingMode(false);
			m_pDefaultCamera->SetFollowingTarget(nullptr);
			m_pMapManager->ResetCameraTarget();
		}
	}
	else
		isF5Down = false;

	static _bool isF6Down = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_F6))
	{
		if (!isF6Down)
		{
			isF6Down = true;

			if (m_bFollowingMode)
			{
				Beholder* pTarget = m_pMapManager->GetNextCameraTarget();
				m_pDefaultCamera->SetFollowingTarget(pTarget);
			}
			else
			{
				m_bFollowingMode = true;
				m_pDefaultCamera->SetFollowingMode(true);

				Beholder* pTarget = m_pMapManager->GetNextCameraTarget();
				m_pDefaultCamera->SetFollowingTarget(pTarget);
			}
		}
	}
	else
		isF6Down = false;

	static _bool isF7Down = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_F7))
	{
		if (!isF7Down)
		{
			isF7Down = true;

			m_pMapManager->SetBattleMode();
		}
	}
	else
		isF7Down = false;
}

void Scene3D::AddBGObject(string meshID, vec3 vPos, vec3 vRot, vec3 vScale, int dir)
{
	CLayer* pLayer = GetLayer((_uint)LAYER_BACKGROUND);
	CGameObject* pGameObject = BGObject::Create((_uint)SCENE_3D, pLayer->GetTag(), (_uint)OBJ_BACKGROUND
		, pLayer, meshID, vPos, vRot, vScale, dir);

	if (nullptr == pGameObject)
		return;

	AddGameObjectToLayer(pLayer->GetTag(), pGameObject);
}

Beholder* Scene3D::AddBeholder(vec3 vPos, vec3 vRot, vec3 vScale)
{
	CLayer* pLayer = GetLayer((_uint)LAYER_CHARACTER);
	CGameObject* pGameObject = Beholder::Create((_uint)SCENE_3D, pLayer->GetTag(), (_uint)OBJ_BACKGROUND
		, pLayer, "Beholder", vPos, vRot, vScale);

	if (nullptr == pGameObject)
		return nullptr;

	AddGameObjectToLayer(pLayer->GetTag(), pGameObject);
	return dynamic_cast<Beholder*>(pGameObject);
}

void Scene3D::SetDefaultCameraSavedPosition(vec3 vPos, vec3 vRot, vec3 target)
{
	m_vCameraSavedPos.x = vPos.x;
	m_vCameraSavedPos.y = vPos.y;
	m_vCameraSavedPos.z = vPos.z;

	m_vCameraSavedRot.x = vRot.x;
	m_vCameraSavedRot.y = vRot.y;
	m_vCameraSavedRot.z = vRot.z;

	m_vCameraSavedTarget.x = target.x;
	m_vCameraSavedTarget.y = target.y;
	m_vCameraSavedTarget.z = target.z;
}

void Scene3D::ResetDefaultCameraPos()
{
	if (nullptr != m_pDefaultCamera)
	{
		m_pDefaultCamera->SetCameraEye(m_vCameraSavedPos);
		m_pDefaultCamera->SetCameraRot(m_vCameraSavedRot);
		m_pDefaultCamera->SetCameraTarget(m_vCameraSavedTarget);
	}
}

void Scene3D::Update(const _float& dt)
{
	if (nullptr != m_pSkyBox)
		CRenderer::GetInstance()->AddRenderObj(m_pSkyBox);

	CLightMaster::GetInstance()->SetLightInfo();

	KeyCheck();

	CScene::Update(dt);
}

void Scene3D::Render()
{
	if (nullptr != m_pUIManager)
		m_pUIManager->RenderUI();
}

void Scene3D::Destroy()
{
	SafeDestroy(m_pInputDevice);
	SafeDestroy(m_pUIManager);
	SafeDestroy(m_pSkyBox);
	SafeDestroy(m_pMapManager);

	CScene::Destroy();
}

RESULT Scene3D::Ready()
{
	RESULT result = PK_NOERROR;
	result = ReadyLayerAndGameObject();
	if (PK_NOERROR != result)
		return result;

	// Light
	CComponent* shader = CComponentMaster::GetInstance()->FindComponent("DefaultShader");
	_uint shaderID = 0;
	if (nullptr != shader)
		shaderID = dynamic_cast<CShader*>(shader)->GetShaderProgram();
	CLightMaster::GetInstance()->SetShader(shaderID);
	CLightMaster::GetInstance()->LoadLights(m_DataPath, m_LightListFileName);

	if (nullptr != m_pMapManager)
		m_pMapManager->Ready(this);

	if (nullptr != m_pDefaultCamera)
	{
		m_pDefaultCamera->AddShaderLocation("DefaultShader");
		m_pDefaultCamera->AddShaderLocation("CrystalShader");
		m_pDefaultCamera->AddShaderLocation("WaterShader");
	}

	if (nullptr != m_pUIManager)
		m_pUIManager->Ready();

	if (nullptr == m_pSkyBox)
	{
		stringstream ss, ss2;
		ss << m_DataPath << "Assets\\Texture\\SkyBox\\";

		vector<string> faces;
		ss2.str(""); ss2 << ss.str() << "right.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "left.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "top.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "bottom.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "front.jpg"; faces.push_back(ss2.str());
		ss2.str(""); ss2 << ss.str() << "back.jpg"; faces.push_back(ss2.str());

		CComponent* skyboxShader = CComponentMaster::GetInstance()->FindComponent("SkyBoxShader");
		m_pSkyBox = CSkyBox::Create(faces, dynamic_cast<CShader*>(skyboxShader));
	}

	return PK_NOERROR;
}

RESULT Scene3D::ReadyLayerAndGameObject()
{
	//Create.Camera
	CLayer* pLayer = GetLayer((_uint)LAYER_OBJECT);
	if (nullptr != pLayer)
	{
		vec3 vPos = vec3(0.f, 0.f, 0.f);
		vec3 vRot = vec3(0.f, 0.f, 0.f);
		vec3 vScale = vec3(1.f);
		CGameObject* pGameObject = DefaultCamera::Create((_uint)SCENE_3D, pLayer->GetTag(), (_uint)OBJ_CAMERA, pLayer,
			vPos, vRot, vScale, 0.6f, 0.1f, 1000.f);
		if (nullptr != pGameObject)
		{
			AddGameObjectToLayer(pLayer->GetTag(), pGameObject);
			m_pDefaultCamera = dynamic_cast<DefaultCamera*>(pGameObject);
		}
	}

	//Create.BackgroundLayer 
	LoadBackgroundObjects();

	return PK_NOERROR;
}

Scene3D* Scene3D::Create()
{
	Scene3D* pInstance = new Scene3D();
	if (PK_NOERROR != pInstance->Ready())
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}

void Scene3D::LoadBackgroundObjects()
{
	m_pObjLayer = GetLayer((_uint)LAYER_TARGET);
	CLayer* pLayer = GetLayer((_uint)LAYER_BACKGROUND);
	CGameObject* pGameObject = nullptr;

	if (nullptr != pLayer)
	{
		pLayer->RemoveAllGameObject();
		vector<CXMLParser::sObjectData> vecObjects;
		CXMLParser::sObjectData cameraData;
		CXMLParser::GetInstance()->LoadMapObjectData(m_DataPath, m_ObjListFileName, vecObjects, cameraData);
		vector<CXMLParser::sObjectData>::iterator iter;
 		for (iter = vecObjects.begin(); iter != vecObjects.end(); ++iter)
		{
			pGameObject = BGObject::Create((_uint)SCENE_3D, pLayer->GetTag(), (_uint)OBJ_BACKGROUND, pLayer, iter->ID,
				iter->POSITION, iter->ROTATION, iter->SCALE);
			if (nullptr == pGameObject)
				continue;
			AddGameObjectToLayer(pLayer->GetTag(), pGameObject);
			dynamic_cast<BGObject*>(pGameObject)->SetLock(iter->LOCK);
		}
		vecObjects.clear();

		if (nullptr != m_pDefaultCamera)
		{
			SetDefaultCameraSavedPosition(cameraData.POSITION, cameraData.ROTATION, cameraData.SCALE);
			m_pDefaultCamera->SetCameraEye(cameraData.POSITION);
			m_pDefaultCamera->SetCameraRot(cameraData.ROTATION);
			m_pDefaultCamera->SetCameraTarget(cameraData.SCALE);
		}
	}
}