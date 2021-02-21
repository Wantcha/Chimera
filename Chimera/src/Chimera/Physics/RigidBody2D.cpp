#include "cmpch.h"
#include "RigidBody2D.h"

namespace Chimera
{
	b2BodyType BodyTypeToBox2D(RigidBody2D::Body2DType type)
	{
		switch (type)
		{
		case RigidBody2D::Body2DType::Static: return b2_staticBody;
		case RigidBody2D::Body2DType::Dynamic: return b2_dynamicBody;
		case RigidBody2D::Body2DType::Kinematic: return b2_kinematicBody;
		}
		return b2_dynamicBody;
	}

	RigidBody2D::Body2DType Box2DToBodyType(b2BodyType type)
	{
		switch (type)
		{
		case b2_staticBody: return RigidBody2D::Body2DType::Static;
		case b2_dynamicBody: return RigidBody2D::Body2DType::Dynamic;
		case b2_kinematicBody: return RigidBody2D::Body2DType::Kinematic;
		}
		return RigidBody2D::Body2DType::Dynamic;
	}

	RigidBody2D::RigidBody2D(b2Body* body, Body2DType type, float gravityScale, bool discreteCollision, bool fixedRotation)
		: m_Body(body)
	{
		SetGravityScale(gravityScale);
		SetFixedRotation(fixedRotation);
		SetDiscreteCollision(discreteCollision);
		SetBodyType(type);
	}

	void RigidBody2D::SetBodyType(Body2DType type)
	{
		m_Body->SetType(BodyTypeToBox2D(type));
		m_Body->ResetMassData();
	}

	RigidBody2D::Body2DType RigidBody2D::GetBodyType()
	{
		return Box2DToBodyType(m_Body->GetType());
	}

	/*void RigidBody2D::UpdateBody(b2Body*& body)
	{
		b2BodyDef bodyDef;
		bodyDef.type = GetBodyType(m_BodyType);
		bodyDef.fixedRotation = m_FixedRotation;
		bodyDef.gravityScale = m_GravityScale;
		bodyDef.bullet = !m_DiscreteCollision;
	}*/
}