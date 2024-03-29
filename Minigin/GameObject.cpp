#include "MiniginPCH.h"
#include "GameObject.h"
#include "BaseComponent.h"

using namespace dae;

GameObject::GameObject(float x, float y, float z)
	: GameObject(glm::vec3(x, y, z))
{
}

dae::GameObject::GameObject(const glm::vec3& pos)
	: m_ComponentVec{}
	, m_ChildrenVec{}
	, m_Parent{}
	, m_LocalTransform{ pos }
	, m_WorldTransform{ pos }
	, m_IsPositionDirty{ false }
	, m_IsRotationDirty{ false }
	, m_IsDead{ false }
{
}

GameObject::GameObject()
	: GameObject(0.f, 0.f, 0.f)
{
}

void GameObject::Update()
{
	for (auto& child : m_ChildrenVec)
	{
		child->Update();
	}

	for (auto& component : m_ComponentVec)
	{
		component->Update();
	}
}

void GameObject::Render() const
{
	for (auto& component : m_ComponentVec)
	{
		component->Render();
	}

	for (auto& child : m_ChildrenVec)
	{
		child->Render();
	}
}

void GameObject::SetParent(std::shared_ptr<GameObject> parent, std::shared_ptr<GameObject> child, bool keepWorldPosition)
{
	if (parent == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
		SetLocalRotation(GetWorldRotation());
		RemoveParent();
		return;
	}
	
	if (keepWorldPosition)
	{
		SetLocalPosition(GetLocalPosition() - parent->GetWorldPosition());
		SetLocalRotation(GetLocalRotation() - parent->GetWorldRotation());
	}
	m_IsPositionDirty = true;
	m_IsRotationDirty = true;

	// Change parent
	RemoveParent();
	m_Parent = parent;
	m_Parent.lock()->AddChild(child);
}

std::weak_ptr<GameObject> GameObject::GetParent() const
{
	return m_Parent;
}

void dae::GameObject::RemoveParent()
{
	auto parent = m_Parent.lock();

	if (parent == nullptr)
	{
		return;
	}

	for (size_t i{}; i < parent->GetChildCount(); ++i)
	{
		if (GetChildAt(i).get() == this)
		{
			parent->RemoveChildAt(i);
			m_Parent.reset();
			return;
		}
	}
}

size_t GameObject::GetChildCount() const
{
	return m_ChildrenVec.size();
}

std::shared_ptr<GameObject> GameObject::GetChildAt(size_t index) const
{
	if (index < GetChildCount() && index >= 0)	
	{
		return m_ChildrenVec[index];
	}
	return nullptr;
}

void GameObject::RemoveChildAt(size_t index)
{
	if (index < GetChildCount() && index >= 0)
	{
		m_ChildrenVec.erase(m_ChildrenVec.begin() + index);
	}
}

std::shared_ptr<GameObject> dae::GameObject::GetThisGameObjectFromParent(std::weak_ptr<GameObject> parent)
{
	auto size = parent.lock()->GetChildCount();
	for (size_t i{}; i < size; ++i)
	{
		auto child = parent.lock()->GetChildAt(i);
		if (child.get() == this)
		{
			return child;
		}
	}

	return nullptr;
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_IsPositionDirty)
	{
		if (m_Parent.lock() == nullptr)	
		{
			SetWorldPosition(m_LocalTransform.GetPosition());
		}
		else
		{
			SetWorldPosition(m_Parent.lock()->GetWorldPosition() + GetLocalPosition());
		}
	}

	m_IsPositionDirty = false;
}

void dae::GameObject::UpdateWorldRotation()
{
	if (m_IsRotationDirty)
	{
		if (m_Parent.lock() == nullptr)
		{
			SetWorldRotation(m_LocalTransform.GetRotation());
		}
		else
		{
			SetWorldRotation(m_Parent.lock()->GetWorldRotation() + GetLocalRotation());
		}
	}

	m_IsRotationDirty = false;
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldTransform.GetPosition();
}

const glm::vec3& dae::GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}

void dae::GameObject::SetLocalPosition(const glm::vec3& position)
{
	m_LocalTransform.SetPosition(position);
	m_IsPositionDirty = true;
	for (auto& child : m_ChildrenVec)
	{
		child->m_IsPositionDirty = true;
	}
}

void dae::GameObject::SetWorldPosition(const glm::vec3& position)
{
	m_WorldTransform.SetPosition(position);
}

float dae::GameObject::GetLocalRotation() const
{
	return m_LocalTransform.GetRotation();
}

void dae::GameObject::SetLocalRotation(float angleDeg)
{
	m_LocalTransform.SetRotation(angleDeg);
	m_IsRotationDirty = true;
	for (auto& child : m_ChildrenVec)
	{
		child->m_IsRotationDirty = true;
	}
}

float dae::GameObject::GetWorldRotation()
{
	if (m_IsRotationDirty)
	{
		UpdateWorldRotation();
	}
	return m_WorldTransform.GetRotation();
}

void dae::GameObject::SetWorldRotation(float angleDeg)
{
	m_WorldTransform.SetRotation(angleDeg);
}

void GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	if (std::find(m_ChildrenVec.begin(), m_ChildrenVec.end(), child) == m_ChildrenVec.end())
	{
		m_ChildrenVec.emplace_back(child);
	}
}
