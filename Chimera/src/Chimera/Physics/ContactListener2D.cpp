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


		if (entityA->HasComponent<NativeScriptComponent>())
		{
			auto& nsc = entityA->GetComponent<NativeScriptComponent>();
			if (nsc.Instance != nullptr)
			{
				if (contact->GetFixtureA()->IsSensor())
				{
					nsc.Instance->OnSensorEnter2D(colliderB);
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

					nsc.Instance->OnCollisionEnter2D(c);
				}

			}

		}

		if (entityB->HasComponent<NativeScriptComponent>())
		{
			auto& nsc = entityB->GetComponent<NativeScriptComponent>();
			if (nsc.Instance != nullptr)
			{
				if (contact->GetFixtureB()->IsSensor())
				{
					nsc.Instance->OnSensorEnter2D(colliderA);
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

					nsc.Instance->OnCollisionEnter2D(c);
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


		if (entityA->HasComponent<NativeScriptComponent>())
		{
			auto& nsc = entityA->GetComponent<NativeScriptComponent>();
			if (nsc.Instance != nullptr)
			{
				if (contact->GetFixtureA()->IsSensor())
				{
					nsc.Instance->OnSensorExit2D(colliderB);
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

					nsc.Instance->OnCollisionExit2D(c);
				}

			}

		}

		if (entityB->HasComponent<NativeScriptComponent>())
		{
			auto& nsc = entityB->GetComponent<NativeScriptComponent>();
			if (nsc.Instance != nullptr)
			{
				if (contact->GetFixtureB()->IsSensor())
				{
					nsc.Instance->OnSensorExit2D(colliderA);
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

					nsc.Instance->OnCollisionExit2D(c);
				}

			}
		}
	}
}