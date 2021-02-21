#include "cmpch.h"
#include "BoxCollider2D.h"

namespace Chimera
{
	BoxCollider2D::BoxCollider2D(b2Body* body, float width, float height, bool isSensor, float friction, float bounciness, float density)
		: Collider2D{ body, isSensor, friction, bounciness, density }, m_Width(width), m_Height(height)
	{
		m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f);
		UpdateFixtureDef();
		
		m_Fixture = m_Body->CreateFixture(&m_FixtureDef);
	}
	void BoxCollider2D::SetSize(float width, float height)
	{
		m_Width = width;
		m_Height = height;
		m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { m_CenterX, m_CenterY }, 0.0f);
		m_FixtureDef.shape = &m_BoxColliderShape;

		UpdateFixture();
	}
	void BoxCollider2D::SetCenter(float x, float y)
	{
		m_CenterX = x;
		m_CenterY = y;
		m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { x, y }, 0.0f);
		m_FixtureDef.shape = &m_BoxColliderShape;

		UpdateFixture();
	}
	void BoxCollider2D::UpdateFixture()
	{
		m_Body->DestroyFixture(m_Fixture);
		m_Fixture = m_Body->CreateFixture(&m_FixtureDef);
		m_Body->ResetMassData();
	}
}