#pragma once

#include "Collider2D.h"

namespace Chimera
{
	class CircleCollider2D : public Collider2D
	{
	public:
		CircleCollider2D() = default;
		CircleCollider2D(const CircleCollider2D& other)
		{
			CM_CORE_ERROR("Copying CircleCollider2D...");
			m_Body = other.m_Body;
			m_CircleColliderShape = other.m_CircleColliderShape;
			m_ToBeDestroyed = other.m_ToBeDestroyed;
			m_Bounciness = other.m_Bounciness;
			m_CenterX = other.m_CenterX;
			m_CenterY = other.m_CenterY;
			m_Friction = other.m_Friction;
			m_IsSensor = other.m_IsSensor;
			m_Fixture = other.m_Fixture;
			m_FixtureDef = other.m_FixtureDef;
			m_Radius = other.m_Radius;
			m_Density = other.m_Density;
			m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

			m_CircleColliderShape.m_p.Set(m_CenterX, m_CenterY);
			m_CircleColliderShape.m_radius = m_Radius;
			m_FixtureDef.shape = &m_CircleColliderShape;
			UpdateFixtureDef();
			if (!m_ToBeDestroyed)
			{
				UpdateFixture();
			}
			//m_Fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
		}
		CircleCollider2D(CircleCollider2D&& other) noexcept
		{
			CM_CORE_ERROR("Moving CircleCollider2D...");
			m_Body = other.m_Body;
			m_CircleColliderShape = other.m_CircleColliderShape;
			m_ToBeDestroyed = other.m_ToBeDestroyed;
			m_Bounciness = other.m_Bounciness;
			m_CenterX = other.m_CenterX;
			m_CenterY = other.m_CenterY;
			m_Friction = other.m_Friction;
			m_IsSensor = other.m_IsSensor;
			m_Fixture = other.m_Fixture;
			m_FixtureDef = other.m_FixtureDef;
			m_Radius = other.m_Radius;
			m_Density = other.m_Density;
			m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

			m_CircleColliderShape.m_p.Set(m_CenterX, m_CenterY);
			m_CircleColliderShape.m_radius = m_Radius;
			m_FixtureDef.shape = &m_CircleColliderShape;
			UpdateFixtureDef();
			if (!m_ToBeDestroyed)
			{
				UpdateFixture();
			}
			//m_Fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

			other.m_Body = nullptr;
			other.m_Fixture = nullptr;
		}

		virtual ~CircleCollider2D() = default;
		CircleCollider2D(b2Body* body, float radius, bool isSensor = false, float friction = 0.3f, float bounciness = 0.0f, float density = 1.0f);

		virtual void SetCenter(float x, float y) override;
		virtual void SetCenter(glm::vec3 center) override { SetCenter(center.x, center.y); }

		void SetRadius(float radius) { m_Radius = radius; m_CircleColliderShape.m_radius = radius; UpdateFixture(); }

		virtual Collider2D* GetCollider() override { return this; }
		float GetRadius() { return m_Radius; }

		CircleCollider2D& operator=(const CircleCollider2D& other)
		{
			CM_CORE_ERROR("Copying Operation CircleCollider2D...");
			if (this != &other)
			{
				m_Body = other.m_Body;
				m_CircleColliderShape = other.m_CircleColliderShape;
				m_ToBeDestroyed = other.m_ToBeDestroyed;
				m_Bounciness = other.m_Bounciness;
				m_CenterX = other.m_CenterX;
				m_CenterY = other.m_CenterY;
				m_Friction = other.m_Friction;
				m_IsSensor = other.m_IsSensor;
				m_Fixture = other.m_Fixture;
				m_FixtureDef = other.m_FixtureDef;
				m_Radius = other.m_Radius;
				m_Density = other.m_Density;
				m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				m_CircleColliderShape.m_p.Set(m_CenterX, m_CenterY);
				m_CircleColliderShape.m_radius = m_Radius;
				m_FixtureDef.shape = &m_CircleColliderShape;
				UpdateFixtureDef();
				if (!m_ToBeDestroyed)
				{
					UpdateFixture();
				}
				//m_Fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
			}
			return *this;
		}

		CircleCollider2D& operator=(CircleCollider2D&& other) noexcept
		{
			CM_CORE_ERROR("Moving Operation CircleCollider2D...");
			if (this != &other)
			{
				m_Body = other.m_Body;
				m_CircleColliderShape = other.m_CircleColliderShape;
				m_ToBeDestroyed = other.m_ToBeDestroyed;
				m_Bounciness = other.m_Bounciness;
				m_CenterX = other.m_CenterX;
				m_CenterY = other.m_CenterY;
				m_Friction = other.m_Friction;
				m_IsSensor = other.m_IsSensor;
				m_Fixture = other.m_Fixture;
				m_FixtureDef = other.m_FixtureDef;
				m_Radius = other.m_Radius;
				m_Density = other.m_Density;
				m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				m_CircleColliderShape.m_p.Set(m_CenterX, m_CenterY);
				m_CircleColliderShape.m_radius = m_Radius;
				m_FixtureDef.shape = &m_CircleColliderShape;
				UpdateFixtureDef();
				if (!m_ToBeDestroyed)
				{
					UpdateFixture();
				}
				//m_Fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

				other.m_Body = nullptr;
				other.m_Fixture = nullptr;
			}
			return *this;
		}
	private:
		virtual void UpdateFixture() override;

		float m_Radius = 0.5f;
		b2CircleShape m_CircleColliderShape;
	};
}
