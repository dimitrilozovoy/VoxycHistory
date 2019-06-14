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
	nearCollisions.clear();

	// Clear near collisions
	for (const auto &pair : objects)
	{
		Object *obj1 = pair.second;

		if (obj1 == nullptr)
			continue;

		obj1->nearCollisions.clear();

		for (const auto &pair2 : objects)
		{
			Object *obj2 = pair2.second;

			if (obj2 == nullptr)
				continue;

			obj2->nearCollisions.clear();
		}
	}

	// Find collisions and near collisions
	for (const auto &pair : objects)
	{
		Object *obj1 = pair.second;

		if (obj1 == nullptr || obj1->category == "terrain" || obj1->category == "voxels")
			continue;

		for (const auto &pair2: objects)
		{
			Object *obj2 = pair2.second;

			if (obj2 == nullptr || obj2->category == "terrain")
				continue;

			if (obj1 == nullptr || obj2 == nullptr)
				continue;

			// Actual collisions

			if (obj1->checkCollision(obj2, 1.0))
			{
				// Collision detected
				collisions[obj1] = obj2;
			}

			// Near collisions

			bool nearcollx = false;
			bool nearcolly = false;
			bool nearcollz = false;
			bool nearcoll = false;

			if (obj1->checkCollision(obj2, 1.2))
			{
				// Collision detected
				nearCollisions[obj1] = obj2;

				obj1->nearCollisions.push_back(obj2);
				obj2->nearCollisions.push_back(obj1);
			}
		}
	}
}

bool Physics::checkCollision(Object *obj1, Object *obj2)
{
	if (obj1 == nullptr || obj2 == nullptr)
		return false;
	
	if (collisions[obj1] == obj2 || collisions[obj2] == obj1)
		return true;
	else
		return false;
}
