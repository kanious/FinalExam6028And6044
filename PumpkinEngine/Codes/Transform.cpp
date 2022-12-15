#include "pch.h"
#include "../Headers/Transform.h"
#include "../Headers/Component.h"


USING(Engine)
USING(glm)

CTransform::CTransform()
	: m_matWorld(mat4x4(1.f))
	, m_vPos(vec3(0.f))
	, m_vRot(vec3(0.f))
	, m_vScale(vec3(1.f))
	, m_vRevolve((0.f))
	, m_pParentTransform(nullptr)
{
}

CTransform::CTransform(const CTransform& rhs)
	: m_matWorld(mat4x4(1.f))
	, m_vPos(vec3(0.f))
	, m_vRot(vec3(0.f))
	, m_vScale(vec3(1.f))
	, m_vRevolve((0.f))
	, m_pParentTransform(nullptr)
{
	m_tag = rhs.m_tag;
}

CTransform::~CTransform()
{
}

void CTransform::Update(const _float& dt)
{
 	m_matWorld = mat4x4(1.f);

	m_matWorld = translate(m_matWorld, m_vPos);
	m_matWorld = rotate(m_matWorld, radians(m_vRot.z), vec3(0.f, 0.f, 1.f));
	m_matWorld = rotate(m_matWorld, radians(m_vRot.y), vec3(0.f, 1.f, 0.f));
	m_matWorld = rotate(m_matWorld, radians(m_vRot.x), vec3(1.f, 0.f, 0.f));
	m_matWorld = scale(m_matWorld, m_vScale);

	if (nullptr != m_pParentTransform)
	{
		mat4x4 matRevolve = mat4x4(1.f);
		matRevolve = rotate(matRevolve, radians(m_vRevolve.x), vec3(1.f, 0.f, 0.f));
		matRevolve = rotate(matRevolve, radians(m_vRevolve.y), vec3(0.f, 1.f, 0.f));
		matRevolve = rotate(matRevolve, radians(m_vRevolve.z), vec3(0.f, 0.f, 1.f));
		m_matWorld = matRevolve * m_matWorld;

		const mat4x4* matParent = m_pParentTransform->GetWorldMatrix();
		m_matWorld = *matParent * m_matWorld;
	}
}

void CTransform::Destroy()
{
	CComponent::Destroy();
}

const vec3 CTransform::GetPositionWithParent()
{
	vec3 vPos = m_vPos;
	if (nullptr != m_pParentTransform)
		vPos += m_pParentTransform->GetPositionWithParent();

	return vPos;
}

const vec3 CTransform::GetRotationWithParent()
{
	vec3 vRot = m_vRot;
	if (nullptr != m_pParentTransform)
		vRot += m_pParentTransform->GetRotationWithParent();

	return vRot;
}

const mat4x4 CTransform::GetParentMatrix()
{
	if (nullptr != m_pParentTransform)
		return *m_pParentTransform->GetWorldMatrix();

	return mat4x4(1.f);
}

void CTransform::SetWorldMatrix(mat4x4 matWorld)
{
	m_matWorld = matWorld;
}

void CTransform::SetPosRotScale(vec3 vPos, vec3 vRot, vec3 vScale)
{
	m_vPos = vPos;
	m_vRot = vRot;
	m_vScale = vScale;
}

void CTransform::SetPosition(vec3 vPos)
{
	m_vPos = vPos;
}

void CTransform::SetPositionX(_float x)
{
	m_vPos.x = x;
}

void CTransform::SetPositionY(_float y)
{
	m_vPos.y = y;
}

void CTransform::SetPositionZ(_float z)
{
	m_vPos.z = z;
}

void CTransform::SetRotation(vec3 vRot)
{
	m_vRot = vRot;
}

void CTransform::SetRotationX(_float x)
{
	m_vRot.x = x;
}

void CTransform::SetRotationY(_float y)
{
	m_vRot.y = y;
}

void CTransform::SetRotationZ(_float z)
{
	m_vRot.z = z;
}

void CTransform::SetScale(vec3 vScale)
{
	m_vScale = vScale;
}

void CTransform::SetScaleX(_float x)
{
	m_vScale.x = x;
}

void CTransform::SetScaleY(_float y)
{
	m_vScale.y = y;
}

void CTransform::SetScaleZ(_float z)
{
	m_vScale.z = z;
}

void CTransform::SetRevolve(vec3 vRevolve)
{
	m_vRevolve = vRevolve;
}

void CTransform::SetParent(CTransform* pParent)
{
	m_pParentTransform = pParent;
}

void CTransform::AddPosition(vec3 vPos)
{
	m_vPos += vPos;
}

void CTransform::AddPositionX(_float x)
{
	m_vPos.x += x;
}

void CTransform::AddPositionY(_float y)
{
	m_vPos.y += y;
}

void CTransform::AddPositionZ(_float z)
{
	m_vPos.z += z;
}

void CTransform::AddRotation(vec3 vRot)
{
	m_vRot += vRot;
}

void CTransform::AddRotationX(_float x)
{
	m_vRot.x += x;
}

void CTransform::AddRotationY(_float y)
{
	m_vRot.y += y;
}

void CTransform::AddRotationZ(_float z)
{
	m_vRot.z += z;
}

void CTransform::AddScale(vec3 vScale)
{
	m_vScale += vScale;
}

void CTransform::AddRevolve(vec3 vRevolve)
{
	m_vRevolve += vRevolve;
}

void CTransform::ComputeWorldMatrix(glm::mat4x4& matWorld, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale)
{
	matWorld = translate(matWorld, m_vPos + vPos);
	matWorld = rotate(matWorld, radians(m_vRot.z + vRot.z), vec3(0.f, 0.f, 1.f));
	matWorld = rotate(matWorld, radians(m_vRot.y + vRot.y), vec3(0.f, 1.f, 0.f));
	matWorld = rotate(matWorld, radians(m_vRot.x + vRot.x), vec3(1.f, 0.f, 0.f));
	matWorld = scale(matWorld, m_vScale + vScale);
}

RESULT CTransform::Ready()
{
	m_tag = "Transform";

	return PK_NOERROR;
}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

CTransform* CTransform::Create()
{
	CTransform* pInstance = new CTransform;
	if (PK_NOERROR != pInstance->Ready())
	{
		pInstance->Destroy();;
		pInstance = nullptr;
	}

	return pInstance;
}
