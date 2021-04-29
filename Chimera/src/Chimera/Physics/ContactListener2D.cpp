#include "cmpch.h"
#include "ContactListener2D.h"
#include "Chimera/Scene/Components.h"

namespace Chimera
{
	void ContactListener2D::BeginContact(b2Contact* contact)
	{
		Contact2D c;
		Collider2D* colliderA = (Collider2D*)contact->GetFixtureA()->GetUserData().pointer;
		Collider2D* colliderB = (Collider2D*)contact->GetFixtureB()->GetUserData().pointer;

		Entity* entityA = (Entity*)colliderA->GetBody()->GetUserData().pointer;
		Entity* entityB = (Entity*)colliderB->GetBody()->GetUserData().pointer;

		if (entityA->HasComponent<LuaScripts>())
		{
			LuaScripts& ls = entityA->GetComponent<LuaScripts>();
			//CM_CORE_ERROR("CONTACTBEGFIN");
			for (Ref<LuaScriptComponent> lsc : ls.Scripts)
			{
				if (lsc->GetSolEnvironment() != nullptr)
				{
					if (contact->GetFixtureB()->IsSensor())
					{
						lsc->OnSensorEnter2D(colliderB);
					}

					else
					{
						c.Entity = entityB;
						int points = contact->GetManifold()->pointCount;

						b2WorldManifold wm;
						contact->GetWorldManifold(&wm);

						for (int i = 0; i < points; i++) {
							c.ContactPositions[i] = glm::vec3{ wm.points[i].x, wm.points[i].y, 0.0f };
						}
						c.Normal = glm::vec2{ wm.normal.x, wm.normal.y };
						c.Collider = colliderA;
						c.OtherCollider = colliderB;

						lsc->OnCollisionEnter2D(c);
					}

				}
			}

		}

		if (entityB->HasComponent<LuaScripts>())
		{
			LuaScripts& ls = entityB->GetComponent<LuaScripts>();
			for (Ref<LuaScriptComponent> lsc : ls.Scripts)
			{
				if (lsc->GetSolEnvironment() != nullptr)
				{
					if (contact->GetFixtureA()->IsSensor())
					{
						lsc->OnSensorEnter2D(colliderA);
					}

					else
					{
						c.Entity = entityA;
						int points = contact->GetManifold()->pointCount;

						b2WorldManifold wm;
						contact->GetWorldManifold(&wm);

						for (int i = 0; i < points; i++) {
							c.ContactPositions[i] = glm::vec3{ wm.points[i].x, wm.points[i].y, 0.0f };
						}
						c.Normal = glm::vec2{ wm.normal.x, wm.normal.y };
						c.Collider = colliderB;
						c.OtherCollider = colliderA;

						lsc->OnCollisionEnter2D(c);
					}

				}
			}
		}
	}
	void ContactListener2D::EndContact(b2Contact* contact)
	{
		Contact2D c;
		Collider2D* colliderA = (Collider2D*)contact->GetFixtureA()->GetUserData().pointer;
		Collider2D* colliderB = (Collider2D*)contact->GetFixtureB()->GetUserData().pointer;

		Entity* entityA = (Entity*)colliderA->GetBody()->GetUserData().pointer;
		Entity* entityB = (Entity*)colliderB->GetBody()->GetUserData().pointer;


		if (entityA->HasComponent<LuaScripts>())
		{
			LuaScripts& ls = entityA->GetComponent<LuaScripts>();
			for (Ref<LuaScriptComponent> lsc : ls.Scripts)
			{
				if (lsc->GetSolEnvironment() != nullptr)
				{
					if (contact->GetFixtureA()->IsSensor())
					{
						lsc->OnSensorExit2D(colliderB);
					}

					else
					{
						c.Entity = entityA;
						int points = contact->GetManifold()->pointCount;

						b2WorldManifold wm;
						contact->GetWorldManifold(&wm);

						for (int i = 0; i < points; i++) {
							c.ContactPositions[i] = glm::vec3{ wm.points[i].x, wm.points[i].y, 0.0f };
						}
						c.Normal = glm::vec2{ wm.normal.x, wm.normal.y };
						c.Collider = colliderA;
						c.OtherCollider = colliderB;

						lsc->OnCollisionExit2D(c);
					}

				}
			}
		}

		if (entityB->HasComponent<LuaScripts>())
		{
			LuaScripts& ls = entityB->GetComponent<LuaScripts>();
			for (Ref<LuaScriptComponent> lsc : ls.Scripts)
			{
				if (lsc->GetSolEnvironment() != nullptr)
				{
					if (contact->GetFixtureB()->IsSensor())
					{
						lsc->OnSensorExit2D(colliderA);
					}

					else
					{
						c.Entity = entityB;
						int points = contact->GetManifold()->pointCount;

						b2WorldManifold wm;
						contact->GetWorldManifold(&wm);

						for (int i = 0; i < points; i++) {
							c.ContactPositions[i] = glm::vec3{ wm.points[i].x, wm.points[i].y, 0.0f };
						}
						c.Normal = glm::vec2{ wm.normal.x, wm.normal.y };
						c.Collider = colliderB;
						c.OtherCollider = colliderA;

						lsc->OnCollisionExit2D(c);
					}

				}
			}

		}
	}
}