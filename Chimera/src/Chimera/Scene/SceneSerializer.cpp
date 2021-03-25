#include "cmpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Chimera
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;// Entity
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity; //Entity ID

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "Name" << YAML::Value << tag.Name;
			out << YAML::Key << "Enabled" << YAML::Value << tag.Enabled;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.GetPosition();
			out << YAML::Key << "Rotation" << YAML::Value << tc.GetRotation();
			out << YAML::Key << "Scale" << YAML::Value << tc.GetScale();
			if(tc.GetParent() == nullptr)
				out << YAML::Key << "Parent" << YAML::Value << (uint32_t)entt::null;
			else
				out << YAML::Key << "Parent" << YAML::Value << (uint32_t)tc.GetParent();
			
			out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;
			std::vector<Entity>& children = tc.GetChildren();
			for (Entity child : children)
			{
				out << YAML::BeginMap;

				out << YAML::Value << (uint32_t)child;

				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthoNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthoFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& src = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << src.Color;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << "RigidBody2DComponent" << YAML::Value;
			out << YAML::BeginMap;

			//out << YAML::Key << "RigidBody2DComponent" << YAML::Value;
			//out << YAML::BeginMap;

			auto& rb = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << (int)rb.GetBodyType();
			out << YAML::Key << "GravityScale" << YAML::Value << rb.GetGravityScale();
			out << YAML::Key << "DiscreteCollision" << YAML::Value << rb.IsDiscreteCollision();
			out << YAML::Key << "FixedRotation" << YAML::Value << rb.IsFixedRotation();

			//out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent" << YAML::Value;
			out << YAML::BeginMap;

			//out << YAML::Key << "BoxCollider2DComponent" << YAML::Value;
			//out << YAML::BeginMap;

			auto& box = entity.GetComponent<BoxCollider2DComponent>();
			glm::vec3 pos = { box.GetCenter().x, box.GetCenter().y, 0.0f };
			out << YAML::Key << "ColliderCenter" << YAML::Value << pos;
			out << YAML::Key << "ColliderSize" << YAML::Value << box.GetSize();
			out << YAML::Key << "Sensor" << YAML::Value << box.IsSensor();
			out << YAML::Key << "Density" << YAML::Value << box.GetDensity();
			out << YAML::Key << "Friction" << YAML::Value << box.GetFriction();
			out << YAML::Key << "Bounciness" << YAML::Value << box.GetBounciness();

			//out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent" << YAML::Value;
			out << YAML::BeginMap;

			//out << YAML::Key << "CircleCollider2DComponent" << YAML::Value;
			//out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CircleCollider2DComponent>();
			glm::vec3 pos = { cc.GetCenter().x, cc.GetCenter().y, 0.0f };
			out << YAML::Key << "ColliderCenter" << YAML::Value << pos;
			out << YAML::Key << "ColliderRadius" << YAML::Value << cc.GetRadius();
			out << YAML::Key << "Sensor" << YAML::Value << cc.IsSensor();
			out << YAML::Key << "Density" << YAML::Value << cc.GetDensity();
			out << YAML::Key << "Friction" << YAML::Value << cc.GetFriction();
			out << YAML::Key << "Bounciness" << YAML::Value << cc.GetBounciness();

			//out << YAML::EndMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<LuaScripts>())
		{
			out << YAML::Key << "LuaScripts" << YAML::Value << YAML::BeginSeq;

			std::vector<Ref<LuaScriptComponent>>& scripts = entity.GetComponent<LuaScripts>().Scripts;
			//out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
			for (Ref<LuaScriptComponent> lsc : scripts)
			{
				out << YAML::BeginMap;

				out << YAML::Key << "LuaScriptComponent" << YAML::Value;
				out << YAML::BeginMap;
				out << YAML::Key << "FilePath" << YAML::Value << lsc->GetFilePath();
				out << YAML::EndMap;

				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Hierarchy Roots" << YAML::Value << YAML::BeginSeq;
		std::vector<Entity>& roots = m_Scene->m_RootEntityList;
		for (Entity root : roots)
		{
			out << YAML::BeginMap;

			out << YAML::Value << (uint32_t)root;

			out << YAML::EndMap;
		}
		out << YAML::EndSeq;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
					return;

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		CM_CORE_ASSERT(false, "");
	}
	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CM_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint32_t uuid = entity["Entity"].as<uint32_t>();

				std::string name;
				bool enabled;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Name"].as<std::string>();
					enabled = tagComponent["Enabled"].as<bool>();
				}
					
				CM_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithID(uuid,name);
				deserializedEntity.GetComponent<TagComponent>().Enabled = enabled;

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.SetPosition(transformComponent["Position"].as<glm::vec3>());
					tc.SetRotation(transformComponent["Rotation"].as<glm::vec3>());
					tc.SetScale(transformComponent["Scale"].as<glm::vec3>());
					//tc.SetParent(Entity{ (entt::entity)transformComponent["Parent"].as<uint32_t>(), m_Scene.get() });
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					auto& cameraProps = cameraComponent["Camera"];

					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetFOV(cameraProps["FOV"].as<float>());
					cc.Camera.SetPerspNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthoNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthoFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent< SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				auto rigiBody2DComponent = entity["RigidBody2DComponent"];
				if (rigiBody2DComponent)
				{
					auto& rb = deserializedEntity.AddComponent<RigidBody2DComponent>();

					rb.SetBodyType((RigidBody2DComponent::Body2DType)rigiBody2DComponent["BodyType"].as<int>());
					rb.SetGravityScale(rigiBody2DComponent["GravityScale"].as<float>());
					rb.SetDiscreteCollision(rigiBody2DComponent["DiscreteCollision"].as<bool>());
					rb.SetFixedRotation(rigiBody2DComponent["FixedRotation"].as<bool>());
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& box = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					
					box.SetCenter(boxCollider2DComponent["ColliderCenter"].as<glm::vec3>());
					box.SetSize(boxCollider2DComponent["ColliderSize"].as<glm::vec2>());
					box.SetDensity(boxCollider2DComponent["Density"].as<float>());
					box.SetFriction(boxCollider2DComponent["Friction"].as<float>());
					box.SetBounciness(boxCollider2DComponent["Bounciness"].as<float>());
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CircleCollider2DComponent>();

					cc.SetCenter(circleCollider2DComponent["ColliderCenter"].as<glm::vec3>());
					cc.SetRadius(circleCollider2DComponent["ColliderRadius"].as<float>());
					cc.SetDensity(circleCollider2DComponent["Density"].as<float>());
					cc.SetFriction(circleCollider2DComponent["Friction"].as<float>());
					cc.SetBounciness(circleCollider2DComponent["Bounciness"].as<float>());
				}

				auto scripts = entity["LuaScripts"];
				if (scripts)
				{
					for (auto script : scripts)
					{
						std::string filePath = script["LuaScriptComponent"]["FilePath"].as<std::string>();
						if (deserializedEntity.HasComponent<LuaScripts>())
						{
							deserializedEntity.GetComponent<LuaScripts>().Scripts.push_back(CreateRef<LuaScriptComponent>(filePath));
						}

						else
						{
							LuaScripts& ls = deserializedEntity.AddComponent<LuaScripts>();
							ls.Scripts.push_back(CreateRef<LuaScriptComponent>(filePath));
						}
					}
				}
			}
			for (auto entity : entities)
			{
				uint32_t uuid = entity["Entity"].as<uint32_t>();
				Entity deserializedEntity = Entity{ (entt::entity)uuid, m_Scene.get() };

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					auto children = transformComponent["Children"];
					for (auto child : children)
					{
						Entity e = Entity{ (entt::entity)child.as<uint32_t>(), m_Scene.get() };
						e.GetComponent<TransformComponent>().SetParent(deserializedEntity);
					}
				}
			}

		}
		auto roots = data["Hierarchy Roots"];
		if (roots)
		{
			std::vector<Entity> newOrder;
			for (auto root : roots)
			{
				entt::entity handle = (entt::entity)root.as<uint32_t>();
				Entity entity = Entity{ handle, m_Scene.get() };
				newOrder.push_back(entity);
			}
			m_Scene->m_RootEntityList = newOrder;
		}
		return true;
	}
	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		CM_CORE_ASSERT(false, "");
		return false;
	}
}