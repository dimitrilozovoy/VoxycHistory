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

#include "Globals.hpp"
#include "DDLUtils.hpp"
#include "Object.h"
#include "../thirdparty/glm/gtx/scalar_multiplication.hpp"

#ifdef PLATFORM_ANDROID
#include <cmath>
#include <cstdlib>
#endif

void Object::setDeltaXZ(float heading, float vel)
{
    delta.x = 0;
    delta.z = 0;

    if (vel < 0)
    {
        vel = -vel;
        heading -= 180.0;
    }

    while (heading < 0.0)
        heading = heading + 360.0;
    while (heading > 360.0)
        heading = heading - 360.0;

    if (heading == 0.0)
    {
        delta.x = 0;
        delta.z = vel;
    }
    else if (heading == 180.0)
    {
        delta.x = 0;
        delta.z = -vel;
    }
    else if (heading == 90.0)
    {
        delta.x = vel;
        delta.z = 0;
    }
    else if (heading == 270.0)
    {
        delta.x = -vel;
        delta.z = 0;
    }
    else if (heading >= 0.0 && heading < 90.0) {
        // x = opposite
        // y = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = std::abs(vel * sin(glm::radians(heading)));
        delta.z = std::abs(vel * cos(glm::radians(heading)));

    } else if (heading >= 90.0 && heading < 180.0) {
        // y = opposite
        // x = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = std::abs(vel * cos(glm::radians(heading - 90.0)));
        delta.z = -std::abs(vel * sin(glm::radians(heading - 90.0)));

    } else if (heading >= 180.0 && heading < 270.0) {
        // x = opposite
        // y = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = -std::abs(vel * sin(glm::radians(heading - 180.0)));
        delta.z = -std::abs(vel * cos(glm::radians(heading - 180.0)));

    } else if (heading >= 270.0 && heading < 360.0) {
        // y = opposite
        // x = adjacent
        // sin(head) = x / hyp
        // cos(head) = y / hyp

        delta.x = -std::abs(vel * cos(glm::radians(heading - 270.0)));
        delta.z = std::abs(vel * sin(glm::radians(heading - 270.0)));
    }
}

void Object::setDeltaXYZ(float heading, float pitch, float vel)
{
	delta.x = 0;
	delta.y = 0;
	delta.z = 0;

	if (vel < 0)
	{
		vel = -vel;
		heading -= 180.0;
		pitch -= 180.0;
	}

	// Heading

	while (heading < 0.0)
		heading = heading + 360.0;
	while (heading > 360.0)
		heading = heading - 360.0;

/*	heading += 90;
	heading = -heading;

	delta.z = cos(glm::radians(heading)) * cos(glm::radians(pitch)) * vel;
	delta.x = sin(glm::radians(heading)) * cos(glm::radians(pitch)) * vel;
	delta.y = sin(glm::radians(pitch)) * vel;*/
		
	if (heading == 0.0)
	{
		delta.x = 0;
		delta.z = vel;
	}
	else if (heading == 180.0)
	{
		delta.x = 0;
		delta.z = -vel;
	}
	else if (heading == 90.0)
	{
		delta.x = vel;
		delta.z = 0;
	}
	else if (heading == 270.0)
	{
		delta.x = -vel;
		delta.z = 0;
	}
	else if (heading >= 0.0 && heading < 90.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = std::abs(vel * sin(glm::radians(heading)));
		delta.z = -std::abs(vel * cos(glm::radians(heading)));

	}
	else if (heading >= 90.0 && heading < 180.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = std::abs(vel * cos(glm::radians(heading - 90.0)));
		delta.z = std::abs(vel * sin(glm::radians(heading - 90.0)));

	}
	else if (heading >= 180.0 && heading < 270.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = -std::abs(vel * sin(glm::radians(heading - 180.0)));
		delta.z = std::abs(vel * cos(glm::radians(heading - 180.0)));

	}
	else if (heading >= 270.0 && heading < 360.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.x = -std::abs(vel * cos(glm::radians(heading - 270.0)));
		delta.z = -std::abs(vel * sin(glm::radians(heading - 270.0)));
	}

	// Pitch

	while (pitch < 0.0)
		pitch = pitch + 360.0;
	while (pitch> 360.0)
		pitch = pitch - 360.0;

	if (pitch == 0.0)
	{
		delta.y = 0;
	}
	else if (pitch == 180.0)
	{
		delta.y = 0;
	}
	else if (pitch == 90.0)
	{
		delta.y = vel;
	}
	else if (pitch == 270.0)
	{
		delta.y = -vel;
	}
	else if (pitch >= 0.0 && pitch < 90.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = std::abs(vel * sin(glm::radians(pitch)));

	}
	else if (pitch >= 90.0 && pitch < 180.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = std::abs(vel * cos(glm::radians(pitch - 90.0)));

	}
	else if (pitch >= 180.0 && pitch < 270.0) {
		// x = opposite
		// y = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = -std::abs(vel * sin(glm::radians(pitch - 180.0)));

	}
	else if (pitch >= 270.0 && pitch < 360.0) {
		// y = opposite
		// x = adjacent
		// sin(head) = x / hyp
		// cos(head) = y / hyp

		delta.y = -std::abs(vel * cos(glm::radians(pitch - 270.0)));
	}
}

void Object::move()
{
	// Move smoothly if needed
	if (moveSmoothly)
	{
		if (abs(delta.x) > 0.1 || abs(delta.z) > 0.1)
		{
			if (speedFactor < 1.0)
			{
				speedFactor += speedFactorDelta;
				delta = delta * speedFactor;
			}

		}
	}

	// Check for possible collisions before moving
	bool okToMove = true;
	std::string stuckOn = "";

	if (ints["ignorecollisions"] != 1)
	{
		float subtickDeltaX = delta.x / (float)g_common.physicsSubticks;
		float subtickDeltaY = delta.y / (float)g_common.physicsSubticks;
		float subtickDeltaZ = delta.z / (float)g_common.physicsSubticks;
		
//		Log("g_common.physicsSubticks", g_common.physicsSubticks);
		
		int s = 0;
		
		for (s = 0; s < g_common.physicsSubticks; s++)
		{
		    // Pretend we moved
//		    position += delta;
		
		    position.x += subtickDeltaX;
		    position.y += subtickDeltaY;
		    position.z += subtickDeltaZ;

		    // Check for other objects
		    for (auto other : nearCollisions)
		    {
				if (okToMove == true && strings["ignorecollisionswith"] == other->name)
					okToMove = true;
				else
				{
					if (
						(other->category == "model"
							|| other->category == "block"
							|| other->category == "voxels"
							|| other->category == "sprite"
							|| other->name == "player")
						&& other->visible
						&& checkCollision(other, 1.0))
					{
						okToMove = false;
						stuckOn = other->name;
					}
				}
		    }

		    // Check if we are escaping play area
		    if (g_common.playAreaMinX != 0 && position.x < g_common.playAreaMinX
			|| g_common.playAreaMaxX != 0 && position.x > g_common.playAreaMaxX
			|| g_common.playAreaMinY != 0 && position.y < g_common.playAreaMinY
			|| g_common.playAreaMaxY != 0 && position.y > g_common.playAreaMaxY
			|| g_common.playAreaMinZ != 0 && position.z < g_common.playAreaMinZ
			|| g_common.playAreaMaxZ != 0 && position.z > g_common.playAreaMaxZ)
		    {
			    okToMove = false;
			    stuckOn = "playarea";
		    }
			
			if (!okToMove)
				break;
		}

		// Unmove
//   	position -= delta;
		position.x -= subtickDeltaX * (float)s;
		position.y -= subtickDeltaY * (float)s;
		position.z -= subtickDeltaZ * (float)s;
	}

	// Move if OK
	if (okToMove)
	{	
		ints["stuck"] = 0;
		strings["stuckon"] = "";
		
		position += delta;

		deltaLastTick.x += delta.x;
		deltaLastTick.y += delta.y;
		deltaLastTick.z += delta.z;
		deltaLastTick.w += delta.w;

		if (moveSmoothly)
		{
			tickTotalDelta.x += delta.x;
			tickTotalDelta.y += delta.y;
			tickTotalDelta.z += delta.z;
			tickTotalDelta.w += delta.w;
		}
	}
	else
	{
		position -= delta;
		
		// If player, slide them down the walls if possible instead of stucking them
		if (name == "player")
		{
			bool slide = false;
				
			int numColls = 0;
				
			if (collNorth && !collEast && !collWest)
			{
				if (delta.z < 0)
					delta.z = 0;
					
				slide = true;
				numColls++;
			}
			
			if (collSouth && !collEast && !collWest)
			{
				if (delta.z > 0)
					delta.z = 0;
					
				slide = true;
				numColls++;
			}

			if (collEast && !collSouth && !collNorth)
			{
				if (delta.x > 0)
					delta.x = 0;
					
				slide = true;
				numColls++;
			}
			
			if (collWest && !collSouth && !collNorth)
			{
				if (delta.x < 0)
					delta.x = 0;
					
				slide = true;
				numColls++;
			}
			
			if (slide && numColls == 1)
			{
				position += delta;

				return;
			}
		}

		ints["stuck"] = 1;
		strings["stuckon"] = stuckOn;
	}
	
//	if (ints["stuck"] == 1 && category != "voxels")
//		Log(name + " stuck");
}

void Object::tickStart()
{
	if (moveSmoothly)
	{
		if (decel)
		{
			delta = decelDelta;
			speedFactor -= speedFactorDeltaDecel;
			delta = delta * speedFactor;

			moveSmoothly = false;
			move();
			moveSmoothly = true;

			if (speedFactor < 0.1)
				decel = false;
		}

		lastTickTotalDelta = tickTotalDelta;

		tickTotalDelta.x = 0.0f;
		tickTotalDelta.y = 0.0f;
		tickTotalDelta.z = 0.0f;
		tickTotalDelta.w = 0.0f;
	}
}

void Object::tickStartPostControls()
{
	float thresh = 0.05;

	if (abs(tickTotalDelta.x) < thresh && abs(tickTotalDelta.z) < thresh
		&& ((abs(lastTickTotalDelta.x) > thresh || abs(lastTickTotalDelta.z) > thresh)))
	{
		// We're not trying to move but were moving on the last tick
		delta = lastTickTotalDelta;
		moveSmoothly = false;
		move();
		moveSmoothly = true;
		decel = true;
		decelDelta = lastTickTotalDelta;
	}

	if (decel)
	{
		if (abs(tickTotalDelta.x) > thresh || abs(tickTotalDelta.z) > thresh)
		{
			decel = false;
		}
	}
}

void Object::tickEnd()
{
/*	if (moveSmoothly)
	{
		if (abs(tickTotalDelta.x) < 0.01 && abs(tickTotalDelta.z) < 0.01)
		{
			if (abs(lastTickTotalDelta.x) > 0.01 || abs(lastTickTotalDelta.z) > 0.01)
			{
//				speedFactor = 0;
			}
		}
	}*/
}

void Object::MoveBackward(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-yaw, distance);
	move();
}

void Object::MoveForward(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-yaw, -distance);
	move();
}

void Object::MoveLeft(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-(yaw + 90), distance);
	move();
}

void Object::MoveRight(float factor)
{
	float distance = moveDistance * factor;

	delta.y = 0;
	setDeltaXZ(-(yaw - 90), distance);

	move();
}

void Object::MoveUp(float factor)
{
	float distance = moveDistance * factor;

	delta.x = 0.0;
	delta.y = distance;
	delta.z = 0.0;
	move();
}

void Object::MoveDown(float factor)
{
	float distance = moveDistance * factor;

	delta.x = 0.0;
	delta.y = -distance;
	delta.z = 0.0;
	move();
}

void Object::MoveYaw(float amount)
{
	float oldYaw = yaw;

	yaw += amount;

	if (yaw == 0.0 && oldYaw < 0.0)
		yaw = 0.1;

	if (yaw == 0.0 && oldYaw > 0.0)
		yaw = -0.1;
}

void Object::MovePitch(float amount)
{
//	if ((amount < 0 && pitch > -90) || (amount > 0 && pitch < 90))
		pitch += amount;
}

void Object::MoveRoll(float amount)
{
//	if ((amount < 0 && pitch > -90) || (amount > 0 && pitch < 90))
		roll += amount;
}

float Object::distanceTo(Object *other)
{
	if (other == nullptr)
		return 0;

	int deltaX = other->position.x - position.x;
	int deltaY = other->position.y - position.y;
	int deltaZ = other->position.z - position.z;

	return sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

float Object::getYawTo(Object *dest)
{
	float dx = dest->position.x - position.x;
	float dy = dest->position.y - position.y;
	float dz = dest->position.z - position.z;
	
	float yaw = atan2(dz, dx);

	return ToDegrees(yaw) + 90.0;
	
/*	float dx = dest->position.x - position.x;
	float dy = dest->position.y - position.y;
	float dz = dest->position.z - position.z;
	
	float yaw = atan2(dz, dx);

	return ToDegrees(yaw) ;*/
	
/*
	float result = 0;
	
	float deltax = position.x - dest->position.x;
	float deltaz = position.z - dest->position.z;

	result = ToDegrees(atan2(deltax, deltaz));*/

/*	float tox = dest->position.x;
	float toz = dest->position.z;

	double result = 0;

	double deltaX = 0;
	double deltaZ = 0;

	deltaX = std::abs(std::abs(tox) - std::abs(position.x));
	deltaZ = std::abs(std::abs(toz) - std::abs(position.z));

	double heading90 = 0;

	// Going north-east
	if (tox > position.x && toz < position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaX / deltaZ)));
		result = heading90;
	}
	// Going south-east
	if (tox > position.x && toz > position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaZ / deltaX)));
		result = 90 + heading90;
	}

	// Going south-west
	if (tox < position.x && toz > position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaX / deltaZ)));
		result = 180 + heading90;
	}

	// Going north-west
	if (tox < position.x && toz < position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaZ / deltaX)));
		result = 270 + heading90;
	}*/
	
//	return result;
}

float Object::getPitchTo(Object *dest)
{
	float dx = dest->position.x - position.x;
	float dy = dest->position.y - position.y;
	float dz = dest->position.z - position.z;

    float pitchTo = atan2(sqrt(dz * dz + dx * dx), dy) + 3.14159265;
	
	return -(ToDegrees(pitchTo) + 90.0);
	
/*	float dx = dest->position.x - position.x;
	float dy = dest->position.y - position.y;
	float dz = dest->position.z - position.z;

    pitch = atan2(sqrt(dz * dz + dx * dx), dy) + 3.14159265;
	
	return ToDegrees(pitch);*/
	
/*	float result = 0;

	float deltax = position.x - dest->position.x;
	float deltaz = position.z - dest->position.z;

	result = ToDegrees(atan2(deltax, deltaz));*/

/*	float toy = dest->position.y;
	float toz = dest->position.z;

	double result = 0;

	double deltaY = 0;
	double deltaZ = 0;

	deltaY = std::abs(std::abs(toy) - std::abs(position.y));
	deltaZ = std::abs(std::abs(toz) - std::abs(position.z));

	double heading90 = 0;

	if (toy > position.y && toz < position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaY / deltaZ)));
		result = heading90;
	}

	if (toy > position.y && toz > position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaZ / deltaY)));
		result = 90 + heading90;
	}

	if (toy < position.y && toz > position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaY / deltaZ)));
		result = 180 + heading90;
	}

	if (toy < position.y && toz < position.z) {
		heading90 = std::abs(ToDegrees(atan(deltaZ / deltaY)));
		result = 270 + heading90;
	}
	
	return result;*/
}

void Object::moveByVelocity()
{
	if (velocity != glm::vec4(0.0, 0.0, 0.0, 0.0))
	{
	    delta = velocity;
	    move();
	}
}

void Object::capVelocity()
{
	if (velocity.x >= maxVelocity)
		velocity.x = maxVelocity;
	if (velocity.y >= maxVelocity)
		velocity.y = maxVelocity;
	if (velocity.z >= maxVelocity)
		velocity.z = maxVelocity;

	if (velocity.x <= -maxVelocity)
		velocity.x = -maxVelocity;
	if (velocity.y <= -maxVelocity)
		velocity.y = -maxVelocity;
	if (velocity.z <= -maxVelocity)
		velocity.z = -maxVelocity;
}

void Object::moveTowardsNextPosition()
{
	glm::vec4 delta = nextPosition - position;

	position += delta / glm::vec4(2.0, 2.0, 2.0, 1.0);
}

void Object::moveTowardsNextWaypoint()
{
	if (nextWaypoint < waypoints.size())
	{
	    Waypoint next = waypoints[nextWaypoint];
		
		if (!wayptTickDeltaReady)
		{
			float diffx = next.pos.x - position.x;
		    float diffy = next.pos.y - position.y;
            float diffz = next.pos.z - position.z;

			wayptTickDeltaX = diffx / (float)ticksPerWaypt;
			wayptTickDeltaY = diffy / (float)ticksPerWaypt;
			wayptTickDeltaZ = diffz / (float)ticksPerWaypt;
			
			wayptTickDeltaReady = true;
			waypointTick = 0;
		}
		
		position.x += wayptTickDeltaX;
		position.y += wayptTickDeltaY;
		position.z += wayptTickDeltaZ;
				
		// Check if we've arrived at next waypoint
		float diffx = next.pos.x - position.x;
	    float diffy = next.pos.y - position.y;
        float diffz = next.pos.z - position.z;

		if (waypointTick >= ticksPerWaypt)
	    {
			nextWaypoint++;
			wayptTickDeltaReady = false;
			waypointTick = 0;
		}
		
		waypointTick++;
	}
	else
	{
//		moveByWaypoints = false;
	}
}

std::string Object::toString()
{
	char str[4096];
	
	std::string shapeName = "";
	if (shape != nullptr)
		shapeName = shape->name;
		
    std::string modelName = "";
	    if (model != nullptr)
		modelName = model->name;
	
	snprintf(str, 4096, "name=%s type=%d shapeType=%d shape=%s model=%s system=%d visible=%d",
	    name.c_str(), type, shapeType, shapeName.c_str(), modelName.c_str(), system, visible);
	
	/*
	    ObjType type;
    ObjShapeType shapeType;
    Shape *shape = nullptr;
	Model2 *model = nullptr;
	Mesh *batchedMesh = nullptr;
	bool mainOfBatch = false;
	bool partOfBatch = false;
	glm::vec4 color;
    std::string textureName;
    float size;
	glm::vec3 scale;
    glm::vec4 position;
	glm::vec4 nextPosition;
	glm::vec4 delta;
	glm::vec4 velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
	float maxVelocity = 4.0;
    float yaw = 0.1;
	float pitch = 0;
	float roll = 0;
    int glTexID = -1;
	const float moveDistance = 1.0;
	bool moveSmoothly = false;
	float fadeNearDistance = 600;
	float fadeFarDistance = 900;
	bool alwaysFacePlayer = false;
	bool isPartOfVehicle = false;
	bool mapTexture = false;
	bool system = false;

	*/
	
	return str;
}

void Object::save(FILE *f)
{
	setFile(f);

	write6Char('O', 'B', 'J', 'E', 'C', 'T');

	clearKVSet();

	addKV("name", name);
	addKV("type", (int)type);
	addKV("shapeType", (int)shapeType);
    if (shape != nullptr)
    	addKV("shapeName", shape->name);
    if (model != nullptr)
    	addKV("modelName", model->name);

	addKV("color.x", color.x);
	addKV("color.y", color.y);
	addKV("color.z", color.z);
	addKV("color.w", color.w);

	addKV("textureName", textureName);
	addKV("size", size);

	addKV("scale.x", scale.x);
	addKV("scale.y", scale.y);
	addKV("scale.z", scale.z);

	addKV("position.x", position.x);
	addKV("position.y", position.y);
	addKV("position.z", position.z);
	addKV("position.w", position.w);

/*	addKV("nextPosition.x", nextPosition.x);
	addKV("nextPosition.y", nextPosition.y);
	addKV("nextPosition.z", nextPosition.z);
	addKV("nextPosition.w", nextPosition.w);*/

	addKV("delta.x", delta.x);
	addKV("delta.y", delta.y);
	addKV("delta.z", delta.z);
	addKV("delta.w", delta.w);

	addKV("velocity.x", velocity.x);
	addKV("velocity.y", velocity.y);
	addKV("velocity.z", velocity.z);
	addKV("velocity.w", velocity.w);

	addKV("maxVelocity", maxVelocity);
	addKV("yaw", yaw);
	addKV("pitch", pitch);
	addKV("roll", roll);
	addKV("moveSmoothly", moveSmoothly);
	addKV("alwaysFacePlayer", alwaysFacePlayer);
	addKV("hitPoints", hitPoints);
	addKV("fireTriggerOn", fireTriggerOn);
	addKV("speed", speed);

	writeKVSet();
}

void Object::load(FILE *f)
{
	setFile(f);

	if (!read6CharEquals('O', 'B', 'J', 'E', 'C', 'T'))
		return;

	readKVSet();

	name = getKVStr("name");
	type = (ObjType)getKVInt("type");
	shapeType = (ObjShapeType)getKVInt("shapeType");
	shapeName = getKVStr("shapeName");
	modelName = getKVStr("modelName");

	color.x = getKVFloat("color.x");
	color.y = getKVFloat("color.y");
	color.z = getKVFloat("color.z");
	color.w = getKVFloat("color.w");

	textureName = getKVStr("textureName");
	size = getKVFloat("size");

	scale.x = getKVFloat("scale.x");
	scale.y = getKVFloat("scale.y");
	scale.z = getKVFloat("scale.z");

	physSize = scale;

	position.x = getKVFloat("position.x");
	position.y = getKVFloat("position.y");
	position.z = getKVFloat("position.z");
	position.w = getKVFloat("position.w");

	nextPosition.x = position.x;
	nextPosition.y = position.y;
	nextPosition.z = position.z;
	nextPosition.w = position.w;
	
/*	nextPosition.x = getKVFloat("nextPosition.x");
	nextPosition.y = getKVFloat("nextPosition.y");
	nextPosition.z = getKVFloat("nextPosition.z");
	nextPosition.w = getKVFloat("nextPosition.w");*/

	delta.x = getKVFloat("delta.x");
	delta.y = getKVFloat("delta.y");
	delta.z = getKVFloat("delta.z");
	delta.w = getKVFloat("delta.w");

	velocity.x = getKVFloat("velocity.x");
	velocity.y = getKVFloat("velocity.y");
	velocity.z = getKVFloat("velocity.z");
	velocity.w = getKVFloat("velocity.w");

	maxVelocity = getKVFloat("maxVelocity");
	yaw = getKVFloat("yaw");
	pitch = getKVFloat("pitch");
	roll = getKVFloat("roll");
	moveSmoothly = getKVBool("moveSmoothly");
	alwaysFacePlayer = getKVFloat("alwaysFacePlayer");
	hitPoints = getKVInt("hitPoints");
	fireTriggerOn = getKVBool("fireTriggerOn");
	speed = getKVFloat("speed");
}

bool Object::checkCollision(Object *obj2, float multiplier)
{
//	if ((name == "player" && obj2->name == "weapon") || (name == "weapon" && obj2->name == "player"))
//		return false;

	if (obj2 == nullptr || obj2->ints["ignorecollisions"] == 1)
		return false;

//	if (obj2 == nullptr || !visible || !obj2->visible || obj2->ints["ignorecollisions"] == 1)
//		return false;

	if (obj2->shape != nullptr && obj2->shape->voxels != nullptr)
	{
		return checkVoxelCollision(obj2, multiplier);
	}

	bool collx = false;
	bool colly = false;
	bool collz = false;
	bool coll = false;

	Object *obj1 = this;

	if ((obj1->position.x < obj2->position.x) && (obj1->position.x + ((obj1->physSize.x * multiplier) / 2) > obj2->position.x - ((obj2->physSize.x * multiplier) / 2))
		|| (obj1->position.x > obj2->position.x) && (obj1->position.x - ((obj1->physSize.x * multiplier) / 2) < obj2->position.x + ((obj2->physSize.x * multiplier) / 2)))
		collx = true;

	if ((obj1->position.y < obj2->position.y) && (obj1->position.y + ((obj1->physSize.y * multiplier) / 2) > obj2->position.y - ((obj2->physSize.y * multiplier) / 2))
		|| (obj1->position.y > obj2->position.y) && (obj1->position.y - ((obj1->physSize.y * multiplier) / 2) < obj2->position.y + ((obj2->physSize.y * multiplier) / 2)))
		colly = true;

	if ((obj1->position.z < obj2->position.z) && (obj1->position.z + ((obj1->physSize.z * multiplier) / 2) > obj2->position.z - ((obj2->physSize.z * multiplier) / 2))
		|| (obj1->position.z > obj2->position.z) && (obj1->position.z - ((obj1->physSize.z * multiplier) / 2) < obj2->position.z + ((obj2->physSize.z * multiplier) / 2)))
		collz = true;

	if (collx && colly && collz)
		return true;
	else
		return false;
}

bool Object::checkVoxelCollision(Object *voxObj, float multiplier)
{
	if (voxObj == nullptr)
		return false;

	Shape *shape = voxObj->shape;

	if (shape == nullptr)
		return false;

	Voxels *voxels = shape->voxels;

	if (voxels == nullptr)
		return false;

	bool result = false;
	int x, y, z = 0;

	float h = (physSize.x / 2) * multiplier;
	int steps = 10;
	float stepx = physSize.x / (float)steps;
	float stepy = physSize.y / (float)steps;
	float stepz = physSize.z / (float)steps;

	float wy = position.y - h / 1.5;

	lastCollNorth = collNorth;
	lastCollEast = collEast;
	lastCollSouth = collSouth;
	lastCollWest = collWest;

	collNorth = false;
	collEast = false;
	collSouth = false;
	collWest = false;
	
	int slack = 2;

	// A
	for (int s = slack; s < steps - slack; s++)
	{
		worldToVoxelCoords(voxObj, position.x - h + stepx * s, wy, position.z - h, x, y, z);
		if (voxels->get(x, y, z) != 0)
		{
			result = true;
			collNorth = true;
		}
	}

	// B
	for (int s = slack; s < steps - slack; s++)
	{
		worldToVoxelCoords(voxObj, position.x + h, wy, position.z - h + stepz * s, x, y, z);
		if (voxels->get(x, y, z) != 0)
		{
			result = true;
			collEast = true;
		}
	}
	
	// C
	for (int s = slack; s < steps - slack; s++)
	{
		worldToVoxelCoords(voxObj, position.x - h + stepx * s, wy, position.z + h, x, y, z);
		if (voxels->get(x, y, z) != 0)
		{
			result = true;
			collSouth = true;
		}
	}

	// D
	for (int s = slack; s < steps - slack; s++)
	{
		worldToVoxelCoords(voxObj, position.x - h, wy, position.z - h + stepz * s, x, y, z);
		if (voxels->get(x, y, z) != 0)
		{
			result = true;
			collWest = true;
		}
	}
	
//	if (result)
//		Log("checkvoxelcoll true");

	return result;
}

bool Object::checkVoxelCollisionCircular(Object *voxObj, float multiplier)
{
	if (voxObj == nullptr)
		return false;

	Shape *shape = voxObj->shape;

	if (shape == nullptr)
		return false;

	Voxels *voxels = shape->voxels;

	if (voxels == nullptr)
		return false;

	bool result = false;
	float wx, wy, wz = 0;
	int x, y, z = 0;

	float rad = scale.x * multiplier / 2.0;
	int sectors = 12;

	float slideToX = position.x;
	float slideToZ = position.z;

	for (int s = 0; s < sectors; s++)
	{
		float hityaw = (360.0f / (float)sectors) * s;

		wx = position.x + cos(ToRadians(hityaw)) * rad;
		wz = position.z + sin(ToRadians(hityaw)) * rad;

		worldToVoxelCoords(voxObj, wx, position.y, wz, x, y, z);

		if (voxels->get(x, y, z) != 0)
			result = true;

		if (result)
		{
			// Find where to slide object
			float newhityaw = RotateAngleAwayFrom(hityaw, yaw, 5);

			slideToX = wx + cos(ToRadians(newhityaw + 180)) * rad * 1.2;
			slideToZ = wz + sin(ToRadians(newhityaw + 180)) * rad * 1.2;

			position.x = slideToX;
			position.z = slideToZ;

			return false;
//			continue;
		}
	}

	return result;
}

void Object::worldToVoxelCoords(Object *voxObj, float wx, float wy, float wz, int &x, int &y, int &z)
{
	if (voxObj == nullptr)
		return;

	float rx = (wx - (voxObj->position.x - voxObj->scale.x / 2.0)) /
		((voxObj->position.x + voxObj->scale.x / 2.0) -
		(voxObj->position.x - voxObj->scale.x / 2.0));
	float ry = (wy - (voxObj->position.y - voxObj->scale.y / 2.0)) /
		((voxObj->position.y + voxObj->scale.y / 2.0) -
		(voxObj->position.y - voxObj->scale.y / 2.0));
	float rz = (wz - (voxObj->position.z - voxObj->scale.z / 2.0)) /
		((voxObj->position.z + voxObj->scale.z / 2.0) -
		(voxObj->position.z - voxObj->scale.z / 2.0));

	int size = voxObj->shape->voxels->getSize();

	x = (int)((float)size * rx);
	y = (int)((float)size * ry);
	z = (int)((float)size * rz);
}

void Object::checkWayptModelLoad()
{
	if (wayptModelLoading && wayptModel != nullptr && wayptModel->state == MODEL_LOADED)
	{
//		Log("setting waypts from model");
	    Mesh *mesh = wayptModel->meshes[0];
			
		if (mesh != nullptr)
		{
			for (int v = 0; v < mesh->vertices.size(); v++)
			{
				Waypoint wpt;
		
		        wpt.tick = (v + 1) * ticksPerWaypt;
					
		        wpt.pos.x = position.x + mesh->vertices[v].x * wayptScale + v * ticksPerWaypt * wayptVelX;
		        wpt.pos.y = position.y + mesh->vertices[v].y * wayptScale + v * ticksPerWaypt * wayptVelY;
				wpt.pos.z = position.z + mesh->vertices[v].z * wayptScale + v * ticksPerWaypt * wayptVelZ;
			    waypoints.push_back(wpt);
				
//				Log("v", v);
			}
		}
		
		wayptModelLoading = false;
	}
}

/*
========================================
checkSight
========================================
*/

bool Object::canSee(Object *dst, std::map<std::string, Object*> objects)
{
	Object *src = this;
	Object ray;

	ray.position = src->position;
	ray.pitch = src->pitch;
	ray.yaw = src->getYawTo(dst);
	ray.roll = src->roll;

	int iter = 0;
	bool hit = false;
	int numSteps = 50;
	float step = src->distanceTo(dst) / (float)numSteps;

	while (!hit && iter < numSteps)
	{
		for (const auto &pair2 : objects)
		{
			Object *obj2 = pair2.second;

			if (obj2 == nullptr || obj2->category == "terrain")
				continue;

			if (ray.checkVoxelCollision(obj2, 0.1))
			{
				return false;
			}
		}

		ray.MoveForward(step);
		iter++;
	}

	return true;
}

