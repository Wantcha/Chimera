#pragma once

#include "Collider2D.h"

namespace Chimera
{
	class BoxCollider2D : public Collider2D
	{
	public:
		BoxCollider2D() = default;
		BoxCollider2D(const BoxCollider2D& other)
		{
			CM_CORE_ERROR("Copying BoxCollider2D...");
			m_Body = other.m_Body;
			m_BoxColliderShape = other.m_BoxColliderShape;
			m_ToBeDestroyed = other.m_ToBeDestroyed;
			m_Bounciness = other.m_Bounciness;
			m_CenterX = other.m_CenterX;
			m_CenterY = other.m_CenterY;
			m_Friction = other.m_Friction;
			m_IsSensor = other.m_IsSensor;
			m_Fixture = other.m_Fixture;
			m_FixtureDef = other.m_FixtureDef;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			m_Density = other.m_Density;
			m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

			m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { m_CenterX, m_CenterY }, 0.0f);
			m_FixtureDef.shape = &m_BoxColliderShape;
			UpdateFixtureDef();

			if (!m_ToBeDestroyed)
			{
				UpdateFixture();
			}
			//m_Fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
		}
		BoxCollider2D(BoxCollider2D&& other) noexcept
		{
			CM_CORE_ERROR("Moving BoxCollider2D...");
			m_Body = other.m_Body;
			m_BoxColliderShape = other.m_BoxColliderShape;
			m_ToBeDestroyed = other.m_ToBeDestroyed;
			m_Bounciness = other.m_Bounciness;
			m_CenterX = other.m_CenterX;
			m_CenterY = other.m_CenterY;
			m_Friction = other.m_Friction;
			m_IsSensor = other.m_IsSensor;
			m_Fixture = other.m_Fixture;
			m_FixtureDef = other.m_FixtureDef;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			m_Density = other.m_Density;
			m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

			m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { m_CenterX, m_CenterY }, 0.0f);
			m_FixtureDef.shape = &m_BoxColliderShape;
			UpdateFixtureDef();
			if (!m_ToBeDestroyed)
			{
				UpdateFixture();
			}
			//m_Fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

			other.m_Body = nullptr;
			other.m_Fixture = nullptr;
		}
		virtual ~BoxCollider2D() = default;
		BoxCollider2D(b2Body* body, float width, float height, bool isSensor = false, float friction = 0.3f, float bounciness = 0.0f, float density = 1.0f);

		virtual void SetCenter(float x, float y) override;
		virtual void SetCenter(glm::vec3 center) override { SetCenter(center.x, center.y); }
		void SetSize(float width, float height);
		void SetSize(glm::vec2 size) { SetSize(size.x, size.y); };
		void SetWidth(float width) { SetSize(width, m_Height); }
		void SetHeight(float height) { SetSize(m_Width, height); }

		virtual Collider2D* GetCollider() override { return this; }
		glm::vec2 GetSize() { return { m_Width, m_Height }; }
		float GetWidth() { return m_Width; }
		float GetHeight() { return m_Height; }

		b2PolygonShape& GetBoxShape() { return m_BoxColliderShape; }

		BoxCollider2D& operator=(const BoxCollider2D& other)
		{
			CM_CORE_ERROR("Copying Operation BoxCollider2D...");
			if (this != &other)
			{
				m_Body = other.m_Body;
				m_BoxColliderShape = other.m_BoxColliderShape;
				m_ToBeDestroyed = other.m_ToBeDestroyed;
				m_Bounciness = other.m_Bounciness;
				m_CenterX = other.m_CenterX;
				m_CenterY = other.m_CenterY;
				m_Friction = other.m_Friction;
				m_IsSensor = other.m_IsSensor;
				m_Fixture = other.m_Fixture;
				m_FixtureDef = other.m_FixtureDef;
				m_Width = other.m_Width;
				m_Height = other.m_Height;
				m_Density = other.m_Density;
				m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { m_CenterX, m_CenterY }, 0.0f);
				m_FixtureDef.shape = &m_BoxColliderShape;
				UpdateFixtureDef();
				if (!m_ToBeDestroyed)
				{
					UpdateFixture();
				}
				//m_Fixture->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
			}
			return *this;
		}

		BoxCollider2D& operator=(BoxCollider2D&& other) noexcept
		{
			CM_CORE_ERROR("Moving Operation BoxCollider2D...");
			if (this != &other)
			{
				m_Body = other.m_Body;
				m_BoxColliderShape = other.m_BoxColliderShape;
				m_ToBeDestroyed = other.m_ToBeDestroyed;
				m_Bounciness = other.m_Bounciness;
				m_CenterX = other.m_CenterX;
				m_CenterY = other.m_CenterY;
				m_Friction = other.m_Friction;
				m_IsSensor = other.m_IsSensor;
				m_Fixture = other.m_Fixture;
				m_FixtureDef = other.m_FixtureDef;
				m_Width = other.m_Width;
				m_Height = other.m_Height;
				m_Density = other.m_Density;
				m_FixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(this);

				m_BoxColliderShape.SetAsBox(m_Width / 2.0f, m_Height / 2.0f, { m_CenterX, m_CenterY }, 0.0f);
				m_FixtureDef.shape = &m_BoxColliderShape;
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

		b2PolygonShape m_BoxColliderShape;
		float m_Width = 0.5f, m_Height = 0.5f;
	};
}
