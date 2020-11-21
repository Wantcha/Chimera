#pragma once

#ifdef CM_PLATFORM_WINDOWS
	#ifdef CM_BUILD_DLL
		#define CHIMERA_API __declspec(dllexport)
	#else
		#define CHIMERA_API __declspec(dllimport)
	#endif
#else
	#error Chimera only supports Windows!

#endif

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

#define CM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


