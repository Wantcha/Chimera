#include "cmpch.h"
#include "RigidBody2DComponent.h"

namespace Chimera
{
	b2BodyType BodyTypeToBox2D(RigidBody2DComponent::Body2DType type)
	{
		switch (type)
		{
		case RigidBody2DComponent::Body2DType::Static: return b2_staticBody;
		case RigidBody2DComponent::Body2DType::Dynamic: return b2_dynamicBody;
		case RigidBody2DComponent::Body2DType::Kinematic: return b2_kinematicBody;
		}
		return b2_dynamicBody;
	}

	RigidBody2DComponent::Body2DType Box2DToBodyType(b2BodyType type)
	{
		switch (type)
		{
		case b2_staticBody: return RigidBody2DComponent::Body2DType::Static;
		case b2_dynamicBody: return RigidBody2DComponent::Body2DType::Dynamic;
		case b2_kinematicBody: return RigidBody2DComponent::Body2DType::Kinematic;
		}
		return RigidBody2DComponent::Body2DType::Dynamic;
	}

	RigidBody2DComponent::RigidBody2DComponent(b2Body* body, Body2DType type, float gravityScale, bool discreteCollision, bool fixedRotation)
		: m_Body(body)
	{
		SetGravityScale(gravityScale);
		SetFixedRotation(fixedRotation);
		SetDiscreteCollision(discreteCollision);
		SetBodyType(type);
	}

	void RigidBody2DComponent::SetBodyType(Body2DType type)
	{
		m_Body->SetType(BodyTypeToBox2D(type));
		m_Body->ResetMassData();
	}

	RigidBody2DComponent::Body2DType RigidBody2DComponent::GetBodyType()
	{
		return Box2DToBodyType(m_Body->GetType());
	}

	/*void RigidBody2DComponent::UpdateBody(b2Body*& body)
	{
		b2BodyDef bodyDef;
		bodyDef.type = GetBodyType(m_BodyType);
		bodyDef.fixedRotation = m_FixedRotation;
		bodyDef.gravityScale = m_GravityScale;
		bodyDef.bullet = !m_DiscreteCollision;
	}*/
}