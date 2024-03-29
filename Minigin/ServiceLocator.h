#pragma once
#include "NullSoundSystem.h"
#include "NullInputManager.h"
#include "NullSceneManager.h"
#include <memory>

namespace dae
{
	class BaseSoundSystem;
	class BaseInputManager;
	class BaseSceneManager;

	class ServiceLocator final
	{
	public: // TODO: SERVICELOCATOR: add renderer, resource manager

		static BaseSoundSystem& GetSoundSystem();
		static void RegisterSoundSystem(BaseSoundSystem* soundSystem);

		static BaseInputManager& GetInputManager();
		static void RegisterInputManager(BaseInputManager* inputManager);

		static BaseSceneManager& GetSceneManager();
		static void RegisterSceneManager(BaseSceneManager* sceneManager);

	private:
		template <typename T>
		static T* SetService(T* newService, T& defaultService)
		{
			if (newService == nullptr)
			{
				return &defaultService;
			}

			return newService;
		}
		
		static BaseSoundSystem* m_pSoundSystemInst;
		static NullSoundSystem m_DefaultSoundSystem;

		static BaseInputManager* m_pInputManagerInst;
		static NullInputManager m_DefaultInputManager;

		static BaseSceneManager* m_pSceneManagerInst;
		static NullSceneManager m_DefaultSceneManager;
	};
}