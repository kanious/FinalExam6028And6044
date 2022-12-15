#include "Blocks.h"

USING(glm)

Blocks::Blocks()
	: m_iIndex(0), m_vCenterPos(vec3(0.f)), m_bIsExist(false), m_bPath(false), m_bTorch(false)
	, m_bCrystal(false), m_bIsPathway(false)
{
	memset(m_bIsWall, 0, sizeof(m_bIsWall));
	memset(m_bIsDoor, 0, sizeof(m_bIsDoor));
}

Blocks::~Blocks()
{
}

void Blocks::Ready(_uint idx, vec3 vCenterPos)
{
	m_iIndex = idx;
	m_vCenterPos = vCenterPos;
}
