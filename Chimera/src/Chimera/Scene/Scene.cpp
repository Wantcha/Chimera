#include "cmpch.h"
#include "Scene.h"
#include "Entity.h"

#include "Components.h"
#include "Chimera/Renderer/Renderer2D.h"

#include <glm/glm.hpp>


namespace Chimera
{

	Scene::Scene()
	{
		b2Vec2 gravity(0, -9.81f);
		m_World = CreateScope<b2World>(gravity);

		g_debugDraw.Create();
		m_World->SetDebugDraw(&g_debugDraw);
		m_World->SetContactListener(&m_ContactListener);
		

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
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "New Entity" : name;
		
		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		auto physicsGroup = m_Registry.group<>(entt::get<TransformComponent, Body2DComponent>);
		for (auto entity : physicsGroup)
		{
			// TODO Check maybe if entity is enabled first
			auto [transform, bc] = physicsGroup.get<TransformComponent, Body2DComponent>(entity);

			bc.Body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);
			

			//Entity* userData = reinterpret_cast<Entity*>(rb.RigidBody.GetBody()->GetUserData().pointer);
			//CM_CORE_WARN("{0}", (uint32_t) *userData);
		}


		Renderer2D::BeginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent, TagComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite, tag] = group.get<TransformComponent, SpriteRendererComponent, TagComponent>(entity);

			if (tag.Enabled)
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		/*auto body = m_World->GetBodyList();
		while (body)
		{
			glm::vec2 pos = { body->GetTransform().p.x, body->GetTransform().p.y };

			Renderer2D::DrawRotatedQuad(0, pos, { 1.0f, 1.0f }, body->GetTransform().q.GetAngle(), { 0.5, 0.5, 1.0f, 1.0f });

			body = body->GetNext();
		}*/

		Renderer2D::EndScene();

		while (!m_DestructionStack.empty())
		{
			CM_CORE_ERROR("Destroying body...");
			m_World->DestroyBody(m_DestructionStack.top());
			m_DestructionStack.pop();
		}

		m_World->Step(1 / 60.0f, 6, 2);

		b2Body* body = m_World->GetBodyList();
		while (body != nullptr)
		{
			Entity* e = reinterpret_cast<Entity*>(body->GetUserData().pointer);
			TransformComponent& tc = e->GetComponent<TransformComponent>();
			tc.Position = { body->GetPosition().x, body->GetPosition().y, e->GetComponent<TransformComponent>().Position.z };
			tc.Rotation.z = body->GetAngle();
			body = body->GetNext();
		}

		if (m_EditColliders)
		{
			g_debugDraw.SetFlags(b2Draw::e_shapeBit);
			g_debugDraw.SetCamera(camera);
			m_World->DebugDraw();
			g_debugDraw.Flush();
		}
	}
	/*void Scene::OnUpdateRuntime(Timestep ts)
	{
		//Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}
					nsc.Instance->OnUpdate(ts);
				});
		}


		//Render 2D sprites

		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = transform.GetTransform();
				break;
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}*/
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

			component.BoxCollider.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(&component.BoxCollider);

			component.BoxCollider.SetBody(body.Body);
			component.BoxCollider.SetSize(transform.Scale.x, transform.Scale.y);

			body.Body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);
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

			BoxCollider2D* bc = &component.BoxCollider;
			component.BoxCollider.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(bc);


			component.BoxCollider.SetBody(body.Body);
			component.BoxCollider.SetSize(transform.Scale.x, transform.Scale.y);

			//Collider2D* colliderA = (Collider2D*)component.BoxCollider.GetFixtureDef().userData.pointer;

			body.Body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);
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

			component.CircleCollider.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(&component.CircleCollider);

			component.CircleCollider.SetBody(body.Body);
			component.CircleCollider.SetRadius(transform.Scale.x / 2.0f);

			body.Body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);
			++body.ColliderCount;
		}

		else
		{
			//CM_CORE_INFO("Lol");
			Body2DComponent& body = entity.AddComponent<Body2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.position.Set(0.0f, 0.0f);
			bodyDef.type = b2_staticBody;

			auto transform = entity.GetComponent<TransformComponent>();

			body.Body = m_World->CreateBody(&bodyDef);

			Entity* e = new Entity(entity);
			body.Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(e);

			component.CircleCollider.GetFixtureDef().userData.pointer = reinterpret_cast<uintptr_t>(&component.CircleCollider);

			component.CircleCollider.SetBody(body.Body);
			component.CircleCollider.SetRadius(transform.Scale.x / 2.0f);

			body.Body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);
			++body.ColliderCount;
		}
		
	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
		if (entity.HasComponent<Body2DComponent>())
		{
			Body2DComponent& body = entity.GetComponent<Body2DComponent>();

			component.RigidBody.SetBody(body.Body);

			component.RigidBody.SetGravityScale(1.0f);
			component.RigidBody.SetDiscreteCollision(true);
			component.RigidBody.SetFixedRotation(false);
			component.RigidBody.SetBodyType(RigidBody2D::Body2DType::Dynamic);
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
			body.Body->SetTransform({ transform.Position.x, transform.Position.y }, transform.Rotation.z);

			component.RigidBody.SetBody(body.Body);

			component.RigidBody.SetGravityScale(1.0f);
			component.RigidBody.SetDiscreteCollision(true);
			component.RigidBody.SetFixedRotation(false);
			component.RigidBody.SetBodyType(RigidBody2D::Body2DType::Dynamic);
			component.RigidBody.SetMass(1.0f);
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
		
		component.BoxCollider.DestroyCollider();

		//bool e = entity.HasComponent<RigidBody2D>();
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

		component.CircleCollider.DestroyCollider();

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
		Entity* e = reinterpret_cast<Entity*>(entity.GetComponent<Body2DComponent>().Body->GetUserData().pointer);
		delete e;
		m_DestructionStack.push(component.Body);
		//m_World->DestroyBody(component.Body);
		CM_CORE_ERROR("Removing Body2DComponent...");
	}

	template<>
	void Scene::OnComponentRemoved<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}
}