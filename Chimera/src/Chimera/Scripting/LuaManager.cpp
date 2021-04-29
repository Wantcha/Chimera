#include "cmpch.h"
#include "LuaManager.h"
#include "Chimera/Core/Application.h"
#include "Chimera/Scene/Scene.h"
#include "Chimera/Scene/SceneManager.h"
#include "Chimera/Scene/Components.h"
#include "Chimera/Core/Input.h"
#include "Chimera/Core/Keycodes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Chimera
{
	LuaManager* LuaManager::s_Instance = nullptr;
	LuaManager::LuaManager()
		:m_State(nullptr)
	{
		s_Instance = this;
	}
	LuaManager::~LuaManager()
	{

	}
	void LuaManager::Init(Scene* scene)
	{
		m_CurrentScene = scene;
	}
	void LuaManager::InitBindings()
	{
		m_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::os);

		auto entity_type = m_State.new_usertype<Entity>("Entity", sol::no_constructor);
		entity_type["name"] = sol::property( &Entity::GetName, &Entity::SetName );
		entity_type["enabled"] = sol::property( &Entity::IsEnabled, &Entity::SetEnabled );
		entity_type["GetLuaScriptComponent"] = [](Entity e, int index) {
			std::vector<Ref<LuaScriptComponent>>& scripts = e.GetComponent<LuaScripts>().Scripts;
			return *scripts[index]->GetSolEnvironment();
			//CM_CORE_ERROR("SALAMA");
		};

		m_State.set_function("GetEntityByName", &LuaManager::GetEntityByName, this);
		m_State.set_function("CreateEmptyEntity", &LuaManager::CreateEmptyEntity, this);
		m_State.set_function("SpawnWrap", &LuaManager::SpawnWrap, this);
		m_State.set_function("Destroy", &LuaManager::Destroy, this);

		BindMath();
		BindApp();
		BindInput();
		BindECS();
		BindScene();
		BindLog();
		BindPhysics();
	}
	
	/*void LuaManager::InitScripts()
	{
		auto& registry = m_CurrentScene->m_Registry;

		auto view = registry.view<LuaScripts>();
		registry.view<LuaScripts>().each([=](auto entity, auto& ls)
		{
			for (Ref<LuaScriptComponent> lsc : ls.Scripts)
			{
				if (!lsc->m_HasBeenInitialized)
				{
					lsc->m_HasBeenInitialized = true;
					lsc->m_Entity = Entity{ entity, m_CurrentScene };
					lsc->OnInit();
				}
			}

		});
	}*/
	void LuaManager::UpdateScripts(Timestep ts)
	{
		auto& registry = m_CurrentScene->m_Registry;

		auto view = registry.view<LuaScripts>();

		view.each([&](auto entity, auto& ls)
			{
				std::vector<Ref<LuaScriptComponent>> scripts = ls.Scripts;
				for (Ref<LuaScriptComponent> lsc : scripts)
				{
					if (lsc->m_Env)
					{
						if (!lsc->m_HasBeenInitialized)
						{
							lsc->m_Entity = Entity{ entity, m_CurrentScene };
							(*(lsc->m_Env))["entity"] = lsc->m_Entity;
							if (lsc->m_Entity.IsEnabled())
							{
								lsc->OnInit();
								lsc->m_HasBeenInitialized = true;
							}
								
						}
						//CM_CORE_ERROR(lsc->m_Entity.GetName());
						if(lsc->m_Entity.IsEnabled())
							lsc->OnUpdate(ts);
					}
				}
			});
	}
	void LuaManager::FixedUpdateScripts(float fixedts)
	{
		auto& registry = m_CurrentScene->m_Registry;

		//auto view = registry.view<LuaScripts>();

		registry.view<LuaScripts>().each([=](auto entity, auto& ls)
			{
				for (Ref<LuaScriptComponent> lsc : ls.Scripts)
				{
					if (lsc->m_HasBeenInitialized)
					{
						if (lsc->m_Entity.IsEnabled())
							lsc->OnFixedUpdate(fixedts);
					}
				}
			});
	}
	void LuaManager::RefreshScripts()
	{
		auto& registry = m_CurrentScene->m_Registry;
		registry.view<LuaScripts>().each([=](auto entity, auto& ls)
			{
				for (Ref<LuaScriptComponent> lsc : ls.Scripts)
				{
					lsc->Load(lsc->GetFilePath());
				}
			});
	}
	void LuaManager::BindMath()
	{
		m_State.new_usertype<glm::vec2>("Vector2", sol::constructors<glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			sol::meta_function::addition, [](const glm::vec2& l, const glm::vec2& r) { return glm::vec2(l.x + r.x, l.y + r.y); },
			sol::meta_function::subtraction, [](const glm::vec2& l, const glm::vec2& r) { return glm::vec2(l.x - r.x, l.y - r.y); },
			sol::meta_function::multiplication, [](const glm::vec2& l, const float r) { return glm::vec2(l.x * r, l.y * r); },
			"GetLength", [](const glm::vec2& v) -> float { return glm::length(v); }
			);

		m_State.new_usertype<glm::vec3>("Vector3", sol::constructors<glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			sol::meta_function::addition, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x + r.x, l.y + r.y, l.z + r.z); },
			sol::meta_function::subtraction, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x - r.x, l.y - r.y, l.z - r.z); },
			sol::meta_function::multiplication, [](const glm::vec3& l, const float r) { return glm::vec3(l.x * r, l.y * r, l.z * r); },
			"GetLength", [](const glm::vec3& v) -> float { return glm::length(v); }
		);

		m_State.new_usertype<glm::quat>("Quaternion", sol::constructors<glm::quat(float, float, float, float)>(),
			"x", &glm::quat::x,
			"y", &glm::quat::y,
			"z", &glm::quat::z,
			"w", &glm::quat::w,
			"ToEuler", [](glm::quat& q) -> glm::vec3 { return glm::eulerAngles(q); }//,
			//sol::meta_function::addition, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x + r.x, l.y + r.y, l.z + r.z); },
			//sol::meta_function::subtraction, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x - r.x, l.y - r.y, l.z - r.z); },
			//sol::meta_function::multiplication, [](const glm::vec3& l, const float r) { return glm::vec3(l.x * r, l.y * r, l.z * r); }
		);

		m_State.new_usertype<glm::mat4>("Matrix4", sol::no_constructor
			//,
			//sol::meta_function::addition, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x + r.x, l.y + r.y, l.z + r.z); },
			//sol::meta_function::subtraction, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x - r.x, l.y - r.y, l.z - r.z); },
			//sol::meta_function::multiplication, [](const glm::vec3& l, const float r) { return glm::vec3(l.x * r, l.y * r, l.z * r); }
		);

		auto vec3 = m_State.create_table("Vec3");
		vec3.set_function("RotateTowards", [](sol::this_state s, const glm::vec3& forwardDirection, const glm::vec3& target) -> glm::vec3
			{
				glm::vec3 pos = glm::normalize(forwardDirection);
				glm::vec3 targetVec = glm::normalize(target);

				/*if (pos == targetVec || pos == - targetVec)
				{
					return glm::eulerAngles( glm::quat(glm::mat4(1.0f)) );
				}*/
				float dotProduct = glm::dot(pos, targetVec);
				if (dotProduct > 1)
					dotProduct = 1;
				else if (dotProduct < -1)
					dotProduct = -1;
				float rotationAngle = (float)glm::acos(dotProduct);
				glm::vec3 rotAxis = glm::normalize(glm::cross(pos, targetVec));

				glm::quat rotQuat = glm::rotate(glm::mat4(1.0f), rotationAngle, rotAxis);
				//CM_CORE_WARN(glm::degrees(rotationAngle));
				//CM_CORE_ERROR(glm::degrees( glm::eulerAngles(rotQuat)).z);
				if (std::isnan(glm::degrees(glm::eulerAngles(rotQuat)).z))
				{
					return glm::eulerAngles(glm::quat(glm::mat4(1.0f)));
				}
					
				return glm::eulerAngles(rotQuat);
			});
		vec3.set_function("Distance", [](sol::this_state s, const glm::vec3& position, const glm::vec3& target) -> float
			{
				return glm::distance(position, target);
			});
	}
	Entity LuaManager::GetEntityByName(const std::string& name)
	{
		auto& registry = m_CurrentScene->m_Registry;

		//auto view = registry.view<TagComponent>();
		entt::entity a = entt::null;
		registry.view<TagComponent>().each([&](auto entity, auto& tag)
			{
				if (tag.Name.compare(name) == 0)
				{
					a = entity;
				}
			});
		return Entity{ a, m_CurrentScene };
	}
	Entity LuaManager::CreateEmptyEntity(const std::string& name)
	{
		return m_CurrentScene->CreateEntity(name);
	}
	Entity LuaManager::SpawnWrap(const std::string& path)
	{
		//CM_CORE_WARN("Trying to spawn wrap: " + path);
		return SceneManager::Get().DeserializeEntity(path);
		//return Entity();
	}
	void LuaManager::Destroy(Entity e)
	{
		m_CurrentScene->DestroyEntity(e);
	}
	void LuaManager::BindECS()
	{
		auto entity_type = m_State["Entity"].get_or_create<sol::usertype<Entity>>();
		auto tagComponent_type = m_State.new_usertype<TagComponent>("TagComponent", sol::no_constructor);
		//entity_type["name"] = ;


		auto transformComponent_type = m_State.new_usertype<TransformComponent>("TransformComponent", sol::no_constructor,
			"position", sol::property( &TransformComponent::GetPosition, &TransformComponent::SetPosition),
			"rotation", sol::property(&TransformComponent::GetRotation, &TransformComponent::SetRotation),
			"scale", sol::property(&TransformComponent::GetScale, &TransformComponent::SetScale),
			"localPosition", sol::property(&TransformComponent::GetLocalPosition, &TransformComponent::SetLocalPosition),
			"localRotation", sol::property(&TransformComponent::GetLocalRotation, &TransformComponent::SetLocalRotation),
			"localScale", sol::property(&TransformComponent::GetLocalScale, &TransformComponent::SetLocalScale),
			"SetParent", &TransformComponent::SetParent,
			//"GetEntity", &TransformComponent::GetEntity,
			"GetParent", &TransformComponent::GetParent,
			"GetChildren", &TransformComponent::GetChildren,
			"GetForward", &TransformComponent::GetForward,
			"GetTransform", &TransformComponent::GetTransform,
			"GetLocalTransform", &TransformComponent::GetLocalTransform/*,
			"LookAt", &TransformComponent::LookAt*/);

		REGISTER_COMPONENT_WITH_ECS(m_State, TransformComponent);

		m_State.new_usertype<Texture2D>("Texture2D", sol::no_constructor,
			"Create", [](const std::string& path) -> Ref<Texture2D> { return Texture2D::Create(path); },
			"GetBoundsWidth", &Texture2D::GetBoundsWidth,
			"GetBoundsHeight", &Texture2D::GetBoundsHeight,
			"GetWidth", &Texture2D::GetWidth,
			"GetHeight", &Texture2D::GetHeight
			);

		auto spriteRendererComponent_type = m_State.new_usertype<SpriteRendererComponent>("SpriteRendererComponent", sol::no_constructor,
			"color", &SpriteRendererComponent::Color,
			"texture", &SpriteRendererComponent::SpriteTexture);

		REGISTER_COMPONENT_WITH_ECS(m_State, SpriteRendererComponent);

		auto cameraComponent_type = m_State.new_usertype<CameraComponent>("CameraComponent", sol::no_constructor,
			"primary", &CameraComponent::Primary,
			"fixedAspectRatio", &CameraComponent::FixedAspectRatio,
			"SetOrthographic", &CameraComponent::SetOrthographic,
			"SetPerspective", &CameraComponent::SetPerspective,
			"fov", sol::property( &CameraComponent::GetFOV, &CameraComponent::SetFOV),
			"orthoSize", sol::property(&CameraComponent::GetOrthographicSize, &CameraComponent::SetOrthographicSize),
			"perspectiveNear", sol::property(&CameraComponent::GetPerspNearClip, &CameraComponent::SetPerspNearClip),
			"perspectiveFar", sol::property(&CameraComponent::GetPerspFarClip, &CameraComponent::GetPerspFarClip),
			"orthoNear", sol::property(&CameraComponent::GetOrthoNearClip, &CameraComponent::SetOrthoNearClip),
			"orthoFar", sol::property(&CameraComponent::GetOrthoFarClip, &CameraComponent::SetOrthoFarClip),
			"GetWidth", &CameraComponent::GetWidth,
			"GetHeight", &CameraComponent::GetHeight,
			"ScreenToWorld", &CameraComponent::ScreenToWorld);

		REGISTER_COMPONENT_WITH_ECS(m_State, CameraComponent);

		auto rigidBody2D_type = m_State.new_usertype<RigidBody2DComponent>("RigidBody2DComponent", sol::no_constructor,
			"gravityScale", sol::property(&RigidBody2DComponent::GetGravityScale, &RigidBody2DComponent::SetGravityScale),
			"discreteCollision", sol::property(&RigidBody2DComponent::IsDiscreteCollision, &RigidBody2DComponent::SetDiscreteCollision),
			"fixedRotation", sol::property(&RigidBody2DComponent::IsFixedRotation, &RigidBody2DComponent::SetFixedRotation),
			"bodyType", sol::property(&RigidBody2DComponent::GetBodyType, &RigidBody2DComponent::SetBodyType),
			"velocity", sol::property(&RigidBody2DComponent::GetLinearVelocity, &RigidBody2DComponent::SetLinearVelocity),
			"ApplyForce", &RigidBody2DComponent::ApplyForce,
			"ApplyForceAtPoint", &RigidBody2DComponent::ApplyForceAtPoint
			);

		std::initializer_list<std::pair<sol::string_view, RigidBody2DComponent::Body2DType>> body2DTypes =
		{
			{"Static", RigidBody2DComponent::Body2DType::Static},
			{"Dynamic", RigidBody2DComponent::Body2DType::Dynamic},
			{"Kinematic", RigidBody2DComponent::Body2DType::Kinematic}
		};
		m_State.new_enum<RigidBody2DComponent::Body2DType, false>("Body2DType", body2DTypes);

		REGISTER_COMPONENT_WITH_ECS(m_State, RigidBody2DComponent);

		auto boxCollider2D_type = m_State.new_usertype<BoxCollider2DComponent>("BoxCollider2DComponent", sol::no_constructor,
			"isSensor", sol::property(&BoxCollider2DComponent::IsSensor, &BoxCollider2DComponent::SetSensor),
			"friction", sol::property(&BoxCollider2DComponent::GetFriction, &BoxCollider2DComponent::SetFriction),
			"density", sol::property(&BoxCollider2DComponent::GetDensity, &BoxCollider2DComponent::SetDensity),
			"bounciness", sol::property(&BoxCollider2DComponent::GetBounciness, &BoxCollider2DComponent::SetBounciness),
			"SetSize", sol::overload(static_cast<void(BoxCollider2DComponent::*)(glm::vec2)>(&BoxCollider2DComponent::SetSize),
									static_cast<void(BoxCollider2DComponent::*)(float, float)>(&BoxCollider2DComponent::SetSize)),
			"GetSize", &BoxCollider2DComponent::GetSize,
			"SetCenter", sol::overload(static_cast<void(BoxCollider2DComponent::*)(glm::vec2)>(&BoxCollider2DComponent::SetCenter),
										static_cast<void(BoxCollider2DComponent::*)(float, float)>(&BoxCollider2DComponent::SetCenter)),
			"GetCenter", &BoxCollider2DComponent::GetCenter
			);

		REGISTER_COMPONENT_WITH_ECS(m_State, BoxCollider2DComponent);

		auto circleCollider2D_type = m_State.new_usertype<CircleCollider2DComponent>("CircleCollider2DComponent", sol::no_constructor,
			"isSensor", sol::property(&CircleCollider2DComponent::IsSensor, &CircleCollider2DComponent::SetSensor),
			"friction", sol::property(&CircleCollider2DComponent::GetFriction, &CircleCollider2DComponent::SetFriction),
			"density", sol::property(&CircleCollider2DComponent::GetDensity, &CircleCollider2DComponent::SetDensity),
			"bounciness", sol::property(&CircleCollider2DComponent::GetBounciness, &CircleCollider2DComponent::SetBounciness),
			"radius", sol::property(&CircleCollider2DComponent::GetRadius, &CircleCollider2DComponent::SetRadius),
			"SetCenter", sol::overload(static_cast<void(CircleCollider2DComponent::*)(glm::vec2)>(&CircleCollider2DComponent::SetCenter),
										static_cast<void(CircleCollider2DComponent::*)(float, float)>(&CircleCollider2DComponent::SetCenter)),
			"GetCenter", &CircleCollider2DComponent::GetCenter
			);

		REGISTER_COMPONENT_WITH_ECS(m_State, CircleCollider2DComponent);
	}
	void LuaManager::BindInput()
	{
		auto input = m_State["Input"].get_or_create<sol::table>();

		input.set_function("IsKeyDown", [](KeyCode key) -> bool
			{
				return Input::IsKeyDown(key);
			});

		input.set_function("IsKeyPressed", [](KeyCode key) -> bool
			{
				return Input::IsKeyPressed(key);
			});
		input.set_function("IsMousePressed", [](int button) -> bool
			{
				return Input::IsMouseButtonPressed(button);
			});
		input.set_function("GetMousePosition", []() -> std::pair<float, float>
			{
				std::pair<float, float> mPos = Input::GetMousePosition();
				glm::vec2 gameWindowOffset = Application::Get().GetGameWindowOffset();
				return { std::get<0>(mPos) - gameWindowOffset.x, Application::Get().GetWindow().GetHeight() - std::get<1>(mPos) - gameWindowOffset.y };
			});

		std::initializer_list<std::pair<sol::string_view, KeyCode>> keyItems =
		{
			{"A", Key::A},
			{"B", Key::B},
			{"C", Key::C},
			{"D", Key::D},
			{"E", Key::E},
			{"F", Key::F},
			{"G", Key::G},
			{"H", Key::H},
			{"I", Key::I},
			{"J", Key::J},
			{"K", Key::K},
			{"L", Key::L},
			{"M", Key::M},
			{"N", Key::N},
			{"O", Key::O},
			{"P", Key::P},
			{"Q", Key::Q},
			{"R", Key::R},
			{"S", Key::S},
			{"T", Key::T},
			{"U", Key::U},
			{"V", Key::V},
			{"W", Key::W},
			{"X", Key::X},
			{"Y", Key::Y},
			{"Z", Key::Z},
			{"Space", Key::Space},
			{"Escape", Key::Escape},
			{"Apostrophe", Key::Apostrophe},
			{"Comma", Key::Comma},
			{"Minus", Key::Minus},
			{"Equal", Key::Equal},
			{"Period", Key::Period},
			{"Slash", Key::Slash},
			{"Semicolon", Key::Semicolon},
			{"LeftBracket", Key::LeftBracket},
			{"Backslash", Key::BackSlash},
			{"RightBracket", Key::RightBracket},
			{"Enter", Key::Enter},
			{"Tab", Key::Tab},
			{"Backspace", Key::Backspace},
			{"Insert", Key::Insert},
			{"Delete", Key::Delete},
			{"Right", Key::Right},
			{"Left", Key::Left},
			{"Down", Key::Down},
			{"Up", Key::Up},
			{"PageUp", Key::PageUp},
			{"PageDown", Key::PageDown},
			{"Home", Key::Home},
			{"End", Key::End},
			{"CapsLock", Key::CapsLock},
			{"ScrollLock", Key::ScrollLock},
			{"NumLock", Key::NumLock},
			{"Pause", Key::Pause},
			{"LeftShift", Key::LeftShift},
			{"LeftControl", Key::LeftControl},
			{"LeftAlt", Key::LeftAlt},
			{"PrintScreen", Key::PrintScreen},
			{"RightSuper", Key::RightSuper},
			{"RightShift", Key::RightShift},
			{"RightControl", Key::RightControl},
			{"RightAlt", Key::RightAlt},
			{"RightSuper", Key::RightSuper},
			{"Menu", Key::MENU},
			{"F1", Key::F1},
			{"F2", Key::F2},
			{"F3", Key::F3},
			{"F4", Key::F4},
			{"F5", Key::F5},
			{"F6", Key::F6},
			{"F7", Key::F7},
			{"F8", Key::F8},
			{"F9", Key::F9},
			{"F10", Key::F10},
			{"F11", Key::F11},
			{"F12", Key::F12},
			{"Keypad0", Key::KP0},
			{"Keypad1", Key::KP1},
			{"Keypad2", Key::KP2},
			{"Keypad3", Key::KP3},
			{"Keypad4", Key::KP4},
			{"Keypad5", Key::KP5},
			{"Keypad6", Key::KP6},
			{"Keypad7", Key::KP7},
			{"Keypad8", Key::KP8},
			{"Keypad9", Key::KP9},
			{"Multiply", Key::KPMultiply},
			{"Add", Key::KPAdd},
			{"KeypadEqual", Key::KPEqual},
			{"KeypadEnter", Key::KPEnter}
		};

		m_State.new_enum<KeyCode, false>("Key", keyItems);

	}
	void LuaManager::BindScene()
	{
		//static const std::string sceneManagerKey = "SceneManager::Get()";
		SceneManager& manager = SceneManager::Get();

		auto sm = m_State.create_table("SceneManager");
		sm.set_function("LoadScene", [&](sol::this_state s, int index) { manager.LoadScene(index); });
		/*m_State.new_usertype<SceneManager>("SceneManager", sol::no_constructor,
			"LoadScene", &SceneManager::LoadScene);*/

		//m_State.set("sceneManager", &manager);
	}
	void LuaManager::BindApp()
	{
		auto window = m_State["Window"].get_or_create<sol::table>();
		window.set_function("GetWidth", [](sol::this_state s) -> uint32_t { return Application::Get().GetWindow().GetWidth(); });
		window.set_function("GetHeight", [](sol::this_state s) -> uint32_t { return Application::Get().GetWindow().GetHeight(); });
	}
	void LuaManager::BindLog()
	{
		auto log = m_State.create_table("Log");
		log.set_function("Warn", [&](sol::this_state s, std::string_view message)
			{
				CM_WARN(message);
			});
	}
	void LuaManager::BindPhysics()
	{
		m_State.new_usertype<Contact2D>("Contact2D", sol::no_constructor,
			"entity", &Contact2D::Entity,
			"collider", &Contact2D::Collider,
			"otherCollider", &Contact2D::OtherCollider,
			"contactCount", &Contact2D::ContactCount,
			"contactPositions", &Contact2D::ContactPositions,
			"normal", &Contact2D::Normal);

		m_State.new_usertype<Collider2D>("Collider2D", sol::no_constructor,
			"isSensor", sol::property(&Collider2D::IsSensor, &Collider2D::SetSensor),
			"bounciness", sol::property(&Collider2D::GetBounciness, &Collider2D::SetBounciness),
			"friction", sol::property(&Collider2D::GetFriction, &Collider2D::SetFriction),
			"density", sol::property(&Collider2D::GetDensity, &Collider2D::SetDensity),
			"GetEntity", &Collider2D::GetEntity);
	}
}