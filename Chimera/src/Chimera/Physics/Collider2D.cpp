#include "cmpch.h"
#include "Collider2D.h"

namespace Chimera
{
	Collider2D::~Collider2D()
	{
		//CM_CORE_ERROR("Removing Collider2D");
		//m_Body->DestroyFixture(m_Fixture);
	}
	Collider2D::Collider2D(b2Body* body, bool isSensor, float friction, float bounciness, float density)
		: m_Body(body), m_IsSensor(isSensor), m_Friction(friction), m_Bounciness(bounciness), m_Density(density)
	{

	}
	void Collider2D::UpdateFixtureDef()
	{
		m_FixtureDef.isSensor = m_IsSensor;
		m_FixtureDef.friction = m_Friction;
		m_FixtureDef.restitution = m_Bounciness;
		m_FixtureDef.density = m_Density;
	}
}