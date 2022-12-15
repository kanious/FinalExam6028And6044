#ifndef _BEHOLDER_H_
#define _BEHOLDER_H_

#include "GameObject.h"
#include "EngineStruct.h"
#include "Light.h"

namespace Engine
{
	class CMesh;
	class CShader;
}
class BGObject;
class MapManager;

class Beholder : public Engine::CGameObject
{
	enum eMode { ROAMING, BATTLE };
	enum eState { ROTATING, MOVING, ARRIVED };

private:
	Engine::CMesh*					m_pMesh;
	Engine::CLight::cLightInfo*		m_pLightInfo[3];
	MapManager*						m_pMapManager;
	BGObject*						m_pConeObj;

	eMode							m_eMode;
	eState							m_eState;

	_uint							m_iIndex;
	_uint							m_iRoomIdx;
	glm::vec3						m_vTargetPos;
	glm::vec3						m_vTargetDir;

	_float							m_fRotSpeed;
	_float							m_fMoveSpeed;
	_float							m_fElaspedTime;

	glm::vec3						m_vOriginPos;
	glm::vec3						m_vOriginRot;

	Beholder*						m_pTarget;
	_bool							m_bDeathEffect;
	_bool							m_bGameEnd;


private:
	explicit Beholder();
	virtual ~Beholder();

public:
	_uint GetBeholderId()	{ return m_iIndex; }
	_uint GetRoomId()		{ return m_iRoomIdx; }
	_bool GetDeadEffect()	{ return m_bDeathEffect; }
	void SetBeholderIndex(_uint index, _uint roomIndex);
	void SetBattleMode(_bool value);
	void DeadByEnemy();
	void SetGameEnd(_bool value) { m_bGameEnd = value; }

private:
	void Roaming(const _float& dt);
	void Battle(const _float& dt);
	void RunDeathEffect(const _float& dt);
	void UpdateLightInfo();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
private:
	virtual void Destroy();
	RESULT Ready(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);
public:
	static Beholder* Create(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);

private:
	_float safe_acos(_float value);
};

#endif //_BEHOLDER_H_