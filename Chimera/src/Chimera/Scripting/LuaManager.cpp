#include "cmpch.h"
#include "LuaManager.h"
#include "Chimera/Scene/Scene.h"
#include "Chimera/Scene/Components.h"
#include "Chimera/Core/Input.h"
#include "Chimera/Core/Keycodes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		m_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table);

		auto entity_type = m_State.new_usertype<Entity>("Entity", sol::no_constructor);
		entity_type["name"] = sol::property( &Entity::GetName, &Entity::SetName );

		BindMath();
		BindApp();
		BindInput();
		BindECS();
		BindScene();
		BindLog();
		BindPhysics();
	}
	
	void LuaManager::InitScripts()
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
	}
	void LuaManager::UpdateScripts(Timestep ts)
	{
		auto& registry = m_CurrentScene->m_Registry;

		auto view = registry.view<LuaScripts>();

		registry.view<LuaScripts>().each([=](auto entity, auto& ls)
			{
				for (Ref<LuaScriptComponent> lsc : ls.Scripts)
				{
					if (lsc->m_Env)
					{
						if (!lsc->m_HasBeenInitialized)
						{
							lsc->m_HasBeenInitialized = true;
							lsc->m_Entity = Entity{ entity, m_CurrentScene };
							(*(lsc->m_Env))["entity"] = lsc->m_Entity;
							lsc->OnInit();
						}

						lsc->OnUpdate(ts);
					}
				}
			});
	}
	void LuaManager::BindMath()
	{
		m_State.new_usertype<glm::vec2>("Vector2", sol::constructors<glm::vec2(float, float)>(),
			"x", &glm::vec2::x,
			"y", &glm::vec2::y,
			sol::meta_function::addition, [](const glm::vec2& l, const glm::vec2& r) { return glm::vec2(l.x + r.x, l.y + r.y); },
			sol::meta_function::subtraction, [](const glm::vec2& l, const glm::vec2& r) { return glm::vec2(l.x - r.x, l.y - r.y); }
			);

		m_State.new_usertype<glm::vec3>("Vector3", sol::constructors<glm::vec3(float, float, float)>(),
			"x", &glm::vec3::x,
			"y", &glm::vec3::y,
			"z", &glm::vec3::z,
			sol::meta_function::addition, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x + r.x, l.y + r.y, l.z + r.z); },
			sol::meta_function::subtraction, [](const glm::vec3& l, const glm::vec3& r) { return glm::vec3(l.x - r.x, l.y - r.y, l.z - r.z); }
		);
	}
	void LuaManager::BindECS()
	{
		auto entity_type = m_State["Entity"].get_or_create<sol::usertype<Entity>>();
		auto tagComponent_type = m_State.new_usertype<TagComponent>("TagComponent", sol::no_constructor);
		//entity_type["name"] = ;


		auto transformComponent_type = m_State.new_usertype<TransformComponent>("TransformComponent", sol::no_constructor,
			"position", &TransformComponent::Position,
			"rotation", &TransformComponent::Rotation,
			"scale", &TransformComponent::Scale);

		REGISTER_COMPONENT_WITH_ECS(m_State, TransformComponent);

		auto spriteRendererComponent_type = m_State.new_usertype<SpriteRendererComponent>("SpriteRendererComponent", sol::no_constructor,
			"color", &SpriteRendererComponent::Color);

		REGISTER_COMPONENT_WITH_ECS(m_State, SpriteRendererComponent);

		auto cameraComponent_type = m_State.new_usertype<CameraComponent>("CameraComponent", sol::no_constructor,
			"primary", &CameraComponent::Primary,
			"fixedAspectRatio", &CameraComponent::FixedAspectRatio,
			"SetOrthographic", &CameraComponent::SetOrthographic,
			"SetPerspective", &CameraComponent::SetPerspective,
			"GetFOV", &CameraComponent::GetFOV,
			"SetFOV", &CameraComponent::SetFOV,
			"GetOrthoSize", &CameraComponent::GetOrthographicSize,
			"SetOrthoSize", &CameraComponent::SetOrthographicSize,
			"GetPerspNear", &CameraComponent::GetPerspNearClip,
			"SetPerspNear", & CameraComponent::SetPerspNearClip,
			"GetPerspFar", &CameraComponent::GetPerspFarClip,
			"SetPerspFar", &CameraComponent::SetPerspFarClip,
			"GetOrthoNear", &CameraComponent::GetOrthoNearClip,
			"SetOrthoNear", &CameraComponent::SetOrthoNearClip,
			"GetOrthoFar", &CameraComponent::GetOrthoFarClip,
			"SetOrthoFar", &CameraComponent::SetOrthoFarClip);

		REGISTER_COMPONENT_WITH_ECS(m_State, CameraComponent);

		auto rigidBody2D_type = m_State.new_usertype<RigidBody2DComponent>("RigidBody2DComponent", sol::no_constructor,
			"gravityScale", sol::property(&RigidBody2DComponent::GetGravityScale, &RigidBody2DComponent::SetGravityScale),
			"discreteCollision", sol::property(&RigidBody2DComponent::IsDiscreteCollision, &RigidBody2DComponent::SetDiscreteCollision),
			"fixedRotation", sol::property(&RigidBody2DComponent::IsFixedRotation, &RigidBody2DComponent::SetFixedRotation),
			"bodyType", sol::property(&RigidBody2DComponent::GetBodyType, &RigidBody2DComponent::SetBodyType)
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
			"SetRadius", &CircleCollider2DComponent::SetRadius,
			"GetRadius", &CircleCollider2DComponent::GetRadius,
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
				return Input::GetMousePosition();
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
	}
	void LuaManager::BindApp()
	{
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
			"isSensor", sol::property( &Collider2D::IsSensor, &Collider2D::SetSensor ),
			"bounciness", sol::property( &Collider2D::GetBounciness, &Collider2D::SetBounciness ),
			"friction", sol::property( &Collider2D::GetFriction, &Collider2D::SetFriction ),
			"density", sol::property( &Collider2D::GetDensity, &Collider2D::SetDensity )/*,
			"GetEntity", &Collider2D::GetEntity*/);
	}
}