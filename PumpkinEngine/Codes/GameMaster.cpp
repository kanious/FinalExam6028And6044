#include "pch.h"
#include "..\Headers\GameMaster.h"
#include "..\Headers\ComponentMaster.h"
#include "..\Headers\InputDevice.h"
#include "..\Headers\ObjectPooler.h"
#include "..\Headers\OpenGLDevice.h"
#include "..\Headers\Renderer.h"
#include "..\Headers\Timer.h"
#include "..\Headers\XMLParser.h"
#include "..\Headers\SoundMaster.h"
#include "..\Headers\CollisionMaster.h"
#include "..\Headers\LightMaster.h"

#include "..\Headers\Scene.h"

USING(Engine)
SINGLETON_FUNCTION(CGameMaster)

CGameMaster::CGameMaster()
	: m_bGameClose(false)
{
	m_pRenderer = CRenderer::GetInstance();
	m_pCurrentScene = nullptr;
}

CGameMaster::~CGameMaster()
{
}

void CGameMaster::Update(const _float& dt)
{
	if (nullptr == m_pCurrentScene)
		return;

	m_pCurrentScene->Update(dt);
}

void CGameMaster::Render()
{
	if (nullptr != m_pRenderer)
		m_pRenderer->Render();

	if (nullptr != m_pCurrentScene)
		m_pCurrentScene->Render();
}

void CGameMaster::Destroy()
{
	SafeDestroy(m_pCurrentScene);

	SafeDestroy(CComponentMaster::GetInstance());
	SafeDestroy(CInputDevice::GetInstance());
	SafeDestroy(CObjectPooler::GetInstance());
	SafeDestroy(COpenGLDevice::GetInstance());
	SafeDestroy(CRenderer::GetInstance());
	SafeDestroy(CTimer::GetInstance());
	SafeDestroy(CSoundMaster::GetInstance());
	SafeDestroy(CXMLParser::GetInstance());
	SafeDestroy(CCollisionMaster::GetInstance());
	SafeDestroy(CLightMaster::GetInstance());

	m_pRenderer = nullptr;
}

void CGameMaster::SetCurrentScene(CScene* pScene)
{
	SafeDestroy(m_pCurrentScene);
	
	m_pCurrentScene = pScene;
}
