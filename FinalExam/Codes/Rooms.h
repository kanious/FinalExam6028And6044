#ifndef _ROOMS_H_
#define _ROOMS_H_

#include "EngineDefines.h"
#include "Define.h"
#include "glm\vec3.hpp"

class Blocks;
class Rooms
{
public:
	_uint					m_iWidth;
	_uint					m_iHeight;
	_uint					m_iCornerIdx[4];
	glm::vec3				m_vCornerPos[4];
	glm::vec3				m_vCenterPos;
	std::vector<Blocks*>	m_vecEscapes;

	std::vector<Blocks*>*	m_pVecBlocks;
	_uint					m_iTorchCount;
	_bool					m_bHasBeholder;

private:
	_float					m_fMinX;
	_float					m_fMaxX;
	_float					m_fMinZ;
	_float					m_fMaxZ;
	
public:
	explicit Rooms();
	~Rooms();

	glm::vec3 GetCenterPos()		{ return m_vCenterPos; }
	_bool IsInside(glm::vec3 vPos);
	Blocks* GetClosestEscape(glm::vec3 targetPos);

private:
	void CalculateCenterPos();

public:
	void Ready(std::vector<Blocks*>* pVector, _uint roomIdx, _uint width, _uint height, _uint maxW, _uint maxH, _uint firstIdx);
};

#endif //_ROOMS_H_