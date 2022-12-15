#include "Rooms.h"
#include "Blocks.h"
#include "OpenGLDefines.h"

USING(glm)
USING(std)

Rooms::Rooms()
	: m_iWidth(0), m_iHeight(0), m_pVecBlocks(nullptr), m_iTorchCount(0), m_bHasBeholder(false)
	, m_vCenterPos(vec3(0.f)), m_fMinX(0.f), m_fMaxX(0.f), m_fMinZ(0.f), m_fMaxZ(0.f)
{
	memset(m_iCornerIdx, 0, sizeof(m_iCornerIdx));
	memset(m_vCornerPos, 0, sizeof(m_vCornerPos));
	m_vecEscapes.clear();
}

Rooms::~Rooms()
{
	m_vecEscapes.clear();
}

_bool Rooms::IsInside(vec3 vPos)
{
	if (vPos.x >= m_fMinX && vPos.x <= m_fMaxX && vPos.z >= m_fMinZ && vPos.z <= m_fMaxZ)
		return true;

	return false;
}

Blocks* Rooms::GetClosestEscape(vec3 targetPos)
{
	Blocks* pBlock = nullptr;
	_float closestDist = FLT_MAX;

	for (int i = 0; i < m_vecEscapes.size(); ++i)
	{
		vec3 vBlockPos = m_vecEscapes[i]->m_vCenterPos;
		_float dist = distance(vBlockPos, targetPos);
		if (dist < closestDist)
		{
			pBlock = m_vecEscapes[i];
			closestDist = dist;
		}
	}

	return pBlock;
}

void Rooms::CalculateCenterPos()
{
	m_fMinX = FLT_MAX;
	m_fMinZ = FLT_MAX;

	m_fMaxX = 0.f;
	m_fMaxZ = 0.f;

	for (int i = 0; i < 4; ++i)
	{
		if (m_vCornerPos[i].x < m_fMinX)
			m_fMinX = m_vCornerPos[i].x;
		if (m_vCornerPos[i].x > m_fMaxX)
			m_fMaxX = m_vCornerPos[i].x;

		if (m_vCornerPos[i].z < m_fMinZ)
			m_fMinZ = m_vCornerPos[i].z;
		if (m_vCornerPos[i].z > m_fMaxZ)
			m_fMaxZ = m_vCornerPos[i].z;
	}

	m_vCenterPos = vec3((m_fMinX + m_fMaxX) / 2.f, 0.f, (m_fMinZ + m_fMaxZ) / 2.f);

	m_fMinX -= 2.5f;
	m_fMaxX += 2.5f;
	m_fMinZ -= 2.5f;
	m_fMaxZ += 2.5f;
}

void Rooms::Ready(vector<Blocks*>* pVector, _uint roomIdx, _uint width, _uint height, _uint maxW, _uint maxH, _uint firstIdx)
{
	m_pVecBlocks = pVector;
	m_iWidth = width;
	m_iHeight = height;
	
	m_iCornerIdx[0] = firstIdx;
	m_iCornerIdx[1] = firstIdx + width - 1;
	m_iCornerIdx[3] = firstIdx + ((height - 1) * maxW);
	m_iCornerIdx[2] = m_iCornerIdx[3] + width - 1;

	m_vCornerPos[0] = (*m_pVecBlocks)[m_iCornerIdx[0]]->m_vCenterPos;
	m_vCornerPos[1] = (*m_pVecBlocks)[m_iCornerIdx[1]]->m_vCenterPos;
	m_vCornerPos[2] = (*m_pVecBlocks)[m_iCornerIdx[2]]->m_vCenterPos;
	m_vCornerPos[3] = (*m_pVecBlocks)[m_iCornerIdx[3]]->m_vCenterPos;

	Blocks* block;
	for (_uint i = 0; i < height; ++i)
	{
		for (_uint j = 0; j < width; ++j)
		{
			_uint curIdx = firstIdx + (i * maxW) + j;
			block = (*m_pVecBlocks)[curIdx];
			block->m_bIsExist = true;
			block->m_iRoomIndex = roomIdx;

			// up
			if (0 == i)
			{
				if (curIdx < maxW)
					block->m_bIsWall[0] = true;
				else
				{
					if ((*m_pVecBlocks)[curIdx - maxW]->m_bIsExist)
					{
						block->m_bIsDoor[0] = true;
						block->m_bIsPathway = true;
						block->m_iDir = 0;
						m_vecEscapes.push_back(block);
					}
					else
						block->m_bIsWall[0] = true;
				}
			}

			// left
			if (0 == j)
			{
				if (0 == curIdx % maxW)
					block->m_bIsWall[1] = true;
				else
				{
					if ((*m_pVecBlocks)[curIdx - 1]->m_bIsExist)
					{
						block->m_bIsDoor[1] = true;
						block->m_bIsPathway = true;
						block->m_iDir = 1;
						m_vecEscapes.push_back(block);
					}
					else
						block->m_bIsWall[1] = true;
				}
			}

			// right
			if (width - 1 == j)
			{
				if (maxW - 1 == curIdx % maxW)
					block->m_bIsWall[2] = true;
				else
				{
					if ((*m_pVecBlocks)[curIdx + 1]->m_bIsExist)
					{
						block->m_bIsDoor[2] = true;
						block->m_bIsPathway = true;
						block->m_iDir = 2;
						m_vecEscapes.push_back(block);
					}
					else
						block->m_bIsWall[2] = true;
				}
			}

			// down
			if (height - 1 == i)
			{
				if ((maxW * maxH) - maxW <= curIdx)
					block->m_bIsWall[3] = true;
				else
				{
					if ((*m_pVecBlocks)[curIdx + maxW]->m_bIsExist)
					{
						block->m_bIsDoor[3] = true;
						block->m_bIsPathway = true;
						block->m_iDir = 3;
						m_vecEscapes.push_back(block);
					}
					else
						block->m_bIsWall[3] = true;
				}
			}
		}
	}

	CalculateCenterPos();
}
