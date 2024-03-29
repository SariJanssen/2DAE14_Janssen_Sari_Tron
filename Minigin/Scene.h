#pragma once
#include "SceneManager.h"
#include <vector>

namespace dae
{
	class GameObject;
	class Scene
	{
		friend Scene* SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<GameObject>& object);

		void Update();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::string& GetName() { return m_Name; }

		void SetIsDeleted(bool isDeleted);
		bool IsDeleted();

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};

		static unsigned int m_IdCounter; 
		bool m_IsDeleted;
	};

}
