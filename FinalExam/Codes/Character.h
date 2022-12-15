#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "GameObject.h"
#include "EngineStruct.h"

namespace Engine
{
	class CMesh;
	class COctree;
	class CInputDevice;
}
class BGObject;

class Character : public Engine::CGameObject
{
private:
	Engine::CMesh*					m_pMesh;
	Engine::CInputDevice*			m_pInputDevice;

	glm::vec3						m_vVelocity;
	glm::vec3						m_vAccel;
	glm::vec3						m_vForce;
	glm::vec3						m_vRotForce;

	BGObject*						m_pTerrain;
	Engine::COctree*				m_pTerrainOctree;

	_bool							m_bCollision;
	_bool							m_bMove;
	_float							m_fSpeed;
	_float							m_fRotSpeed;

private:
	explicit Character();
	virtual ~Character();

public:
	void SetTerrain(BGObject* terrain);
	Engine::TRIANGLE* GetTrianglesFromMesh();
	_uint GetTriangleNumber();
	Engine::COctree* GetOctreeFromMesh();
	void AddForce(glm::vec3 force)		{ m_vForce += force; }

private:
	void KeyCheck(const _float& dt);
	_bool CollisionCheck();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
private:
	virtual void Destroy();
	RESULT Ready(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale, std::string soundTag);
public:
	static Character* Create(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale, std::string soundTag);
};

#endif //_CHARACTER_H_