#pragma once
#include <memory>
#include "GameObject.h"

namespace dae
{
	class BaseComponent
	{
	public:
		BaseComponent(std::shared_ptr<GameObject> gameObject)
			: m_GameObject(gameObject) {}
		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void Update() = 0;
		virtual void Render() const = 0;

	protected:
		std::shared_ptr<GameObject> GetGameObject() const
		{
			return m_GameObject.lock();
		}

	private:
		std::weak_ptr<GameObject> m_GameObject;
	};
}

