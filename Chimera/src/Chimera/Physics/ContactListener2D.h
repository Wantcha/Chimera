#pragma once

#include "box2d/box2d.h"
#include "box2d/b2_contact.h"
#include "Chimera/Physics/Collider2D.h"

namespace Chimera
{
	class Entity;

	class ContactListener2D : public b2ContactListener
	{
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
	};

	struct Contact2D
	{
		Entity* Entity;
		Collider2D* Collider;
		Collider2D* OtherCollider;

		float ContactCount;
		glm::vec3 ContactPositions[2];
		glm::vec2 Normal;
	};
}