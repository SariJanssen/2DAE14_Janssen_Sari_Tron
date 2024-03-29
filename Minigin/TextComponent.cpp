#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "ObserverEvents.h"

dae::TextComponent::TextComponent(std::shared_ptr<dae::GameObject> gameObject, const std::string& text, const std::shared_ptr<Font>& font, const glm::vec3& textColor)
	: BaseComponent(gameObject)
	, m_NeedsUpdate(true)
	, m_Text(text)
	, m_Font(font)
	, m_TextTexture(nullptr)
	, m_Color(textColor)
{ }

void dae::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		if (m_Text.empty())
		{
			m_TextTexture = nullptr;
			return;
		}

		const SDL_Color color = { static_cast<Uint8>(m_Color.r),  static_cast<Uint8>(m_Color.g),  static_cast<Uint8>(m_Color.b) };
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		auto pos = GetGameObject()->GetWorldPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

void dae::TextComponent::Notify(std::shared_ptr<GameObject> gameObject, std::shared_ptr<BaseObserverEvent> event)
{
	auto observerEvent = std::dynamic_pointer_cast<TextChangedObserverEvent>(event);
	if (observerEvent != nullptr)
	{
		SetText(observerEvent->text);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	if (text != m_Text)
	{
		m_Text = text;
		m_NeedsUpdate = true;
	}
}

const std::string& dae::TextComponent::GetText() const
{
	return m_Text;
}
