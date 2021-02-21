#pragma once
#include "Entity.h"
#include "Chimera/Physics/ContactListener2D.h"

namespace Chimera
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {};

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnCollisionEnter2D(Contact2D contact) {}
		virtual void OnCollisionExit2D(Contact2D contact) {}
		virtual void OnSensorEnter2D(Collider2D* other) {}
		virtual void OnSensorExit2D(Collider2D* other) {}
	private:
		Entity m_Entity;
		friend class Scene;
		friend class ContactListener2D;
	};
}
