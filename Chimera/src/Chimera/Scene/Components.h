#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Chimera/Physics/BoxCollider2D.h"
#include "Chimera/Physics/RigidBody2D.h"
#include "Chimera/Physics/CircleCollider2D.h"

#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Chimera/Renderer/Texture.h"

namespace Chimera
{
	struct TagComponent
	{
		std::string Tag;
		bool Enabled = true;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
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

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
		}
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
	};

	struct BoxCollider2DComponent
	{
		BoxCollider2D BoxCollider;
		//bool EditMode = false;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
		BoxCollider2DComponent(b2Body* body, float width, float height)
			: BoxCollider( body, width, height ) {}
	};

	struct CircleCollider2DComponent
	{
		CircleCollider2D CircleCollider;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
		CircleCollider2DComponent(b2Body* body, float radius)
			: CircleCollider(body, radius) {}
	};

	struct RigidBody2DComponent
	{
		RigidBody2D RigidBody;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;
		RigidBody2DComponent(b2Body* body)
			: RigidBody(body) {}
	};

	struct Body2DComponent
	{
		b2Body* Body;
		int ColliderCount = 0;
		Body2DComponent() = default;
		Body2DComponent(const Body2DComponent&) = default;
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
