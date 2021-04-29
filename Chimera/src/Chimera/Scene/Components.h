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

		void SetPosition(glm::vec3 pos) { m_Position = pos; RecalcTransform();  }
		void SetRotation(glm::vec3 rot) { m_Rotation = rot; RecalcTransform(); }
		void SetScale(glm::vec3 scale) { m_Scale = scale; RecalcTransform(); }
		void SetLocalPosition(glm::vec3 pos) { m_LocalPosition = pos; RecalcLocalTransform();}
		void SetLocalRotation(glm::vec3 rot) { m_LocalRotation = rot; RecalcLocalTransform(); }
		void SetLocalScale(glm::vec3 scale) { m_LocalScale = scale; RecalcLocalTransform(); }

		glm::vec3 GetPosition() const { return m_Position; }
		glm::vec3 GetRotation() const { return m_Rotation; }
		glm::vec3 GetScale() const { return m_Scale; }
		glm::vec3 GetLocalPosition() const { return m_LocalPosition; }
		glm::vec3 GetLocalRotation() const { return m_LocalRotation; }
		glm::vec3 GetLocalScale() const { return m_LocalScale; }

		glm::mat4 GetLocalTransform() const
		{
			return m_LocalTransform;
			//glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

			//return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
		}

		glm::mat4 SetTransform(const glm::mat4& transform) 
		{
			m_Transform = transform;
			Math::DecomposeTransform(transform, m_Position, m_Rotation, m_Scale);
			RecalcLocalTransform();
		}

		glm::mat4 SetLocalTransform(const glm::mat4& transform)
		{
			m_LocalTransform = transform;
			Math::DecomposeTransform(transform, m_LocalPosition, m_LocalRotation, m_LocalScale);
			RecalcTransform();
		}

		glm::mat4 GetTransform()
		{
			return m_Transform;
		}

		glm::vec3 GetForward()
		{
			return glm::normalize(glm::quat(m_Rotation) * glm::vec3(1, 0, 0));
		}

		void LookAt(const glm::vec3& target, const glm::vec3& up = glm::vec3(0.f, 1.f, 0.f))
		{
			SetTransform(glm::lookAt(m_Position, target, up));
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
					oldGlobal = m_Parent.GetComponent<TransformComponent>().GetTransform();
				m_Parent = newParent;

				// Maintain consistent transform between reparenting operations
				glm::mat4 newTransform = glm::inverse(newParent.GetComponent<TransformComponent>().GetTransform()) * oldGlobal * GetLocalTransform();

				m_LocalTransform = newTransform;
				Math::DecomposeTransform(newTransform, m_LocalPosition, m_LocalRotation, m_LocalScale);

				m_CurrentEntity.RemoveRootNode();
				newParent.GetComponent<TransformComponent>().GetChildren().push_back(m_CurrentEntity);
			}
			else
			{
				if (m_Parent != nullptr)
				{
					glm::mat4 newTransform = m_Parent.GetComponent<TransformComponent>().GetTransform() * GetLocalTransform();
					m_LocalTransform = newTransform;
					Math::DecomposeTransform(newTransform, m_LocalPosition, m_LocalRotation, m_LocalScale);

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
			glm::mat4 rotation = glm::toMat4(glm::quat(m_LocalRotation));

			m_LocalTransform = glm::translate(glm::mat4(1.0f), m_LocalPosition) * rotation * glm::scale(glm::mat4(1.0f), m_LocalScale);

			m_Transform = m_Parent == nullptr ? GetLocalTransform() : m_Parent.GetComponent<TransformComponent>().GetTransform() * GetLocalTransform();
			Math::DecomposeTransform(m_Transform, m_Position, m_Rotation, m_Scale);

			for (Entity child : m_Children)
			{
				child.GetComponent<TransformComponent>().RecalcLocalTransform();
			}
		}

		void RecalcTransform()
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(m_Rotation));

			m_Transform = glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);

			glm::mat4 oldParent = glm::mat4(1.0f);
			if (m_Parent != nullptr)
				oldParent = m_Parent.GetComponent<TransformComponent>().GetTransform();

			m_LocalTransform = glm::inverse(oldParent) * GetTransform();
			Math::DecomposeTransform(m_LocalTransform, m_LocalPosition, m_LocalRotation, m_LocalScale);

			for (Entity child : m_Children)
			{
				child.GetComponent<TransformComponent>().RecalcTransform();
			}
		}

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 m_LocalPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_LocalRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_LocalScale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 m_LocalTransform = glm::mat4(1.0f);
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

		uint32_t GetWidth() const { return Camera.GetWidth(); }
		uint32_t GetHeight() const { return Camera.GetHeight(); }

		glm::vec3 ScreenToWorld(const glm::vec2& mousePosition, const glm::mat4& camTransform) { return Camera.ScreenToWorld(mousePosition, camTransform); }
	};

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