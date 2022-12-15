#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Base.h"
#include "glm\vec3.hpp"
#include "glm\mat4x4.hpp"

NAMESPACE_BEGIN(Engine)

class CComponent;
class CRenderer;
class CLayer;
class CTransform;
class CBoundingBox;

class ENGINE_API CGameObject : public CBase
{
protected:
	_wchar_t*							m_UUID;
	std::string							m_meshName;
	std::string							m_objName;
	_uint								m_objTag;
	_uint								m_layerTag;
	_uint								m_sceneTag;
	_bool								m_bEnable;
	_bool								m_bDead;
	_bool								m_bLock;
	_bool								m_bDebug;
	_bool								m_bWireFrame;

	typedef std::unordered_map<std::string, CComponent*> COMPONENT_MAP;
	COMPONENT_MAP						m_mapComponent;
	CRenderer*							m_pRenderer;
	CLayer*								m_pLayer;
	CTransform*							m_pTransform;
	CBoundingBox*						m_pBoundingBox;

protected:
	explicit CGameObject();
	virtual ~CGameObject();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
	void Enable();
	void Disable();
protected:
	virtual void Destroy();

public:
	void SetupGameObject(_uint sceneTag, _uint layerTag, _uint objTag);
	const _wchar_t* GetUUID()				{ return m_UUID; }
	const std::string GetMeshName()			{ return m_meshName; }
	const std::string GetObjName()			{ return m_objName; }
	const _uint GetObjectTag()				{ return m_objTag; }
	const _uint GetLayerTag()				{ return m_layerTag; }
	const _uint GetSceneTag()				{ return m_sceneTag; }
	const _bool GetEnable()					{ return m_bEnable; }
	const _bool GetDead()					{ return m_bDead; }
	const _bool GetLock()					{ return m_bLock; }
	const _bool GetDebug()					{ return m_bDebug; }
	const _bool GetWireFrame()				{ return m_bWireFrame; }
	CBoundingBox* GetBoundingBox()			{ return m_pBoundingBox; }
	CTransform* GetTransform()				{ return m_pTransform; }
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	_float GetRotationY();
	glm::vec3 GetScale();
	const glm::mat4x4* GetWorldMatrix();
	void SetMeshName(std::string name);
	void SetObjectName(std::string name);
	void SetObjectTag(_uint objTag);
	void SetLayerTag(_uint layerTag);
	void SetSceneTag(_uint sceneTag);
	void SetParentLayer(CLayer* pLayer);
	void SetEnable(_bool enable)			{ m_bEnable = enable; }
	void SetDead(_bool dead);
	void SetLock(_bool lock)				{ m_bLock = lock; }
	void SetDebug(_bool debug)				{ m_bDebug = debug; }
	void SetWireFrame(_bool wire)			{ m_bWireFrame = wire; }
	void SetPosition(glm::vec3 vPos);
	void SetRotation(glm::vec3 vRot);
	void SetRotationX(_float fAngle);
	void SetRotationY(_float fAngle);
	void SetRotationZ(_float fAngle);
	void SetScale(glm::vec3 vScale);
	void SetParentTransform(CTransform* pTransform);

public:
	void AttachComponent(std::string componentTag, CComponent* pInstance);
	RESULT RemoveComponent(std::string componentTag);
	CComponent* GetComponent(std::string componentTag);

private:
	_wchar_t* UUIDGenerate();
};

NAMESPACE_END

#endif //_GAMEOBJECT_H_