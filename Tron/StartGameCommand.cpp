#include "StartGameCommand.h"
#include "LevelManager.h"

StartGameCommand::StartGameCommand(std::shared_ptr<dae::BaseComponent> component)
	: Command(component)
{
}

void StartGameCommand::Execute(float )
{
	LevelManager::GetInstance().HandleInput(LevelManager::InputStates::Start);
}
