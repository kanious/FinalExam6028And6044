#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "Scene.h"
#include "glm\vec3.hpp"

namespace Engine
{
	class CInputDevice;
	class CLayer;
	class CSkyBox;
}

class UIManager;
class DefaultCamera;
class BGObject;
class MapManager;
class Beholder;

class Scene3D : public Engine::CScene
{
private:
	Engine::CInputDevice*		m_pInputDevice;
	Engine::CLayer*				m_pObjLayer;
	Engine::CSkyBox*			m_pSkyBox;
	UIManager*					m_pUIManager;
	MapManager*					m_pMapManager;
	DefaultCamera*				m_pDefaultCamera;

	glm::vec3					m_vCameraSavedPos;
	glm::vec3					m_vCameraSavedRot;
	glm::vec3					m_vCameraSavedTarget;

	_bool						m_bFollowingMode;

private:
	explicit Scene3D();
	virtual ~Scene3D();
	
private:
	void KeyCheck();
public:
	void AddBGObject(std::string meshID, glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale, int dir = 0);
	Beholder* AddBeholder(glm::vec3 vPos, glm::vec3 vRot, glm::vec3 vScale);

private:
	void SetDefaultCameraSavedPosition(glm::vec3 vPos, glm::vec3 vRot, glm::vec3 target);
	void ResetDefaultCameraPos();

public:
	virtual void Update(const _float& dt);
	virtual void Render();
private:
	virtual void Destroy();

private:
	RESULT Ready();
	RESULT ReadyLayerAndGameObject();
public:
	static Scene3D* Create();
	virtual void LoadBackgroundObjects();
};

#endif //_SCENE3D_H_