#include "BulletPoolComponent.h"
#include "GameObject.h"
#include "BulletComponent.h"
#include <iostream>
#include "Texture2DComponent.h"
#include "ServiceLocator.h"
#include "LevelLayoutComponent.h"

BulletPoolComponent::BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout
	, const std::string& sceneName, const std::string& texturePath, BulletComponent::Type bulletSource, int bulletAmount)
	: BaseComponent(gameObject)
	, m_FirstAvailable{}
	, m_Bullets{}
	, m_BulletType{bulletSource}
{
	auto pScene = dae::ServiceLocator::GetSceneManager().GetScene(sceneName);
	
	BulletComponent* previous{};

	for (size_t i = 0; i < bulletAmount; ++i)
	{
		auto bullet_go = std::make_shared<dae::GameObject>();
		std::shared_ptr<BulletComponent> bulletComponent{ new BulletComponent(bullet_go, levelLayout) }; // new because can not create object from friend class using RAII
		bullet_go->AddComponent(bulletComponent);
		auto textureComponent = std::make_shared<dae::Texture2DComponent>(bullet_go, texturePath);
		textureComponent->SetRenderPositionOffset(glm::vec2(textureComponent->GetWidth() / 2.f, textureComponent->GetHeight() / 2.f));
		bullet_go->AddComponent(textureComponent);

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
		}
	}
}