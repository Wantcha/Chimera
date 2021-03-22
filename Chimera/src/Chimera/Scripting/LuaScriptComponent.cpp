#include "cmpch.h"
#include "LuaScriptComponent.h"
#include "LuaManager.h"

namespace Chimera
{
	LuaScriptComponent::LuaScriptComponent()
	{
		m_FilePath = "";
		m_Name = "";
		m_Env = nullptr;
	}
	LuaScriptComponent::LuaScriptComponent(const std::string& filePath)
	{
		m_FilePath = filePath;
		Initialize();
	}
	LuaScriptComponent::~LuaScriptComponent()
	{
		if (m_Env)
		{
			sol::protected_function releaseFunc = (*m_Env)["Release"];
			if (releaseFunc.valid())
				releaseFunc.call();
		}
	}
	void LuaScriptComponent::Initialize()
	{
		auto lastSlash = m_FilePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = m_FilePath.rfind('.');
		auto count = lastDot == std::string::npos ? m_FilePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = m_FilePath.substr(lastSlash, count);

		LoadScript(m_FilePath);
	}
	void LuaScriptComponent::OnInit()
	{
		if (m_OnInitFunc)
		{
			sol::protected_function_result result = m_OnInitFunc->call();
			if (!result.valid())
			{
				sol::error err = result;
				CM_CORE_ERROR("Failed to execute Lua Script: Init function");
				CM_CORE_ERROR("Error : {0}", err.what());
			}
		}
	}
	void LuaScriptComponent::OnUpdate(Timestep ts)
	{
		if (m_OnUpdateFunc)
		{
			sol::protected_function_result result = m_OnUpdateFunc->call(float(ts));
			if (!result.valid())
			{
				sol::error err = result;
				CM_CORE_ERROR("Failed to execute Lua Script: Update function");
				CM_CORE_ERROR("Error : {0}", err.what());
			}
		}
	}
	void LuaScriptComponent::OnCollisionEnter2D(Contact2D contact)
	{
		if (m_OnCollisionEnter2DFunc)
		{
			sol::protected_function_result result = m_OnCollisionEnter2DFunc->call(contact);
			if (!result.valid())
			{
				sol::error err = result;
				CM_CORE_ERROR("Failed to execute Lua Script: OnCollisionEnter2D function");
				CM_CORE_ERROR("Error : {0}", err.what());
			}
		}
	}
	void LuaScriptComponent::OnCollisionExit2D(Contact2D contact)
	{
		if (m_OnCollisionExit2DFunc)
		{
			sol::protected_function_result result = m_OnCollisionExit2DFunc->call(contact);
			if (!result.valid())
			{
				sol::error err = result;
				CM_CORE_ERROR("Failed to execute Lua Script: OnCollisionExit2D function");
				CM_CORE_ERROR("Error : {0}", err.what());
			}
		}
	}
	void LuaScriptComponent::OnSensorEnter2D(Collider2D* other)
	{
		if (m_OnSensorEnter2DFunc)
		{
			sol::protected_function_result result = m_OnSensorEnter2DFunc->call(other);
			if (!result.valid())
			{
				sol::error err = result;
				CM_CORE_ERROR("Failed to execute Lua Script: OnSensorEnter2D function");
				CM_CORE_ERROR("Error : {0}", err.what());
			}
		}
	}
	void LuaScriptComponent::OnSensorExit2D(Collider2D* other)
	{
		if (m_OnSensorExit2DFunc)
		{
			sol::protected_function_result result = m_OnSensorExit2DFunc->call(other);
			if (!result.valid())
			{
				sol::error err = result;
				CM_CORE_ERROR("Failed to execute Lua Script: OnSensorExit2D function");
				CM_CORE_ERROR("Error : {0}", err.what());
			}
		}
	}
	void LuaScriptComponent::Load(const std::string& fileName)
	{
		if (m_Env)
		{
			sol::protected_function releaseFunc = (*m_Env)["Release"];
			if (releaseFunc.valid())
				releaseFunc.call();
		}
		m_FilePath = fileName;
		m_HasBeenInitialized = false;
		Initialize();
	}
	void LuaScriptComponent::LoadScript(const std::string& fileName)
	{
		m_Env = CreateRef<sol::environment>(LuaManager::Get().GetState(), sol::create, LuaManager::Get().GetState().globals());

		// auto entity = LuaManager::Get().GetEntityFromScript(*this);
		// (*m_Env)["entity"] = m_Entity;

		auto loadFileResult = LuaManager::Get().GetState().script_file(fileName, *m_Env, sol::script_pass_on_error);
		if (!loadFileResult.valid())
		{
			sol::error err = loadFileResult;
			CM_CORE_ERROR("Failed to execute Lua script {0}", m_FilePath);
			CM_CORE_ERROR("Error: {0}", err.what());
			m_Errors.push_back(std::string(err.what()));
		}

		m_OnInitFunc = CreateRef<sol::protected_function>((*m_Env)["Init"]);
		if (!m_OnInitFunc->valid())
			m_OnInitFunc.reset();

		m_OnUpdateFunc = CreateRef<sol::protected_function>((*m_Env)["Update"]);
		if (!m_OnUpdateFunc->valid())
			m_OnUpdateFunc.reset();

		m_OnCollisionEnter2DFunc = CreateRef<sol::protected_function>((*m_Env)["CollisionEnter2D"]);
		if (!m_OnCollisionEnter2DFunc->valid())
			m_OnCollisionEnter2DFunc.reset();

		m_OnCollisionExit2DFunc = CreateRef<sol::protected_function>((*m_Env)["CollisionExit2D"]);
		if (!m_OnCollisionExit2DFunc->valid())
			m_OnCollisionExit2DFunc.reset();

		m_OnSensorEnter2DFunc = CreateRef<sol::protected_function>((*m_Env)["SensorEnter2D"]);
		if (!m_OnSensorEnter2DFunc->valid())
			m_OnSensorEnter2DFunc.reset();

		m_OnSensorExit2DFunc = CreateRef<sol::protected_function>((*m_Env)["SensorExit2D"]);
		if (!m_OnSensorExit2DFunc->valid())
			m_OnSensorExit2DFunc.reset();

		LuaManager::Get().GetState().collect_garbage();
	}
}