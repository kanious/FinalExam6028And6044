#ifndef _TORCHEFFECT_H_
#define _TORCHEFFECT_H_

#include "GameObject.h"
#include "EngineStruct.h"

namespace Engine
{
	class CMesh;
	class CShader;
}
class TorchEffect : public Engine::CGameObject
{
private:
	Engine::CMesh*					m_pMesh;
	Engine::CShader*				m_pShader;
	_float							m_fFrameIndex;

private:
	explicit TorchEffect();
	virtual ~TorchEffect();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
private:
	virtual void Destroy();
	RESULT Ready(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);
public:
	static TorchEffect* Create(_uint sTag, _uint lTag, _uint oTag, Engine::CLayer* pLayer, std::string meshID,
		glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);
};

#endif //_TORCHEFFECT_H_