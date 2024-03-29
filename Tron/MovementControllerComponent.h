#pragma once
#include <BaseComponent.h>
#include "MoveComponent.h"
#include "LevelPathfindingComponent.h"
#include "LevelLayoutComponent.h"

class EnemyControllerComponent;

class MovementControllerComponent : public dae::BaseComponent
{
public:
	MovementControllerComponent(std::shared_ptr<dae::GameObject> gameObject, std::shared_ptr<EnemyControllerComponent> controller
		, std::shared_ptr<MoveComponent> moveComponent, std::shared_ptr<LevelPathfindingComponent> pathfinding
		, std::shared_ptr<LevelLayoutComponent> levelLayout);
	~MovementControllerComponent() = default;

	void Update() override;
	void Render() const override {};

	void MoveToTarget();
	std::shared_ptr<dae::GameObject> GetTarget() const;

private:
	void GetPathToClosestTarget();
	bool IsStillOnPath();

	std::weak_ptr<dae::GameObject> m_TargetObject;
	std::weak_ptr<MoveComponent> m_MoveComponent;
	std::weak_ptr<LevelPathfindingComponent> m_Pathfinding;
	std::weak_ptr<LevelLayoutComponent> m_Layout;

	std::weak_ptr<EnemyControllerComponent> m_Controller;
	std::vector<int> m_Path;
	glm::vec2 m_Target;
};

