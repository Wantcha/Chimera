#include "cmpch.h"
#include "BoxCollider2DComponent.h"

namespace Chimera
{
	BoxCollider2DComponent::BoxCollider2DComponent(b2Body* body, float width, float height, bool isSensor, float friction, float bounciness, float density)
		: Collider2D{ body, isSensor, friction, bounciness, density }, m_Width(width), m_Height(height)
	{
		m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f);
		UpdateFixtureDef();
		
		m_Fixture = m_Body->CreateFixture(&m_FixtureDef);
	}
	void BoxCollider2DComponent::SetSize(float width, float height)
	{
		m_Width = width;
		m_Height = height;
		m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { m_CenterX, m_CenterY }, 0.0f);
		m_FixtureDef.shape = &m_BoxColliderShape;

		UpdateFixture();
	}
	void BoxCollider2DComponent::SetCenter(float x, float y)
	{
		m_CenterX = x;
		m_CenterY = y;
		m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { x, y }, 0.0f);
		m_FixtureDef.shape = &m_BoxColliderShape;

		UpdateFixture();
	}
	void BoxCollider2DComponent::UpdateFixture()
	{
		m_Body->DestroyFixture(m_Fixture);
		m_Fixture = m_Body->CreateFixture(&m_FixtureDef);
		m_Body->ResetMassData();
	}
}