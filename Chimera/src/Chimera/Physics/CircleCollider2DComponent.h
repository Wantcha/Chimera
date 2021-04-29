#pragma once

#include "Collider2D.h"

namespace Chimera
{
	class CircleCollider2DComponent : public Collider2D
	{
	public:
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent& other)
		{
			//CM_CORE_ERROR("Copying CircleCollider2DComponent...");
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
		CircleCollider2DComponent(CircleCollider2DComponent&& other) noexcept
		{
			//CM_CORE_ERROR("Moving CircleCollider2DComponent...");
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

		virtual ~CircleCollider2DComponent() = default;
		CircleCollider2DComponent(b2Body* body, float radius, bool isSensor = false, float friction = 0.3f, float bounciness = 0.0f, float density = 1.0f);

		virtual void SetCenter(float x, float y) override;
		virtual void SetCenter(glm::vec2 center) override { SetCenter(center.x, center.y); }

		void SetRadius(float radius) { m_Radius = radius; m_CircleColliderShape.m_radius = radius; UpdateFixture(); }

		virtual Collider2D* GetCollider() override { return this; }
		float GetRadius() { return m_Radius; }

		CircleCollider2DComponent& operator=(const CircleCollider2DComponent& other)
		{
			//CM_CORE_ERROR("Copying Operation CircleCollider2DComponent...");
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

		CircleCollider2DComponent& operator=(CircleCollider2DComponent&& other) noexcept
		{
			//CM_CORE_ERROR("Moving Operation CircleCollider2DComponent...");
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
