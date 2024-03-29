#pragma once
#include "BaseInputManager.h"

namespace dae
{
	class NullInputManager : public BaseInputManager
	{
	public:
		bool ProcessInput() override { return false; };
		void AddInput(InputAction) override {};
		void RemoveInput(InputAction) override {};
		void Quit() override {};

		glm::vec3 GetJoystickPosition(Joystick, int) const override { return glm::vec3{ 0.f, 0.f, 0.f }; };
		bool IsControllerButton(ButtonState, ControllerButton, int) const override { return false; };
		bool IsKeyboardKey(ButtonState, KeyboardKey) const override { return false; };
	};
}
