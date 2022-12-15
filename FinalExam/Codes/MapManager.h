#ifndef _MAPMANAGER_H_
#define _MAPMANAGER_H_

#include "Base.h"
#include "glm\vec3.hpp"

class Scene3D;
class Blocks;
class Rooms;
class Beholder;
class MapManager : public Engine::CBase
{
	SINGLETON(MapManager)

private:
	struct sBeholderInfo
	{
		Beholder* beholder;
		_uint curRoamingIndex;
	};

private:
	Scene3D*						m_pScene;
	std::vector<Blocks*>			m_vecBlocks;
	std::vector<Rooms*>				m_vecRooms;
	std::vector<sBeholderInfo*>		m_vecBeholders;

	_uint							m_iCurCameraTarget;
	_bool							m_bBattleMode;

private:
	explicit MapManager();
	~MapManager();
public:
	void Destroy();
	RESULT Ready(Scene3D* pScene);

public:
	Beholder* GetNextCameraTarget();
	void ResetCameraTarget();
	glm::vec3 GetNextTargetPos(_uint beholderIdx);
	void SetBattleMode();
	Beholder* GetLockOnTarget(Beholder* me);
	void KillEnemy(Beholder* target);
	void RemoveBeholder();
	glm::vec3 GetNextPathPos(Beholder* me, Beholder* target);

private:
	void SetMapInformation();
	void AddRandomTorchAndCrystal();
	void CreateBackgroundObjects();
	void CreateBeholder();
	void CreateTorchAndCrystal();
	void CreateWater();
	std::string GetRandomWall();
	std::string GetRandomFloor();
	std::string GetRandomTorch();
};

#endif //_MAPMANAGER_H_