#pragma once

#include "box2d/box2d.h"
#include <glm/glm.hpp>

namespace Chimera
{
	class Collider2D
	{
	public:
		Collider2D() = default;
		/*Collider2D(const Collider2D&& other) noexcept
		{
			CM_CORE_ERROR("Moving Collider2D");
		};*/
		virtual ~Collider2D();
		Collider2D(b2Body* body, bool isSensor = false, float friction = 0.3f, float bounciness = 0.1f, float density = 1.0f);

		virtual void SetCenter(float x, float y) = 0;
		virtual void SetCenter(glm::vec3 center) = 0;

		void SetBody(b2Body* body) { m_Body = body; UpdateFixtureDef(); }
		void SetDensity(float density) { m_Density = m_FixtureDef.density = density; UpdateFixture(); }
		void SetFriction(float friction) { m_Friction = m_FixtureDef.friction = friction; UpdateFixture(); }
		void SetBounciness(float bounciness) { m_Bounciness = m_FixtureDef.restitution = bounciness; UpdateFixture(); }
		void SetSensor(bool sensor) { m_IsSensor = m_FixtureDef.isSensor = sensor; UpdateFixture(); }

		virtual Collider2D* GetCollider() = 0;
		glm::vec2 GetCenter() const { return { m_CenterX, m_CenterY }; }
		b2Body* GetBody() const { return m_Body; }
		bool IsSensor() const { return m_IsSensor; }
		float GetDensity() const { return m_Density; }
		float GetBounciness() const { return m_Bounciness; }
		float GetFriction() const { return m_Friction; }
		b2FixtureDef& GetFixtureDef() { return m_FixtureDef; }
		b2Fixture* GetFixture() { return m_Fixture; }

		Collider2D& operator=(const Collider2D& other)
		{
			CM_CORE_ERROR("Copy Collider2D");

			return *this;
		}
		/*(Collider2D& operator=(const Collider2D&& other) noexcept
		{
			CM_CORE_ERROR("Move operator Collider2D");

			return *this;
		}*/
		void DestroyCollider()
		{
			m_ToBeDestroyed = true;
			m_Body->DestroyFixture(m_Fixture);
		}
	protected:
		virtual void UpdateFixture() = 0;
		void UpdateFixtureDef();

		b2Body* m_Body;
		b2FixtureDef m_FixtureDef;
		b2Fixture* m_Fixture;

		float m_CenterX = 0.0f, m_CenterY = 0.0f;

		bool m_IsSensor = false;
		float m_Density = 1.0f, m_Bounciness = 0.1f, m_Friction = 0.3f;
		bool m_ToBeDestroyed = false;
	};
}
