#include "cmpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Chimera/Assets/AssetManager.h"

#include "Components.h"
#include "Chimera/Renderer/Renderer2D.h"

#include <glm/glm.hpp>
#include "Chimera/Scripting/LuaManager.h"

#include "Chimera/Events/ApplicationEvent.h"


namespace Chimera
{

	Scene::Scene()
	{
		b2Vec2 gravity(0, -9.81f);
		m_World = CreateScope<b2World>(gravity);

		g_debugDraw.Create();
		m_World->SetDebugDraw(&g_debugDraw);
		m_World->SetContactListener(&m_ContactListener);
		m_World->SetAllowSleeping(false);

		LuaManager::Get().Init(this);
		/*m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, this };
				if (entity.GetComponent<TransformComponent>().Parent == nullptr)
					m_RootEntityList.push_back(entity);
			}
		);*/
		// LuaManager::Get().InitScripts();


		/*b2BodyDef boxBodyDef;
		boxBodyDef.position.Set(0.0f, 1.0f);
		boxBodyDef.type = b2_dynamicBody;
		b2Body* boxBody = m_World->CreateBody(&boxBodyDef);
		boxBody->SetAngularVelocity(0.1f);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(0.5f, 0.5f);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.0f;
		fixtureDef.shape = &boxShape;
		fixtureDef.friction = 0.3f;

		boxBody->CreateFixture(&fixtureDef);*/



		/*b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0.0f, -2.0f);
		b2Body* groundBody = m_World->CreateBody(&groundBodyDef);

		b2PolygonShape groundShape;
		groundShape.SetAsBox(10.0f, 0.5f);


		groundBody->CreateFixture(&groundShape, 0.0f);*/


	}
	Scene::~Scene()
	{
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		entity.AddComponent<TransformComponent>();
		/*Entity e = LuaManager::Get().GetEntityByName("Trei");
		if (e != nullptr)
		{
			TransformComponent tc = e.GetComponent<TransformComponent>();
			if (tc.GetParent())
				CM_CORE_WARN((uint32_t)tc.GetParent());
		}*/
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "New Entity" : name;

		m_RootEntityList.push_back(entity);

		return entity;
	}
	Entity Scene::CreateEntityWithID(uint32_t id, const std::string& name)
	{
		Entity entity = { m_Registry.create((entt::entity)id), this };

		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Name = name.empty() ? "New Entity" : name;

		m_RootEntityList.push_back(entity);

		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		if (entity.HasComponent<Body2DComponent>())
		{
			entity.RemoveComponent<Body2DComponent>();
		}
		std::vector<Entity> children = entity.GetComponent<TransformComponent>().GetChildren();

		for (Entity& child : children)
		{
			DestroyEntity(child);
		}

		//Entity aux = entity;
		Entity parent = entity.GetComponent<TransformComponent>().GetParent();
		if (parent == nullptr)
			RemoveRootEntity(entity);
		else
		{
			std::vector<Entity>& entitySiblings = parent.GetComponent<TransformComponent>().GetChildren();

			std::vector<Entity>::iterator position = std::find(entitySiblings.begin(), entitySiblings.end(), entity);
			if (position != entitySiblings.end())
				entitySiblings.erase(position);
		}
		//aux.SetNullScene();
		m_Registry.destroy(entity);
	}
	void Scene::OnRenderEditor(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent, TagComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite, tag] = group.get<TransformComponent, SpriteRendererComponent, TagComponent>(entity);

			if (tag.Enabled)
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		if (m_EditColliders)
		{
			g_debugDraw.SetFlags(b2Draw::e_shapeBit);
			g_debugDraw.SetCamera(camera);
			m_World->DebugDraw();
			g_debugDraw.Flush();
		}

		Renderer2D::EndScene();
	}
	void Scene::OnUpdateEditor(Timestep ts)
	{
		//LuaManager::Get().UpdateScripts(ts);

		auto physicsGroup = m_Registry.group<>(entt::get<TransformComponent, Body2DComponent>);
		for (auto entity : physicsGroup)
		{
			// TODO Check maybe if entity is enabled first
			auto [transform, bc] = physicsGroup.get<TransformComponent, Body2DComponent>(entity);

			bc.Body->SetTransform({ transform.GetPosition().x, transform.GetPosition().y }, transform.GetRotation().z);

			//Entity* userData = reinterpret_cast<Entity*>(rb.RigidBody.GetBody()->GetUserData().pointer);
			//CM_CORE_WARN("{0}", (uint32_t) *userData);
		}

		while (!m_DestructionStack.empty())
		{
			Entity* e = reinterpret_cast<Entity*>(m_DestructionStack.top()->GetUserData().pointer);
			delete e;
			//CM_CORE_ERROR("Destroying body...");
			m_World->DestroyBody(m_DestructionStack.top());
			m_DestructionStack.pop();
		}
	}
	void Scene::OnFixedUpdate(float fixedts)
	{
		LuaManager::Get().FixedUpdateScripts(fixedts);
		m_World->Step(fixedts, 6, 2);

		b2Body* body = m_World->GetBodyList();
		while (body != nullptr)
		{
			Entity* e = reinterpret_cast<Entity*>(body->GetUserData().pointer);
			TransformComponent& tc = e->GetComponent<TransformComponent>();
			tc.SetPosition({ body->GetPosition().x, body->GetPosition().y, tc.GetPosition().z });
			tc.SetRotation({ tc.GetRotation().x, tc.GetRotation().y, body->GetAngle() });
			body = body->GetNext();
		}
	}
	void Scene::OnRenderRuntime()
	{
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		float cameraZ;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.GetTransform();
				cameraZ = transform.GetPosition().z;
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent, TagComponent>);
			group.sort<TransformComponent>([&](const TransformComponent& a, const TransformComponent& b)
				{
					// ONLY GOOD FOR 2D! Normally, we would need to get the distance from the camera, but for a 2D context where the camera
					// is only moving on the XY plane and the Z is more or less static (also the camera doesn't rotate), this works just fine.
					return a.GetPosition().z < b.GetPosition().z;
				});

			for (auto entity : group)
			{
				auto [transform, sprite, tag] = group.get<TransformComponent, SpriteRendererComponent, TagComponent>(entity);

				if (tag.Enabled)
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}

			Renderer2D::EndScene();
		}
	}
	void Scene::OnUpdateRuntime(Timestep ts)
	{
		LuaManager::Get().UpdateScripts(ts);

		auto physicsGroup = m_Registry.group<>(entt::get<TransformComponent, Body2DComponent>);
		for (auto entity : physicsGroup)
		{
			// TODO Check maybe if entity is enabled first
			auto [transform, bc] = physicsGroup.get<TransformComponent, Body2DComponent>(entity);

			bc.Body->SetTransform({ transform.GetPosition().x, transform.GetPosition().y }, transform.GetRotation().z);

		}

		while (!m_DestructionStack.empty())
		{
			//CM_CORE_ERROR("Destroying body...");
			m_World->DestroyBody(m_DestructionStack.top());
			m_DestructionStack.pop();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		//Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	/*void Scene::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<WindowResizeEvent>(CM_BIND_EVENT_FN(Scene::OnViewportResize));
	}*/

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::MoveRootEntity(Entity entity, uint32_t position)
	{
		RemoveRootEntity(entity);
		InsertRootEntity(entity, position);
	}

	void Scene::PushRootEntity(Entity entity)
	{
		m_RootEntityList.push_back(entity);
	}

	void Scene::InsertRootEntity(Entity entity, uint32_t position)
	{
		m_RootEntityList.insert(m_RootEntityList.begin() + position, entity);
	}

	void Scene::RemoveRootEntity(Entity entity)
	{
		std::vector<Entity>::iterator position = std::find(m_RootEntityList.begin(), m_RootEntityList.end(), entity);
		if (position != m_RootEntityList.end())
			m_RootEntityList.erase(position);
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
		component.SetEntity(entity);
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		if (entity.HasComponent<Body2DComponent>())
		{
			Body2DComponent& body = entity.GetComponent<Body2DComponent>();

			auto transform = entity.GetComponent<TransformComponent>();

			component.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(&component);

			component.SetBody(body.Body);
			component.SetSize(transform.GetScale().x, transform.GetScale().y);

			body.Body->SetTransform({ transform.GetPosition().x, transform.GetPosition().y }, transform.GetRotation().z);
			++body.ColliderCount;
		}

		else
		{
			Body2DComponent& body = entity.AddComponent<Body2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.position.Set(0.0f, 0.0f);
			bodyDef.type = b2_staticBody;

			auto transform = entity.GetComponent<TransformComponent>();

			body.Body = m_World->CreateBody(&bodyDef);

			Entity* e = new Entity(entity);
			body.Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(e);

			BoxCollider2DComponent* bc = &component;
			component.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(bc);


			component.SetBody(body.Body);
			component.SetSize(transform.GetScale().x, transform.GetScale().y);

			//Collider2D* colliderA = (Collider2D*)component.BoxCollider.GetFixtureDef().userData.pointer;

			body.Body->SetTransform({ transform.GetPosition().x, transform.GetPosition().y }, transform.GetRotation().z);
			++body.ColliderCount;
		}
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
		if (entity.HasComponent<Body2DComponent>())
		{
			Body2DComponent& body = entity.GetComponent<Body2DComponent>();

			auto transform = entity.GetComponent<TransformComponent>();

			component.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(&component);

			component.SetBody(body.Body);
			component.SetRadius(transform.GetScale().x / 2.0f);

			body.Body->SetTransform({ transform.GetPosition().x, transform.GetPosition().y }, transform.GetRotation().z);
			++body.ColliderCount;
		}

		else
		{
			Body2DComponent& body = entity.AddComponent<Body2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.position.Set(0.0f, 0.0f);
			bodyDef.type = b2_staticBody;

			auto transform = entity.GetComponent<TransformComponent>();

			body.Body = m_World->CreateBody(&bodyDef);

			Entity* e = new Entity(entity);
			body.Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(e);

			component.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(&component);

			component.SetBody(body.Body);
			component.SetRadius(transform.GetScale().x / 2.0f);

			body.Body->SetTransform({ transform.GetPosition().x, transform.GetPosition().y }, transform.GetRotation().z);
			++body.ColliderCount;
		}

	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
		if (entity.HasComponent<Body2DComponent>())
		{
			Body2DComponent& body = entity.GetComponent<Body2DComponent>();

			component.SetBody(body.Body);

			component.SetGravityScale(1.0f);
			component.SetDiscreteCollision(true);
			component.SetFixedRotation(false);
			component.SetBodyType(RigidBody2DComponent::Body2DType::Dynamic);
		}

		else
		{
			Body2DComponent& body = entity.AddComponent<Body2DComponent>();
			b2BodyDef bodyDef;
			bodyDef.position.Set(0.0f, 0.0f);
			bodyDef.type = b2_dynamicBody;

			body.Body = m_World->CreateBody(&bodyDef);

			Entity* e = new Entity(entity);

			body.Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(e);
			//uintptr_t* p = &body.Body->GetUserData().pointer;
			//Entity* userData = reinterpret_cast<Entity*>(body.Body->GetUserData().pointer);

			auto transform = entity.GetComponent<TransformComponent>();
			body.Body->SetTransform({ transform.GetPosition().x, transform.GetPosition().y }, transform.GetRotation().z);

			component.SetBody(body.Body);

			component.SetGravityScale(1.0f);
			component.SetDiscreteCollision(true);
			component.SetFixedRotation(false);
			component.SetBodyType(RigidBody2DComponent::Body2DType::Dynamic);
			component.SetMass(1.0f);
			//userData = reinterpret_cast<Entity*>(body.Body->GetUserData().pointer);
			//CM_CORE_INFO("Test");
		}

	}

	template<>
	void Scene::OnComponentAdded<Body2DComponent>(Entity entity, Body2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<LuaScriptComponent>(Entity entity, LuaScriptComponent& component)
	{
		//component.OnInit();

	}

	template<>
	void Scene::OnComponentAdded<LuaScripts>(Entity entity, LuaScripts& component)
	{
		//component.OnInit();

	}


	// On Component Removed template specializations
	template<typename T>
	void Scene::OnComponentRemoved(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentRemoved<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentRemoved<CameraComponent>(Entity entity, CameraComponent& component)
	{

	}

	template<>
	void Scene::OnComponentRemoved<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentRemoved<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
		Body2DComponent& body = entity.GetComponent<Body2DComponent>();
		body.ColliderCount--;

		component.DestroyCollider();

		//bool e = entity.HasComponent<RigidBody2DComponent>();
		if (body.ColliderCount == 0 && !entity.HasComponent<RigidBody2DComponent>())
		{
			entity.RemoveComponent<Body2DComponent>();
		}
	}

	template<>
	void Scene::OnComponentRemoved<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
		Body2DComponent& body = entity.GetComponent<Body2DComponent>();
		body.ColliderCount--;

		component.DestroyCollider();

		if (body.ColliderCount == 0 && !entity.HasComponent<RigidBody2DComponent>())
		{
			entity.RemoveComponent<Body2DComponent>();
		}
	}

	template<>
	void Scene::OnComponentRemoved<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
		if (entity.GetComponent<Body2DComponent>().ColliderCount == 0)
		{
			entity.RemoveComponent<Body2DComponent>();
		}

	}

	template<>
	void Scene::OnComponentRemoved<Body2DComponent>(Entity entity, Body2DComponent& component)
	{
		//Entity* e = reinterpret_cast<Entity*>(entity.GetComponent<Body2DComponent>().Body->GetUserData().pointer);
		//delete e;
		m_DestructionStack.push(component.Body);
		//m_World->DestroyBody(component.Body);
		//CM_CORE_ERROR("Removing Body2DComponent...");
	}

	template<>
	void Scene::OnComponentRemoved<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}
	template<>
	void Scene::OnComponentRemoved<LuaScriptComponent>(Entity entity, LuaScriptComponent& component)
	{

	}

	template<>
	void Scene::OnComponentRemoved<LuaScripts>(Entity entity, LuaScripts& component)
	{
		//component.OnInit();

	}
}