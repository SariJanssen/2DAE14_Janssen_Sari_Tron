#pragma once
#include "BaseComponent.h"

#pragma warning(push)
#pragma warning(disable:26495) 
#pragma warning(disable:4201) 
#pragma warning(disable:26812) 
#include <glm/glm.hpp>
#pragma warning(pop)


namespace dae
{
	class Texture2D;

	class Texture2DComponent final : public BaseComponent
	{
	public:
		Texture2DComponent(std::shared_ptr<GameObject> gameObject, const std::string& fileName);
		~Texture2DComponent() = default;

		virtual void Update() override {};
		virtual void Render() const override;

		void SetTexture(const std::string& fileName);
		void SetRenderPositionOffset(glm::vec2 renderPos);
		int GetWidth();
		int GetHeight();
	private:
		std::shared_ptr<Texture2D> m_Texture;
		glm::vec2 m_Offset;
	};

}