#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Chimera/Math/Math.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Chimera/Physics/BoxCollider2DComponent.h"
#include "Chimera/Physics/RigidBody2DComponent.h"
#include "Chimera/Physics/CircleCollider2DComponent.h"

#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Chimera/Renderer/Texture.h"

#include "Chimera/Scripting/LuaScriptComponent.h"

namespace Chimera
{
	struct TagComponent
	{
		std::string Name;
		bool Enabled = true;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& name)
			: Name(name) {}

		void SetName(std::string n)
		{
			Name = n;
		}

		std::string GetName() { return Name; }
	};

	class TransformComponent
	{
	public:
		TransformComponent() {};
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: m_Position(position) {}

		void SetPosition(glm::vec3 pos) { m_Position = pos; RecalcLocalTransform(); /*CM_CORE_ERROR("LOL");*/ }
		void SetRotation(glm::vec3 rot) { m_Rotation = rot; RecalcLocalTransform(); }
		void SetScale(glm::vec3 scale) { m_Scale = scale; RecalcLocalTransform(); }

		glm::vec3& GetPosition() { return m_Position; }
		glm::vec3& GetRotation() { return m_Rotation; }
		glm::vec3& GetScale() { return m_Scale; }

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

			return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
		}
		glm::mat4 GetGlobalTransform()
		{
			return m_Parent == nullptr ? GetTransform() : m_Parent.GetComponent<TransformComponent>().GetGlobalTransform() * GetTransform();
		}

		void SetParent(Entity newParent)
		{
			if (m_Parent != nullptr)
			{
				std::vector<Entity>& children = m_Parent.GetComponent<TransformComponent>().m_Children;

				std::vector<Entity>::iterator position = std::find(children.begin(), children.end(), m_CurrentEntity);
				if (position != children.end())
					children.erase(position);
			}

			if (newParent != nullptr)
			{
				if (IsAlreadyUpTheHierarchy(m_CurrentEntity, newParent))
				{
					for (Entity& child : m_Children)
						child.GetComponent<TransformComponent>().SetParent(m_Parent);
				}

				glm::mat4 oldGlobal = glm::mat4(1.0f);
				if(m_Parent != nullptr)
					oldGlobal = m_Parent.GetComponent<TransformComponent>().GetGlobalTransform();
				m_Parent = newParent;

				// Maintain consistent transform between reparenting operations
				glm::mat4 newTransform = glm::inverse(newParent.GetComponent<TransformComponent>().GetGlobalTransform()) * oldGlobal * GetTransform();

				Math::DecomposeTransform(newTransform, m_Position, m_Rotation, m_Scale);

				m_CurrentEntity.RemoveRootNode();
				newParent.GetComponent<TransformComponent>().GetChildren().push_back(m_CurrentEntity);
			}
			else
			{
				if (m_Parent != nullptr)
				{
					glm::mat4 newTransform = m_Parent.GetComponent<TransformComponent>().GetGlobalTransform() * GetTransform();
					Math::DecomposeTransform(newTransform, m_Position, m_Rotation, m_Scale);

					m_Parent = nullptr;
					m_CurrentEntity.MakeRootNode();
				}
			}
		}

		inline Entity GetParent()
		{
			return m_Parent;
		}

		inline void SetEntity(Entity e)
		{
			m_CurrentEntity = e;
		}
		inline Entity GetEntity() const
		{
			return m_CurrentEntity;
		}
		inline void PushChild(Entity entity)
		{
			m_Children.push_back(entity);
		}
		std::vector<Entity>& GetChildren() { return m_Children; }

	private:
		bool IsAlreadyUpTheHierarchy(Entity entityToCheck, Entity entity)
		{
			Entity newParent = entity.GetComponent<TransformComponent>().GetParent();
			if (newParent == nullptr)
				return false;
			if (newParent == entityToCheck)
				return true;
			return IsAlreadyUpTheHierarchy(entityToCheck, newParent);
		}

		void RecalcLocalTransform()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

			m_Transform = glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
		}

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
		glm::mat4 m_Transform = glm::mat4(1.0f);

		Entity m_CurrentEntity;
		Entity m_Parent = nullptr;
		glm::mat4 m_WorldMatrix = glm::mat4(1.0f);
		std::vector<Entity> m_Children;
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> SpriteTexture = Texture2D::Create(1, 1);

		SpriteRendererComponent()
		{
			uint32_t whiteTextureData = 0xffffffff;
			SpriteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		};
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {
			uint32_t whiteTextureData = 0xffffffff;
			SpriteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; //Could make the scene be responsible for this
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		void SetPerspective(float fov, float nearClip, float farClip) { Camera.SetPerspective(fov, nearClip, farClip); }
		void SetOrthographic(float size, float nearClip, float farClip)
		{
			Camera.SetOrthographic(size, nearClip, farClip);
		}
		float GetFOV() { return Camera.GetFOV(); }
		void SetFOV(float fov) { Camera.SetFOV(fov); }
		float GetPerspNearClip() const { return Camera.GetPerspNearClip(); }
		float GetPerspFarClip() const { return Camera.GetPerspFarClip(); }
		void SetPerspNearClip(float nearClip) { Camera.SetPerspNearClip(nearClip); }
		void SetPerspFarClip(float farClip) { Camera.SetPerspFarClip(farClip); }

		float GetOrthographicSize() { return Camera.GetOrthographicSize(); }
		void SetOrthographicSize(float size) { Camera.SetOrthographicSize(size); }
		float GetOrthoNearClip() const { return Camera.GetOrthoNearClip(); }
		float GetOrthoFarClip() const { return Camera.GetOrthoFarClip(); }
		void SetOrthoNearClip(float nearClip) { Camera.SetOrthoNearClip(nearClip); }
		void SetOrthoFarClip(float farClip) { Camera.SetOrthoFarClip(farClip); }

	};

	/*struct BoxCollider2DComponent
	{
		BoxCollider2DComponent BoxCollider;
		//bool EditMode = false;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
		BoxCollider2DComponent(b2Body* body, float width, float height)
			: BoxCollider( body, width, height ) {}
	};*/

	/*struct CircleCollider2DComponent
	{
		CircleCollider2DComponent CircleCollider;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
		CircleCollider2DComponent(b2Body* body, float radius)
			: CircleCollider(body, radius) {}
	};*/

	/*struct RigidBody2DComponent
	{
		RigidBody2DComponent RigidBody;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
		RigidBody2DComponent(b2Body* body)
			: RigidBody(body) {}
	};*/

	struct Body2DComponent
	{
		b2Body* Body;
		int ColliderCount = 0;
		Body2DComponent() = default;
		Body2DComponent(const Body2DComponent&) = default;
	};

	struct LuaScripts
	{
		std::vector<Ref<LuaScriptComponent>> Scripts;

		LuaScripts() = default;
		LuaScripts(const LuaScripts&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}