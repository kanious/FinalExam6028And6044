#include "pch.h"
#include "..\Headers\Camera.h"
#include "..\Headers\Component.h"
#include "..\Headers\OpenGLDefines.h"
#include "..\Headers\OpenGLDevice.h"

USING(Engine)
USING(glm)

CCamera::CCamera()
{
	m_vEye = vec3(0.f);
	m_vTarget = vec3(0.f);
	m_vUp = vec3(0.f);
	m_matView = mat4x4(1.f);

	m_fFov = 0.f;
	m_fRatio = 0.f;
	m_fNear = 0.f;
	m_fFar = 0.f;
	m_matProj = mat4x4(1.f);
}

CCamera::CCamera(const CCamera& rhs)
	: m_vEye(rhs.m_vEye), m_vTarget(rhs.m_vTarget), m_vUp(rhs.m_vUp), m_matView(rhs.m_matView)
	, m_fFov(rhs.m_fFov), m_fRatio(rhs.m_fRatio), m_fNear(rhs.m_fNear), m_fFar(rhs.m_fFar)
	, m_matProj(rhs.m_matProj)
{
	m_tag = rhs.m_tag;
}

CCamera::~CCamera()
{
}

void CCamera::Update(const _float& dt)
{
	CalculateViewMatrix();
	SetRatio();
	CalculateProjMatrix();

	COpenGLDevice::GetInstance()->SetViewMatrix(m_matView);
	COpenGLDevice::GetInstance()->SetProjMatrix(m_matProj);
}

void CCamera::Destroy()
{
	CComponent::Destroy();
}

void CCamera::SetCameraEye(vec3 vEye)
{
	m_vEye.x = vEye.x;
	m_vEye.y = vEye.y;
	m_vEye.z = vEye.z;
}

void CCamera::SetCameraTarget(vec3 vTarget)
{
	m_vTarget.x = vTarget.x;
	m_vTarget.y = vTarget.y;
	m_vTarget.z = vTarget.z;
}

void CCamera::SetCameraUp(vec3 vUp)
{
	m_vUp.x = vUp.x;
	m_vUp.y = vUp.y;
	m_vUp.z = vUp.z;
}

void CCamera::SetFieldOfView(_float fov)
{
	m_fFov = fov;
}

void CCamera::SetRatio()
{
	if (0 == COpenGLDevice::GetInstance()->GetWidthSize() || 0 == COpenGLDevice::GetInstance()->GetHeightSize())
		m_fRatio = 0.f;
	else
		m_fRatio = COpenGLDevice::GetInstance()->GetWidthSize() / (_float)COpenGLDevice::GetInstance()->GetHeightSize();
}

void CCamera::SetNear(_float fNear)
{
	m_fNear = fNear;
}

void CCamera::SetFar(_float fFar)
{
	m_fFar = fFar;
}

void CCamera::CalculateViewMatrix()
{
	m_matView = lookAt(m_vEye, m_vTarget, m_vUp);
}

void CCamera::CalculateProjMatrix()
{
	m_matProj = perspective(m_fFov, m_fRatio, m_fNear, m_fFar);
}

RESULT CCamera::Ready()
{
	m_tag = "Camera";

	return PK_NOERROR;
}

CComponent* CCamera::Clone()
{
	return new CCamera(*this);
}

CCamera* CCamera::Create()
{
	CCamera* pInstance = new CCamera;
	if (PK_NOERROR != pInstance->Ready())
	{
		pInstance->Destroy();;
		pInstance = nullptr;
	}

	return pInstance;
}

void CCamera::InitCameraSetting(glm::vec3 vEye, glm::vec3 vTarget, glm::vec3 vUp, _float fov, _float fNear, _float fFar)
{
	m_vEye = vEye;
	m_vTarget = vTarget;
	m_vUp = vUp;
	m_fFov = fov;
	SetRatio();
	m_fNear = fNear;
	m_fFar = fFar;

	CalculateViewMatrix();
	CalculateProjMatrix();
}