#include "MapManager.h"
#include "OpenGLDefines.h"
#include "Define.h"
#include "Scene3D.h"
#include "Rooms.h"
#include "Blocks.h"
#include "Function.h"
#include "Beholder.h"

SINGLETON_FUNCTION(MapManager)
USING(Engine)
USING(glm)
USING(std)

#define WIDTH 33
#define HEIGHT 16
#define TOTAL WIDTH*HEIGHT

MapManager::MapManager()
	: m_pScene(nullptr), m_iCurCameraTarget(0), m_bBattleMode(false)
{
	m_vecBlocks.clear();
	m_vecRooms.clear();
	m_vecBeholders.clear();
}

MapManager::~MapManager()
{
}

void MapManager::Destroy()
{
	for (int i = 0; i < TOTAL; ++i)
		delete m_vecBlocks[i];
	m_vecBlocks.clear();

	for (int i = 0; i < m_vecRooms.size(); ++i)
		delete m_vecRooms[i];
	m_vecRooms.clear();

	for (int i = 0; i < m_vecBeholders.size(); ++i)
		delete m_vecBeholders[i];
	m_vecBeholders.clear();
}

RESULT MapManager::Ready(Scene3D* pScene)
{
	if (nullptr == pScene)
		return PK_ERROR_NULLPTR;

	m_pScene = pScene;

	m_vecBlocks.resize(TOTAL);
	for (int i = 0; i < TOTAL; ++i)
	{
		m_vecBlocks[i] = new Blocks();
		m_vecBlocks[i]->Ready(i, vec3(i % WIDTH * 5.f, 0.f, i / WIDTH * 5.f));
	}

	SetMapInformation();
	AddRandomTorchAndCrystal();

	CreateBackgroundObjects();
	CreateBeholder();
	CreateTorchAndCrystal();
	CreateWater();
	return PK_NOERROR;
}

Beholder* MapManager::GetNextCameraTarget()
{
	sBeholderInfo* pInfo = m_vecBeholders[m_iCurCameraTarget];
	Beholder* target = pInfo->beholder;

	++m_iCurCameraTarget;
	if (m_iCurCameraTarget >= m_vecBeholders.size())
		m_iCurCameraTarget -= m_vecBeholders.size();

	return target; 
}

void MapManager::ResetCameraTarget()
{
	m_iCurCameraTarget = 0;
}

vec3 MapManager::GetNextTargetPos(_uint beholderIdx)
{
	sBeholderInfo* info = m_vecBeholders[beholderIdx];

	_uint roomIdx = info->beholder->GetRoomId();
	Rooms* pRoom = m_vecRooms[roomIdx];
	
	vec3 newPos = pRoom->m_vCornerPos[info->curRoamingIndex];

	++info->curRoamingIndex;
	if (4 <= info->curRoamingIndex)
		info->curRoamingIndex -= 4;

	return newPos;
}

void MapManager::SetBattleMode()
{
	m_bBattleMode = !m_bBattleMode;

	for (int i = 0; i < m_vecBeholders.size(); ++i)
	{
		m_vecBeholders[i]->beholder->SetBattleMode(m_bBattleMode);
	}
}

Beholder* MapManager::GetLockOnTarget(Beholder* me)
{
	for (int i = 0; i < m_vecBeholders.size(); ++i)
	{
		Beholder* target = m_vecBeholders[i]->beholder;
		if (target->GetBeholderId() == me->GetBeholderId())
			continue;

		if (!target->GetDeadEffect())
			return target; 
	}

	return nullptr;
}

void MapManager::KillEnemy(Beholder* target)
{
	target->DeadByEnemy();
}

void MapManager::RemoveBeholder()
{
	_uint iCount = 0;
	for (int i = 0; i < m_vecBeholders.size(); ++i)
	{
		if (m_vecBeholders[i]->beholder->GetEnable())
			++iCount;
	}
	if (1 >= iCount)
	{
		for (int i = 0; i < m_vecBeholders.size(); ++i)
 			m_vecBeholders[i]->beholder->SetGameEnd(true);
	}
}

vec3 MapManager::GetNextPathPos(Beholder* me, Beholder* target) 
{
	if (nullptr == me || nullptr == target)
		return vec3(0.f);

	vec3 vPos = me->GetPosition();
	vPos.y = 0.f;
	vec3 vTargetPos = target->GetPosition();
	vTargetPos.y = 0.f;

	Rooms* pRoom = nullptr;
	for (int i = 0; i < m_vecRooms.size(); ++i)
	{
		if (m_vecRooms[i]->IsInside(vPos))
		{
			pRoom = m_vecRooms[i];
			break;
		}
	}

	Blocks* pClosestBlock = pRoom->GetClosestEscape(vTargetPos);

	_float dist = distance(vPos, pClosestBlock->m_vCenterPos);
	if (0.5f < dist)
		return pClosestBlock->m_vCenterPos;

	// Search Path
	_int dir = pClosestBlock->m_iDir;
	_uint iDestIdx = 0;
	switch (dir)
	{
	case 0:
		iDestIdx = pClosestBlock->m_iIndex - WIDTH - WIDTH;
		break;

	case 1:
		iDestIdx = pClosestBlock->m_iIndex - 2;
		break;
		
	case 2:
		iDestIdx = pClosestBlock->m_iIndex + 2;
		break;

	case 3:
		iDestIdx = pClosestBlock->m_iIndex + WIDTH + WIDTH;
		break;
	}
	Blocks* pDest = m_vecBlocks[iDestIdx];
	return pDest->m_vCenterPos;
}

void MapManager::SetMapInformation()
{
	// Set Pass
	vector<_uint> vecPassage;
	_uint passIdx = 0;
	passIdx = 2 * WIDTH + 4; vecPassage.push_back(passIdx);
	passIdx = 12 * WIDTH + 4; vecPassage.push_back(passIdx);
	passIdx = 4 * WIDTH + 7; vecPassage.push_back(passIdx);
	passIdx = 10 * WIDTH + 7; vecPassage.push_back(passIdx);
	passIdx = 7 * WIDTH + 12; vecPassage.push_back(passIdx);
	passIdx = 6 * WIDTH + 16; vecPassage.push_back(passIdx);
	passIdx = 12 * WIDTH + 18; vecPassage.push_back(passIdx);
	passIdx = 9 * WIDTH + 19; vecPassage.push_back(passIdx);
	passIdx = 9 * WIDTH + 25; vecPassage.push_back(passIdx);
	passIdx = 6 * WIDTH + 26; vecPassage.push_back(passIdx);
	passIdx = 12 * WIDTH + 26; vecPassage.push_back(passIdx);

	for (int i = 0; i < vecPassage.size(); ++i)
	{
		m_vecBlocks[vecPassage[i]]->m_bIsExist = true;
		m_vecBlocks[vecPassage[i]]->m_bPath = true;
	}

	// Set Rooms
	Rooms* pRoom = nullptr;
	pRoom = new Rooms(); passIdx = 0 * WIDTH + 0; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 4, 5, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 10 * WIDTH + 0; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 4, 5, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 0 * WIDTH + 5; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 5, 4, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 11 * WIDTH + 5; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 5, 4, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 5 * WIDTH + 6; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 6, 5, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 7 * WIDTH + 13; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 6, 5, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 2 * WIDTH + 15; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 3, 4, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 13 * WIDTH + 17; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 3, 3, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 9 * WIDTH + 20; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 5, 3, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 3 * WIDTH + 25; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 3, 3, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 13 * WIDTH + 25; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 3, 3, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);
	pRoom = new Rooms(); passIdx = 7 * WIDTH + 26; pRoom->Ready(&m_vecBlocks, m_vecRooms.size(), 7, 5, WIDTH, HEIGHT, passIdx); m_vecRooms.push_back(pRoom);

	// Set Pass Wall
	for (int i = 0; i < vecPassage.size(); ++i)
	{
		Blocks* pBlock = m_vecBlocks[vecPassage[i]];

		Blocks* nextBlock;

		// up
		nextBlock = m_vecBlocks[vecPassage[i] - WIDTH];
		if (!nextBlock->m_bIsExist)
			pBlock->m_bIsWall[0] = true;

		// left
		nextBlock = m_vecBlocks[vecPassage[i] - 1];
		if (!nextBlock->m_bIsExist)
			pBlock->m_bIsWall[1] = true;

		// right
		nextBlock = m_vecBlocks[vecPassage[i] + 1];
		if (!nextBlock->m_bIsExist)
			pBlock->m_bIsWall[2] = true;

		// down
		nextBlock = m_vecBlocks[vecPassage[i] + WIDTH];
		if (!nextBlock->m_bIsExist)
			pBlock->m_bIsWall[3] = true;
	}
}

void MapManager::AddRandomTorchAndCrystal()
{
	vector<_uint> vecTorchTemp;
	vector<_uint> vecCrystalTemp;
	for (int i = 0; i < TOTAL; ++i)
	{
		Blocks* pBlock = m_vecBlocks[i];
		if (!pBlock->m_bIsExist)
			continue;
		if (pBlock->m_bPath)
			continue;

		//if (pBlock->m_bIsWall[0] || pBlock->m_bIsWall[1]
		//	|| pBlock->m_bIsWall[2] || pBlock->m_bIsWall[3])
		if (pBlock->m_bIsWall[0] || pBlock->m_bIsWall[1] || pBlock->m_bIsWall[2])
		{
			if (pBlock->m_bIsWall[0] && pBlock->m_bIsWall[1])
				continue;

			if (pBlock->m_bIsWall[0] && pBlock->m_bIsWall[2])
				continue;

			if (pBlock->m_bIsWall[1] && pBlock->m_bIsWall[3])
				continue;

			if (pBlock->m_bIsWall[2] && pBlock->m_bIsWall[3])
				continue;

			vecTorchTemp.push_back(i);
		}

		if (!pBlock->m_bIsWall[0] && !pBlock->m_bIsWall[1] &&
			!pBlock->m_bIsWall[2] && !pBlock->m_bIsWall[3] &&
			!pBlock->m_bIsPathway)
			vecCrystalTemp.push_back(i);
	}

	Blocks* pBlock = m_vecBlocks[125];  // water room
	pBlock->m_bTorch = true;
	Rooms* room = m_vecRooms[pBlock->m_iRoomIndex];
	++room->m_iTorchCount;

	_uint iCount = 0;
	_uint randNum = 0;
	
	while (iCount < 8)
	{
		randNum = GetRandNum(0, vecTorchTemp.size() - 1);
		Blocks* pBlock = m_vecBlocks[vecTorchTemp[randNum]];
		if (pBlock->m_bTorch)
			continue;
		Rooms* room = m_vecRooms[pBlock->m_iRoomIndex];
		if (1 <= room->m_iTorchCount)
			continue;

		pBlock->m_bTorch = true;
		++iCount;
		++room->m_iTorchCount;
	}

	iCount = 0;
	while (iCount < 7)
	{
		randNum = GetRandNum(0, vecCrystalTemp.size() - 1);
		Blocks* pBlock = m_vecBlocks[vecCrystalTemp[randNum]];
		if (pBlock->m_bCrystal)
			continue;

		pBlock->m_bCrystal = true;
		++iCount;
	}
}

void MapManager::CreateBackgroundObjects()
{
	vec3 vRot(0.f);
	vec3 vScale(0.01f);
	string floorID = "";
	string wallID = "";

	for (int i = 0; i < TOTAL; ++i)
	{
		Blocks* pBlock = m_vecBlocks[i];

		if (!pBlock->m_bIsExist)
			continue;

		vec3 vPos = pBlock->m_vCenterPos; 
		floorID = GetRandomFloor();
		m_pScene->AddBGObject(floorID, vPos, vRot, vScale);

		// up
		vPos.z -= 2.5f;
		vRot.y = 180.f;
		if (pBlock->m_bIsDoor[0])
			m_pScene->AddBGObject("DoorWall", vPos, vRot, vScale);
		else if (pBlock->m_bIsWall[0])
		{
			wallID = GetRandomWall();
			pBlock->m_wallID[0] = wallID;
			m_pScene->AddBGObject(wallID, vPos, vRot, vScale);
		}

		// left
		vPos = pBlock->m_vCenterPos;
		vPos.x -= 2.5f;
		vRot.y = -90.f;
		if (pBlock->m_bIsDoor[1])
			m_pScene->AddBGObject("DoorWall", vPos, vRot, vScale);
		if (pBlock->m_bIsWall[1])
		{
			wallID = GetRandomWall();
			pBlock->m_wallID[1] = wallID;
			m_pScene->AddBGObject(wallID, vPos, vRot, vScale);
		}

		// right
		vPos = pBlock->m_vCenterPos;
		vPos.x += 2.5f;
		vRot.y = 90.f;
		if (pBlock->m_bIsDoor[2])
			m_pScene->AddBGObject("DoorWall", vPos, vRot, vScale);
		if (pBlock->m_bIsWall[2])
		{
			wallID = GetRandomWall();
			pBlock->m_wallID[2] = wallID;
			m_pScene->AddBGObject(wallID, vPos, vRot, vScale);
		}

		// down
		vPos = pBlock->m_vCenterPos;
		vPos.z += 2.5f;
		vRot.y = 0.f;
		if (pBlock->m_bIsDoor[3])
			m_pScene->AddBGObject("DoorWall", vPos, vRot, vScale);
		if (pBlock->m_bIsWall[3])
		{
			wallID = GetRandomWall();
			pBlock->m_wallID[3] = wallID;
			m_pScene->AddBGObject(wallID, vPos, vRot, vScale);
		}
	}
}

void MapManager::CreateBeholder()
{
	vec3 vPos(0.f);
	vec3 vRot(0.f, 0.f, 0.f);
	vec3 vScale(1.f, 1.f, 1.f);
	Beholder* pBeholder = nullptr;

	Rooms* room = m_vecRooms[0];
	room->m_bHasBeholder = true;
	vPos = room->GetCenterPos();
	vPos.y += 2.5f;
	pBeholder = m_pScene->AddBeholder(vPos, vRot, vScale);
	if (nullptr != pBeholder)
		pBeholder->SetBeholderIndex(0, 0);
	sBeholderInfo* newInfo = new sBeholderInfo;
	newInfo->beholder = pBeholder;
	newInfo->curRoamingIndex = 0;
	m_vecBeholders.push_back(newInfo);

	room = m_vecRooms[5];
	room->m_bHasBeholder = true;
	vPos = room->GetCenterPos();
	vPos.y += 2.5f;
	pBeholder = m_pScene->AddBeholder(vPos, vRot, vScale);
	if (nullptr != pBeholder)
		pBeholder->SetBeholderIndex(1, 5);
	newInfo = new sBeholderInfo;
	newInfo->beholder = pBeholder;  
	newInfo->curRoamingIndex = 0;
	m_vecBeholders.push_back(newInfo);

	room = m_vecRooms[11];
	room->m_bHasBeholder = true;
	vPos = room->GetCenterPos();
	vPos.y += 2.5f;
	pBeholder = m_pScene->AddBeholder(vPos, vRot, vScale);
	if (nullptr != pBeholder)
		pBeholder->SetBeholderIndex(2, 11);
	newInfo = new sBeholderInfo;
	newInfo->beholder = pBeholder;
	newInfo->curRoamingIndex = 0;
	m_vecBeholders.push_back(newInfo);
}

void MapManager::CreateTorchAndCrystal()
{
	vec3 vRot(0.f);

	_int dir = 0;
	string torchID = "";
	string crystalID = "";
	_uint crystalCount = 0;

	for (int i = 0; i < TOTAL; ++i)
	{
		Blocks* pBlock = m_vecBlocks[i];

		if (pBlock->m_bTorch)
		{
			vec3 vPos = pBlock->m_vCenterPos;
			vec3 vScale(0.01f);
			vPos.y += 2.5f;
			if (pBlock->m_bIsWall[0])
			{
				vPos.z -= 2.5;
				dir = 0;
				if ("Wall01" == pBlock->m_wallID[0])
					vPos.z += 0.2f;
				vRot.y = 180.f;

				torchID = GetRandomTorch();
				m_pScene->AddBGObject(torchID, vPos, vRot, vScale, dir);
			}
			else if (pBlock->m_bIsWall[1])
			{
				vPos.x -= 2.5f;
				dir = 1;
				if ("Wall01" == pBlock->m_wallID[1])
					vPos.x += 0.2f;
				vRot.y -= 90.f;

				torchID = GetRandomTorch();
				m_pScene->AddBGObject(torchID, vPos, vRot, vScale, dir);
			}
			else if (pBlock->m_bIsWall[2])
			{
				vPos.x += 2.5f;
				dir = 2;
				if ("Wall01" == pBlock->m_wallID[2])
					vPos.x -= 0.2f;
				vRot.y = 90.f;

				torchID = GetRandomTorch();
				m_pScene->AddBGObject(torchID, vPos, vRot, vScale, dir);
			}
		}

		vRot.y = 0.f;

		if (pBlock->m_bCrystal)
		{
			vec3 vPos = pBlock->m_vCenterPos;
			vec3 vScale = vec3(0.005f);

			if (0 == crystalCount % 3)
				crystalID = "Crystal1";
			else if (1 == crystalCount % 3)
				crystalID = "Crystal2";
			else
				crystalID = "Crystal3";

			m_pScene->AddBGObject(crystalID, vPos, vRot, vScale);
			++crystalCount;
		}
	}
}

void MapManager::CreateWater()
{
	vec3 vPos(130.f, 0.7f, 20.4f);
	vec3 vRot(0.f, 180.f, 0.f);
	vec3 vScale(0.24f, 0.1f, 0.24f);
	m_pScene->AddBGObject("Water", vPos, vRot, vScale);
}

string MapManager::GetRandomWall()
{
	string str = "Wall02";

	int rand = GetRandNum(1, 100);
	if (rand > 90)
		str = "Wall01";

	return str;
}

string MapManager::GetRandomFloor()
{
	string str = "Floor01";

	int rand = GetRandNum(1, 100);
	if (rand > 90)
		str = "Floor02";

	return str;
}

string MapManager::GetRandomTorch()
{
	string str = "Torch";

	int rand = GetRandNum(1, 100);
	if (rand > 50)
		str = "Torch2";

	return str;
}

