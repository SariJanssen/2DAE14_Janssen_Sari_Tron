#pragma once
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <functional>
#include <memory>

#include "Minigin.h"
#include "GameObject.h"
#include "Texture2DComponent.h"
#include "ServiceLocator.h"
#include "BaseSceneManager.h"

#include "MoveComponent.h"
#include "MoveCommand.h"

#include "LevelLayoutComponent.h"
#include "LevelVisualComponent.h"
#include "LevelMovementComponent.h"
#include "LevelPathfindingComponent.h"
#include "MovementControllerComponent.h"
#include "HealthComponent.h"

#include "BulletPoolComponent.h"
#include "GunComponent.h"
#include "FireCommand.h"

void LoadGame()
{
	auto scene = dae::ServiceLocator::GetSceneManager().CreateScene("Level0");
	auto& inputM = dae::ServiceLocator::GetInputManager();
	
	//dae::ServiceLocator::GetSoundSystem().RegisterMusic(0, "../Data/01_BGM#01.mp3");
	//dae::ServiceLocator::GetSoundSystem().PlayMusic(0, 1, 0);

	// LEVEL
	auto level_go = std::make_shared<dae::GameObject>(100.f, 20.f, 0.f);
	std::shared_ptr<LevelLayoutComponent> level_layout = std::make_shared<LevelLayoutComponent>(level_go, "../Data/LevelLayout01.csv", 16, 8);
	level_go->AddComponent(level_layout);
	auto level_visuals = std::make_shared<LevelVisualComponent>(level_go, level_layout);
	level_go->AddComponent(level_visuals);
	auto level_movement = std::make_shared<LevelMovementComponent>(level_go, level_layout);
	level_go->AddComponent(level_movement);
	auto level_pathfinding = std::make_shared<LevelPathfindingComponent>(level_go, level_layout);
	level_go->AddComponent(level_pathfinding);

	scene->Add(level_go);

	// BULLET POOL
	auto bulletPool_go = std::make_shared<dae::GameObject>();
	auto bulletPool_comp = std::make_shared<BulletPoolComponent>(bulletPool_go, level_layout, "Level0", 30);
	bulletPool_go->AddComponent(bulletPool_comp);

	scene->Add(bulletPool_go);

	// PLAYER
	auto player_startPos = level_layout->GetGridCenter(level_layout->GetPlayerStartPositions()[0]);
	auto playerRed_go = std::make_shared<dae::GameObject>(player_startPos.x, player_startPos.y, 0.f);
	auto playerRed_texture = std::make_shared<dae::Texture2DComponent>(playerRed_go, "Sprites/RedTank.png");
	playerRed_texture->SetRenderPositionOffset(glm::vec2(16.f, 16.f));
	playerRed_go->AddComponent(playerRed_texture);
	playerRed_go->AddComponent(std::make_shared<MoveComponent>(playerRed_go, level_movement, 60.f));
	playerRed_go->AddComponent(std::make_shared<HealthComponent>(playerRed_go, 3));
	playerRed_go->AddComponent(std::make_shared<GunComponent>(playerRed_go, bulletPool_comp, BulletComponent::Type::Player, 5, 1.f, 100.f));

	scene->Add(playerRed_go);

	// INPUT
	auto movementInput = dae::InputAction(0, std::make_shared<MoveCommand>(playerRed_go->GetComponent<MoveComponent>(), dae::Joystick::LeftStick), dae::Joystick::LeftStick);
	inputM.AddInput(movementInput);
	auto fireInput = dae::InputAction(0, std::make_shared<FireCommand>(playerRed_go->GetComponent<GunComponent>(), dae::Joystick::RightStick), dae::Joystick::RightStick);
	inputM.AddInput(fireInput);

	// ENEMY
	//const auto& enemy_startPosVec = level_layout->GetEnemyStartPositions();
	//for (size_t i = 0; i < enemy_startPosVec.size(); ++i)
	//{
	//	const auto& enemy_startPos = level_layout->GetGridCenter(enemy_startPosVec[i]);
	//	auto enemy_go = std::make_shared<dae::GameObject>(enemy_startPos.x, enemy_startPos.y, 0.f);
	//	auto enemy_texture = std::make_shared<dae::Texture2DComponent>(enemy_go, "Sprites/BlueTank.png");
	//	enemy_texture->SetRenderPositionOffset(glm::vec2(16.f, 16.f));
	//	enemy_go->AddComponent(enemy_texture);
	//	auto enemy_moveComp = std::make_shared<MoveComponent>(enemy_go, level_movement, 40.f);
	//	enemy_go->AddComponent(enemy_moveComp);
	//	enemy_go->AddComponent(std::make_shared<MovementControllerComponent>(enemy_go, playerRed_go, enemy_moveComp, level_pathfinding, level_layout));

	//	scene->Add(enemy_go);
	//}
}

int main(int, char* [])
{
	std::function<void()> loadGame = LoadGame;

	dae::Minigin engine;
	engine.Run(loadGame);
	return 0;
}