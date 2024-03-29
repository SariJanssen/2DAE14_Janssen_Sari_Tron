#include "MiniginPCH.h"
#include "GameObject.h"
#include "Texture2DComponent.h"
#include "Texture2D.h"
#include "TransformComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"


dae::Texture2DComponent::Texture2DComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& fileName)
	: BaseComponent(gameObject)
	, m_Texture{}
	, m_Offset{}
	, m_IsVisible{true}
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void dae::Texture2DComponent::Render() const
{
	if (!m_IsVisible)
	{
		return;
	}

	glm::vec3 position = GetGameObject()->GetWorldPosition();
	position.x -= m_Offset.x;
	position.y -= m_Offset.y;

	dae::Renderer::GetInstance().RenderTexture(*m_Texture, position.x, position.y, GetGameObject()->GetWorldRotation());
}

void dae::Texture2DComponent::SetTexture(const std::string& fileName)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(fileName);
}

void dae::Texture2DComponent::SetRenderPositionOffset(glm::vec2 renderPos)
{
	m_Offset = renderPos;
}

int dae::Texture2DComponent::GetWidth()
{
	return m_Texture->GetWidth();
}

int dae::Texture2DComponent::GetHeight()
{
	return m_Texture->GetHeight();
}

void dae::Texture2DComponent::SetIsVisible(bool isVisible)
{
	m_IsVisible = isVisible;
}
