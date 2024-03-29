#pragma once
#include <BaseComponent.h>
#include <string>
#include "BulletComponent.h"
#include "CollisionHandlerComponent.h"

class LevelLayoutComponent;

class BulletPoolComponent final : public dae::BaseComponent
{
public:
	BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<LevelLayoutComponent> levelLayout
		, std::shared_ptr<CollisionHandlerComponent> collisionHandler, const std::string& sceneName, const std::string& texturePath
		, BulletComponent::Type bulletSource, CollisionHandlerComponent::Layer bulletLayer, int bulletAmount);
	~BulletPoolComponent() = default;

	void CreateBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed);

	void Update() override;
	void Render() const override {};

private:
	BulletComponent* m_FirstAvailable;

	std::vector<BulletComponent*> m_Bullets;
	std::vector<std::weak_ptr<CollisionComponent>> m_BulletCollisions;

	std::shared_ptr<CollisionHandlerComponent> m_CollisionHandler;
	BulletComponent::Type m_BulletType;
	CollisionHandlerComponent::Layer m_BulletLayer;
};

