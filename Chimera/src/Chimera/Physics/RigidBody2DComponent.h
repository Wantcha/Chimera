#pragma once

#include "Chimera/Core/Core.h"
#include "box2d/box2d.h"
#include "Chimera/Scene/Scene.h"

namespace Chimera
{

	class RigidBody2DComponent
	{
	public:
		enum class Body2DType { Static = 0, Dynamic = 1, Kinematic = 2 };

	public:
		RigidBody2DComponent() = default;
		~RigidBody2DComponent() = default;

		RigidBody2DComponent(b2Body* body, Body2DType type = Body2DType::Dynamic, float gravityScale = 1.0f, bool discreteCollision = true, bool fixedRotation = false);

		void SetBody(b2Body* body) { m_Body = body; }

		void SetGravityScale(float scale) { m_Body->SetGravityScale(scale); }
		void SetFixedRotation(bool fixed) { m_Body->SetFixedRotation(fixed); }
		void SetDiscreteCollision(bool discrete) { m_Body->SetBullet(!discrete); }
		void SetBodyType(Body2DType type);
		void SetMass(float mass) { b2MassData massData; m_Body->GetMassData(&massData); massData.mass = mass; m_Body->SetMassData(&massData); }
		void SetLinearVelocity(glm::vec2 velocity) { m_Body->SetLinearVelocity(b2Vec2{ velocity.x, velocity.y }); }

		b2Body* GetBody() const { return m_Body; }
		float GetGravityScale() const { return m_Body->GetGravityScale(); }
		bool IsFixedRotation() const { return m_Body->IsFixedRotation(); }
		bool IsDiscreteCollision() const { return !m_Body->IsBullet(); }
		Body2DType GetBodyType();
		glm::vec2 GetLinearVelocity() const { return glm::vec2{ m_Body->GetLinearVelocity().x, m_Body->GetLinearVelocity().y }; }

		glm::vec3 GetPosition() const { return { m_Body->GetTransform().p.x, m_Body->GetTransform().p.y , zPos}; }
		float GetRotation() const { return m_Body->GetTransform().q.GetAngle(); }

		void SetTransform(glm::vec3 position, float rotation) { m_Body->SetTransform({ position.x, position.y }, rotation); zPos = position.z; }
		//void SetRotation() { return m_Body->GetTransform().q.GetAngle(); };
		void ApplyForce(glm::vec2& force) { m_Body->ApplyForceToCenter(b2Vec2{ force.x, force.y }, false); }
		void ApplyForceAtPoint(glm::vec2& force, glm::vec2& point) { m_Body->ApplyForce(b2Vec2{ force.x, force.y }, b2Vec2{ point.x, point.y }, false); }

	private:
		b2Body* m_Body;
		float zPos = 0.0f;

		/*Body2DType m_BodyType = Body2DType::Dynamic;
		float m_GravityScale = 1.0f;
		bool m_FixedRotation = false;
		bool m_DiscreteCollision = true;*/
	};
}
