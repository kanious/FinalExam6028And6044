#include "DefaultCamera.h"
#include "ComponentMaster.h"
#include "Layer.h"

#include "Component.h"
#include "Camera.h"
#include "Transform.h"
#include "OpenGLDefines.h"
#include "InputDevice.h"
#include "Shader.h"
#include "ComponentMaster.h"
#include "Component.h"
#include "Beholder.h"

USING(Engine)
USING(glm)
USING(std)

DefaultCamera::DefaultCamera()
	: m_pCamera(nullptr), m_bMouseEnable(true), m_fAngleY(0.f), m_fAngleX(0.f)
	, m_fSpeed(20.f), m_fRotSpeed(20.f), m_pTarget(nullptr)
{
	m_pInputDevice = CInputDevice::GetInstance(); m_pInputDevice->AddRefCnt();
}

DefaultCamera::~DefaultCamera()
{
}

vec3 DefaultCamera::GetCameraEye()
{
	if (nullptr == m_pCamera)
		return vec3(0.f);

	return m_pCamera->GetCameraEye();
}

vec3 DefaultCamera::GetCameraRot()
{
	if (nullptr == m_pTransform)
		return vec3(0.f);

	return m_pTransform->GetRotation();
}

vec3 DefaultCamera::GetCameraTarget()
{
	if (nullptr == m_pCamera)
		return vec3(0.f);

	return m_pCamera->GetCameraTarget();
}

void DefaultCamera::SetMouseEnable(_bool enable)
{
	m_bMouseEnable = enable;
	if (enable)
		m_pInputDevice->SetMouseCursorMode(GLFW_CURSOR_NORMAL);
	else
		m_pInputDevice->SetMouseCursorMode(GLFW_CURSOR_DISABLED);

	m_pInputDevice->InitMousePos();
}

void DefaultCamera::SetCameraEye(glm::vec3 eye)
{
	if (nullptr == m_pCamera || nullptr == m_pTransform || nullptr == m_pInputDevice)
		return;

	m_pTransform->SetPosition(eye);
	m_pCamera->SetCameraEye(eye);
}

void DefaultCamera::SetCameraRot(glm::vec3 rot)
{
	if (nullptr == m_pTransform)
		return;

	m_pTransform->SetRotation(rot);
}

void DefaultCamera::SetCameraTarget(glm::vec3 target)
{
	if (nullptr == m_pCamera || nullptr == m_pTransform || nullptr == m_pInputDevice)
		return;

	vec3 vEye = m_pCamera->GetCameraEye();
	vec3 vDir = target - vEye;
	vDir = normalize(vDir);
	m_pCamera->SetCameraTarget(vEye + vDir);
	m_pInputDevice->InitMousePos();

	m_fAngleX = m_pTransform->GetRotationX();
	m_fAngleY = m_pTransform->GetRotationY();
}

void DefaultCamera::AddShaderLocation(string shaderTag)
{
	CComponent* shader = CComponentMaster::GetInstance()->FindComponent(shaderTag);
	if (nullptr != shader)
	{
		_uint shaderID = dynamic_cast<CShader*>(shader)->GetShaderProgram();
		_uint shaderLocation = glGetUniformLocation(shaderID, "eyeLocation");
		m_vecShaders.push_back(shaderLocation);
	}
}

void DefaultCamera::KeyCheck(const _float& dt)
{
	if (nullptr == m_pInputDevice || nullptr == m_pTransform)
		return;

	if (!m_bMouseEnable)
	{
		vec2 vMousePos = m_pInputDevice->GetMouseMovedDistance();
		m_fAngleX += vMousePos.y;
		m_fAngleY -= vMousePos.x;

		if (m_fAngleX > 89.f)
			m_fAngleX = 89.f;
		if (m_fAngleX < -89.f)
			m_fAngleX = -89.f;

		if (m_fAngleY > 360.f)
			m_fAngleY -= 360.f;
		if (m_fAngleY < 0)
			m_fAngleY += 360.f;

		m_pTransform->SetRotationX(m_fAngleX);
		m_pTransform->SetRotationY(m_fAngleY);

		m_pTransform->Update(0);
		vec3 vDir = m_pTransform->GetLookVector();

		m_pCamera->SetCameraTarget(m_pTransform->GetPosition() + vDir * 10.f);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_W))
	{
		vec3 vDir = m_pTransform->GetLookVector();
		m_pTransform->AddPosition(vDir * dt * 20.f);

		m_pCamera->SetCameraEye(m_pTransform->GetPosition());
		m_pCamera->SetCameraTarget(m_pTransform->GetPosition() + vDir * 10.f);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_S))
	{
		vec3 vDir = m_pTransform->GetLookVector();
		m_pTransform->AddPosition(vDir * dt * -20.f);

		m_pCamera->SetCameraEye(m_pTransform->GetPosition());
		m_pCamera->SetCameraTarget(m_pTransform->GetPosition() + vDir * 10.f);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_A))
	{
		vec3 vRight = m_pTransform->GetRightVector();
		m_pTransform->AddPosition(vRight * dt * 20.f);

		m_pCamera->SetCameraEye(m_pTransform->GetPosition());
		m_pCamera->SetCameraTarget(m_pTransform->GetPosition() + m_pTransform->GetLookVector() * 10.f);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_D))
	{
		vec3 vRight = m_pTransform->GetRightVector();
		m_pTransform->AddPosition(vRight * dt * -20.f);

		m_pCamera->SetCameraEye(m_pTransform->GetPosition());
		m_pCamera->SetCameraTarget(m_pTransform->GetPosition() + m_pTransform->GetLookVector() * 10.f);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		vec3 vUp = vec3(0.f, 1.f, 0.f);
		m_pTransform->AddPosition(vUp * dt * -20.f);

		m_pCamera->SetCameraEye(m_pTransform->GetPosition());
		m_pCamera->SetCameraTarget(m_pTransform->GetPosition() + m_pTransform->GetLookVector() * 10.f);
	}

	if (m_pInputDevice->IsKeyDown(GLFW_KEY_SPACE))
	{
		vec3 vUp = vec3(0.f, -1.f, 0.f);
		m_pTransform->AddPosition(vUp * dt * -20.f);

		m_pCamera->SetCameraEye(m_pTransform->GetPosition());
		m_pCamera->SetCameraTarget(m_pTransform->GetPosition() + m_pTransform->GetLookVector() * 10.f);
	}

	static _bool isLeftAltDown = false;
	if (m_pInputDevice->IsKeyDown(GLFW_KEY_LEFT_ALT))
	{
		if (!isLeftAltDown)
		{
			isLeftAltDown = true;
			
			SetMouseEnable(!m_bMouseEnable);
		}
	}
	else
		isLeftAltDown = false;
}

void DefaultCamera::FollowTarget(const _float& dt)
{
	if (nullptr == m_pTarget)
		return;

	vec3 vPos = m_pTarget->GetPosition();

	CTransform* pTransform = m_pTarget->GetTransform();
	vec3 vLook = pTransform->GetLookVector();
	vPos -= vLook * 15.f;
	vec3 vUp = pTransform->GetUpVector();
	vPos += vUp * 15.f;
	vec3 vCameraPos = m_pCamera->GetCameraEye();

	vec3 vOriginTargetDir(0.f);
	_float dist = distance(vPos, vCameraPos);
	if (1.f < dist)
	{
		vec3 vDir = vPos - vCameraPos;
		vDir = normalize(vDir);

		vec3 vTargetPos = m_pCamera->GetCameraTarget();
		vOriginTargetDir = vTargetPos - vCameraPos;

		vCameraPos += vDir * dt * dist;
		m_pCamera->SetCameraEye(vCameraPos);
	}

	vec3 vTargetPos = vCameraPos + vOriginTargetDir;
	_float dist2 = distance(vTargetPos, m_pTarget->GetPosition());

	if (0.1f < dist2)
	{
		vec3 vTargetDir = m_pTarget->GetPosition() - vTargetPos;
		vTargetDir = normalize(vTargetDir);
		vTargetPos += vTargetDir * dt * dist2;
		m_pCamera->SetCameraTarget(vTargetPos);
	}

}

void DefaultCamera::Update(const _float& dt)
{
	if (m_bFollowingMonster)
		FollowTarget(dt);
	else
		KeyCheck(dt);

	CGameObject::Update(dt);

	vec3 vEye = m_pCamera->GetCameraEye();

	for (int i = 0; i < m_vecShaders.size(); ++i)
		glUniform4f(m_vecShaders[i], vEye.x, vEye.y, vEye.z, 1.0f);
}

void DefaultCamera::Destroy()
{
	SafeDestroy(m_pInputDevice);

	CGameObject::Destroy();
}

RESULT DefaultCamera::Ready(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, vec3 vPos, vec3 vRot, vec3 vScale, _float fov, _float fNear, _float fFar)
{
	SetupGameObject(sTag, lTag, oTag);
	m_pLayer = pLayer;

	CComponentMaster* pMaster = CComponentMaster::GetInstance();
	CComponent* pComponent = nullptr;

	//Clone.Camera
	m_pCamera = CloneComponent<CCamera*>("Camera");
	AttachComponent("Camera", m_pCamera);

	if (nullptr != m_pTransform)
	{
		m_pTransform->SetPosition(vPos);
		m_pTransform->SetRotation(vRot);
		m_pTransform->SetScale(vScale);
		m_pTransform->Update(0);

		if (nullptr != m_pCamera)
			m_pCamera->InitCameraSetting(m_pTransform->GetPosition(), m_pTransform->GetLookVector(), vec3(0.f, 1.f, 0.f), fov, fNear, fFar);
	}
	return PK_NOERROR;
}

DefaultCamera* DefaultCamera::Create(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, vec3 vPos, vec3 vRot, vec3 vScale, _float fov, _float fNear, _float fFar)
{
	DefaultCamera* pInstance = new DefaultCamera();
	if (PK_NOERROR != pInstance->Ready(sTag, lTag, oTag, pLayer, vPos, vRot, vScale, fov, fNear, fFar))
	{
		pInstance->Destroy();
		pInstance = nullptr;
	}

	return pInstance;
}