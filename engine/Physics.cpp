/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Physics.h"

void Physics::init()
{
}

void Physics::tick(std::map<std::string, Object*> &objects)
{
	collisions.clear();

	for (const auto &pair: objects)
	{
		Object *obj1 = pair.second;

		for (const auto &pair: objects)
		{
			Object *obj2 = pair.second;

			if (obj1 == nullptr || obj2 == nullptr)
				continue;

			bool collx = false;
			bool colly = false;
			bool collz = false;
			bool coll = false;

			if ((obj1->position.x < obj2->position.x) && (obj1->position.x + (obj1->scale.x / 2) > obj2->position.x - (obj2->scale.x / 2))
				|| (obj1->position.x > obj2->position.x) && (obj1->position.x - (obj1->scale.x / 2) < obj2->position.x + (obj2->scale.x / 2)))
				collx = true;

			if ((obj1->position.y < obj2->position.y) && (obj1->position.y + (obj1->scale.y / 2) > obj2->position.y - (obj2->scale.y / 2))
				|| (obj1->position.y > obj2->position.y) && (obj1->position.y - (obj1->scale.y / 2) < obj2->position.y + (obj2->scale.y / 2)))
				colly = true;

			if ((obj1->position.z < obj2->position.z) && (obj1->position.z + (obj1->scale.y / 2) > obj2->position.z - (obj2->scale.z / 2))
				|| (obj1->position.z > obj2->position.z) && (obj1->position.z - (obj1->scale.z / 2) < obj2->position.z + (obj2->scale.z / 2)))
				collz = true;

			if (collx && colly && collz)
				coll = true;

			if (coll)
			{
				// Collision detected
				collisions[obj1] = obj2;
			}
		}
	}

}

bool Physics::checkCollision(Object *obj1, Object *obj2)
{
	if (collisions[obj1] == obj2 || collisions[obj2] == obj1)
		return true;
	else
		return false;
}