#include "pch.h"
#include "..\Headers\Base.h"
#include "..\Headers\Component.h"


USING(Engine)

CComponent::CComponent()
{
	m_tag = "";
	m_bEnable = true;
}

CComponent::~CComponent()
{
}

void CComponent::Update(const _float& dt)
{
}

void CComponent::Render()
{
}

void CComponent::Enable()
{
	m_bEnable = true;
}

void CComponent::Disable()
{
	m_bEnable = false;
}

void CComponent::Destroy()
{
}

void CComponent::SetTag(std::string componentTag)
{
	m_tag = componentTag;
}
