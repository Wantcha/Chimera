#pragma once
#include "Chimera/Core/Timestep.h"
#include <sol/sol.hpp>
#include "Chimera/Scene/Entity.h"
#include "Chimera/Physics/ContactListener2D.h"

namespace Chimera
{
	class Scene;

	class LuaScriptComponent
	{
	public:
		LuaScriptComponent();
		LuaScriptComponent(const std::string& filePath);
		~LuaScriptComponent();

		void Initialize();
		void OnInit();
		void OnUpdate(Timestep ts);
		void OnFixedUpdate(float fixedts);
		void OnCollisionEnter2D(Contact2D contact);
		void OnCollisionExit2D(Contact2D contact);
		void OnSensorEnter2D(Collider2D* other);
		void OnSensorExit2D(Collider2D* other);

		void Load(const std::string& filePath);
		void Unload();
		void LoadScript(const std::string& fileName);

		const Ref<sol::environment> GetSolEnvironment() const
		{
			return m_Env;
		}

		const std::string& GetName() const
		{
			return m_Name;
		}

		const std::string& GetFilePath() const
		{
			return m_FilePath;
		}

		void SetFilePath(const std::string& path)
		{
			m_FilePath = path;
		}

		const std::vector<std::string>& GetErrors() const
		{
			return m_Errors;
		}

		bool Loaded()
		{
			return m_Env.get() != nullptr;
		}

	private:
		std::string m_FilePath;
		std::string m_Name;
		Entity m_Entity;

		std::vector<std::string> m_Errors;
		Ref<sol::environment> m_Env;
		Ref<sol::protected_function> m_OnInitFunc;
		Ref<sol::protected_function> m_OnUpdateFunc;
		Ref<sol::protected_function> m_OnFixedUpdateFunc;
		Ref<sol::protected_function> m_OnCollisionEnter2DFunc;
		Ref<sol::protected_function> m_OnCollisionExit2DFunc;
		Ref<sol::protected_function> m_OnSensorEnter2DFunc;
		Ref<sol::protected_function> m_OnSensorExit2DFunc;

		bool m_HasBeenInitialized = false;

		friend class LuaManager;
	};
}