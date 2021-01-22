#pragma once

#include <memory>

//#ifdef CM_PLATFORM_WINDOWS
#ifdef CM_DYNAMIC_LINK
	#ifdef CM_BUILD_DLL
		#define CHIMERA_API __declspec(dllexport)
	#else
		#define CHIMERA_API __declspec(dllimport)
	#endif
#else
	#define CHIMERA_API
#endif
//#else
	//#error Chimera only supports Windows!

//#endif

#define BIT(x) (1<<x)

#if CM_DEBUG
	#define CM_ENABLE_ASSERTS
#endif

#ifdef CM_ENABLE_ASSERTS
	#define CM_ASSERT(x, ...) { if (!(x)) { CM_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak(); } }
	#define CM_CORE_ASSERT(x, ...) { if (!(x)) { CM_CORE_ERROR("Assertion Failed {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define CM_ASSERT(x, ...)
	#define CM_CORE_ASSERT(x, ...)
#endif // CM_ENABLE_ASSERTS

//#define CM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#define CM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); } //Creates a lambda

namespace Chimera
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	//Use these functions instead of std::make_unique or std::make_shared for cleaner code
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
