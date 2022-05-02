#include "MiniginPCH.h"
#include "ServiceLocator.h"
#include "BaseSoundSystem.h"
#include "BaseInputManager.h"
#include "BaseSceneManager.h"


using namespace dae;

NullSoundSystem ServiceLocator::m_DefaultSoundSystem;
BaseSoundSystem* ServiceLocator::m_pSoundSystemInst = &m_DefaultSoundSystem;

NullInputManager ServiceLocator::m_DefaultInputManager;
BaseInputManager* ServiceLocator::m_pInputManagerInst = &m_DefaultInputManager;

NullSceneManager ServiceLocator::m_DefaultSceneManager;
BaseSceneManager* ServiceLocator::m_pSceneManagerInst = &m_DefaultSceneManager;

void ServiceLocator::Destroy()
{
	if (m_pSoundSystemInst != &m_DefaultSoundSystem)
	{
		delete m_pSoundSystemInst;
		m_pSoundSystemInst = &m_DefaultSoundSystem;
	}

	if (m_pInputManagerInst != &m_DefaultInputManager)
	{
		delete m_pInputManagerInst;
		m_pInputManagerInst = &m_DefaultInputManager;
	}

	if (m_pSceneManagerInst != &m_DefaultSceneManager)
	{
		delete m_pSceneManagerInst;
		m_pSceneManagerInst = &m_DefaultSceneManager;
	}
}


BaseSoundSystem& ServiceLocator::GetSoundSystem()
{
	return *m_pSoundSystemInst;
}

void ServiceLocator::RegisterSoundSystem(BaseSoundSystem* soundSystem)
{
	m_pSoundSystemInst = SetService<BaseSoundSystem>(m_pSoundSystemInst, soundSystem, m_DefaultSoundSystem);
}

BaseInputManager& ServiceLocator::GetInputManager()
{
	return *m_pInputManagerInst;
}

void ServiceLocator::RegisterInputManager(BaseInputManager* inputManager)
{
	m_pInputManagerInst = SetService<BaseInputManager>(m_pInputManagerInst, inputManager, m_DefaultInputManager);
}

BaseSceneManager& dae::ServiceLocator::GetSceneManager()
{
	return *m_pSceneManagerInst;
}

void dae::ServiceLocator::RegisterSceneManager(BaseSceneManager* sceneManager)
{
	m_pSceneManagerInst = SetService<BaseSceneManager>(m_pSceneManagerInst, sceneManager, m_DefaultSceneManager);
}
