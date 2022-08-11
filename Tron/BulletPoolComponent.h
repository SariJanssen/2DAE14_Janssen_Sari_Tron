#pragma once
#include <BaseComponent.h>
#include <string>
#include "BulletComponent.h"

class BulletPoolComponent final : public dae::BaseComponent
{
public:
	BulletPoolComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& sceneName, int bulletAmount);
	~BulletPoolComponent() = default;

	void CreateBullet(const glm::vec2& startPos, const glm::vec2& direction, int bounces, float bulletSpeed, BulletComponent::Type bulletSource);

	void Update() override;
	void Render() const override {};

private:
	BulletComponent* m_FirstAvailable;

	std::vector<dae::GameObject*> m_Bullets;
};

