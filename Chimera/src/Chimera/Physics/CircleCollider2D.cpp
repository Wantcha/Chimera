#include "cmpch.h"

#include "CircleCollider2D.h"

namespace Chimera
{
	CircleCollider2D::CircleCollider2D(b2Body* body, float radius, bool isSensor, float friction, float bounciness, float density)
		: Collider2D{ body, isSensor, friction, bounciness, density }, m_Radius(radius)
	{
		//m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f);
		m_CircleColliderShape.m_radius = radius;
		UpdateFixtureDef();

		m_Fixture = m_Body->CreateFixture(&m_FixtureDef);
	}
	void CircleCollider2D::SetCenter(float x, float y)
	{
		m_CenterX = x;
		m_CenterY = y;
		m_CircleColliderShape.m_p.Set(x, y);
		UpdateFixture();
	}
	void CircleCollider2D::UpdateFixture()
	{
		m_FixtureDef.shape = &m_CircleColliderShape;

		m_Body->DestroyFixture(m_Fixture);
		m_Fixture = m_Body->CreateFixture(&m_FixtureDef);
		m_Body->ResetMassData();
	}
}