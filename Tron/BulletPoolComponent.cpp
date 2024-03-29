#include "BulletPoolComponent.h"
#include "GameObject.h"
#include "BulletComponent.h"
#include <iostream>
#include "Texture2DComponent.h"
#include "ServiceLocator.h"
#include "LevelLayoutComponent.h"
#include "CollisionHandlerComponent.h"
#include "CollisionComponent.h"

BulletPoolComponent::BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout
	, std::shared_ptr<CollisionHandlerComponent> collisionHandler, const std::string& sceneName, const std::string& texturePath
	, BulletComponent::Type bulletSource, CollisionHandlerComponent::Layer bulletLayer, int bulletAmount)
	: BaseComponent(gameObject)
	, m_FirstAvailable{}
	, m_Bullets{}
	, m_CollisionHandler{collisionHandler}
	, m_BulletType{bulletSource}
	, m_BulletLayer{bulletLayer}
{
	auto pScene = dae::ServiceLocator::GetSceneManager().GetScene(sceneName);
	
	BulletComponent* previous{};

	for (size_t i = 0; i < static_cast<size_t>(bulletAmount); ++i)
	{
		auto bullet_go = std::make_shared<dae::GameObject>();
		std::shared_ptr<BulletComponent> bulletComponent{ new BulletComponent(bullet_go, levelLayout) }; // new because can not create object from friend class using RAII
		bullet_go->AddComponent(bulletComponent);
		auto textureComponent = std::make_shared<dae::Texture2DComponent>(bullet_go, texturePath);
		textureComponent->SetRenderPositionOffset(glm::vec2(textureComponent->GetWidth() / 2.f, textureComponent->GetHeight() / 2.f));
		textureComponent->SetIsVisible(false);

		bullet_go->AddComponent(textureComponent);
		auto collider = std::make_shared<CollisionComponent>(bullet_go, static_cast<float>(textureComponent->GetWidth()), static_cast<float>(textureComponent->GetHeight()));
		bullet_go->AddComponent(collider);

		collider->AddObserver(bulletComponent);

		if (i == 0)
		{
			m_FirstAvailable = bulletComponent.get();
			previous = bulletComponent.get();
		}
		else
		{
			bulletComponent->SetNext(nullptr);
			previous->SetNext(bulletComponent.get());
			previous = bulletComponent.get();
		}

		m_Bullets.emplace_back(bulletComponent.get());
		m_BulletCollisions.emplace_back(collider);
		pScene->Add(bullet_go);
	}
}

void BulletPoolComponent::CreateBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed)
{
	if (m_FirstAvailable == nullptr)
	{
		return;
	}

	BulletComponent* newBullet = m_FirstAvailable;
	m_FirstAvailable = newBullet->GetNext();
	newBullet->GetGameObject()->GetComponent<dae::Texture2DComponent>()->SetIsVisible(true);
	newBullet->InitializeBullet(startPos, direction, bounces, bulletSpeed, m_BulletType);
	
	for (size_t i = 0; i < m_Bullets.size(); ++i)
	{
		if (m_Bullets[i] == newBullet)
		{
			m_CollisionHandler->AddCollider(m_BulletCollisions[i].lock(), m_BulletLayer);
		}
	}
}


void BulletPoolComponent::Update()
{
	for (size_t i = 0; i < m_Bullets.size(); ++i)
	{
		if (!m_Bullets[i]->IsInUse() && !m_Bullets[i]->GetIsInPool())
		{
			m_Bullets[i]->SetNext(m_FirstAvailable);
			m_FirstAvailable = m_Bullets[i];
			m_Bullets[i]->SetIsInPool(true);
			m_Bullets[i]->GetGameObject()->GetComponent<dae::Texture2DComponent>()->SetIsVisible(false);

			m_CollisionHandler->RemoveCollider(m_BulletCollisions[i].lock(), m_BulletLayer);
		}
	}
}
