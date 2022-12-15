#ifndef _BGOBJECT_H_
#define _BGOBJECT_H_

#include "GameObject.h"
#include "EngineStruct.h"

namespace Engine
{
	class CMesh;
}
class TorchEffect;

class BGObject : public Engine::CGameObject
{
private:
	Engine::CMesh*					m_pMesh;
	TorchEffect*					m_pTorchObj;
	_bool							m_bTransparent;

private:
	explicit BGObject();
	virtual ~BGObject();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
private:
	virtual void Destroy();
	RESULT Ready(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale, int dir);
public:
	static BGObject* Create(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale, int dir = 0);
};

#endif //_BGOBJECT_H_