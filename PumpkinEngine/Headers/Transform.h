#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Component.h"
#include "OpenGLDefines.h"

NAMESPACE_BEGIN(Engine)

class ENGINE_API CTransform : public CComponent
{
private:
	glm::mat4x4				m_matWorld;
	glm::vec3				m_vPos;
	glm::vec3				m_vRot;
	glm::vec3				m_vScale;
	glm::vec3				m_vRevolve;
	CTransform*				m_pParentTransform;

private:
	explicit CTransform();
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform();
public:
	virtual void Update(const _float& dt);
private:
	virtual void Destroy();

public:
	const glm::mat4x4* GetWorldMatrix()				{ return &m_matWorld; }
	const glm::vec3 GetPosition()					{ return m_vPos; }
	const glm::vec3 GetPositionWithParent();
	const _float GetPositionX()						{ return m_vPos.x; }
	const _float GetPositionY()						{ return m_vPos.y; }
	const _float GetPositionZ()						{ return m_vPos.z; }
	const glm::vec3 GetRotation()					{ return m_vRot; }
	const glm::vec3 GetRotationWithParent();
	const _float GetRotationX()						{ return m_vRot.x; }
	const _float GetRotationY()						{ return m_vRot.y; }
	const _float GetRotationZ()						{ return m_vRot.z; }
	const glm::vec3 GetScale()						{ return m_vScale; }
	const glm::vec3 GetRevolve()					{ return m_vRevolve; }
	const CTransform* GetParent()					{ return m_pParentTransform; }
	const glm::mat4x4 GetParentMatrix();
	const glm::vec3 GetRightVector()				{ return normalize(m_matWorld[0]); }
	const glm::vec3 GetUpVector()					{ return normalize(m_matWorld[1]); }
	const glm::vec3 GetLookVector()					{ return normalize(m_matWorld[2]); }
	const glm::vec3 GetPositionInWorldMatrix()		{ return m_matWorld[3]; }
	void SetWorldMatrix(glm::mat4x4 matWorld);
	void SetPosRotScale(glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);
	void SetPosition(glm::vec3 vPos);
	void SetPositionX(_float x);
	void SetPositionY(_float y);
	void SetPositionZ(_float z);
	void SetRotation(glm::vec3 vRot);
	void SetRotationX(_float x);
	void SetRotationY(_float y);
	void SetRotationZ(_float z);
	void SetScale(glm::vec3 vScale);
	void SetScaleX(_float x);
	void SetScaleY(_float y);
	void SetScaleZ(_float z);
	void SetRevolve(glm::vec3 vRevolve);
	void SetParent(CTransform* pParent);
	void AddPosition(glm::vec3 vPos);
	void AddPositionX(_float x);
	void AddPositionY(_float y);
	void AddPositionZ(_float z);
	void AddRotation(glm::vec3 vRot);
	void AddRotationX(_float x);
	void AddRotationY(_float y);
	void AddRotationZ(_float z);
	void AddScale(glm::vec3 vScale);
	void AddRevolve(glm::vec3 vRevolve);
public:
	void ComputeWorldMatrix(glm::mat4x4& matWorld, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);

private:
	RESULT Ready();
public:
	virtual CComponent* Clone();
	static CTransform* Create();
};

NAMESPACE_END

#endif //_TRANSFORM_H_