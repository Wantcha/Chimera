#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };


		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: Position(Position) {}

		/*Entity Parent;
		glm::mat4 WorldMatrix = glm::mat4(1.0f);
		std::vector<Entity> Children;*/

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
		/*glm::mat4 GetGlobalTransform()
		{
			//return Parent.IsNull() ? GetTransform() : WorldMatrix * GetTransform();
		}

		void SetParent(Entity parent)
		{
			Parent = parent;
			WorldMatrix = parent.GetComponent<TransformComponent>().GetGlobalTransform();
		}

		inline Entity GetParent()
		{
			return Parent;
		}*/
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{1.0f, 1.0f, 1.0f, 1.0f};
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
		float GetPerspFarClip() const { return Camera.GetPerspFarClip();}
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
		void ( *DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
