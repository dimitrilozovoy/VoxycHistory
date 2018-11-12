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
	if (moveSmoothly)
		nextPosition += delta;
	else
		position += delta;
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
	delta = velocity;
	move();
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

void Object::rotateYawTowards(float targetYaw, float step)
{
	yaw = limit360(yaw);
	targetYaw = limit360(targetYaw);

	if (targetYaw == 0)
	{
		if (yaw > 180)
			yaw += step;
		else
			yaw -= step;
	}
	else
	{
		float cWiseDist = 0;
		float ccWiseDist = 0;

		if (targetYaw > yaw)
		{
			cWiseDist = targetYaw - yaw;
			ccWiseDist = yaw + (360 - targetYaw);
		}

		if (targetYaw < yaw)
		{
			cWiseDist = (360 - targetYaw) + targetYaw;
			ccWiseDist = yaw - targetYaw;
		}

		if (cWiseDist > ccWiseDist)
			yaw -= step;
		else
			yaw += step;

		yaw = limit360(yaw);
	}
}

float Object::limit360(float value)
{
	while (value > 360)
		value -= 360;
	while (value < 0)
		value += 360;

	return value;
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
	yaw = getKVFloat("roll");
	pitch = getKVFloat("pitch");
	roll = getKVFloat("roll");
	moveSmoothly = getKVBool("moveSmoothly");
	alwaysFacePlayer = getKVFloat("alwaysFacePlayer");
	hitPoints = getKVInt("hitPoints");
	fireTriggerOn = getKVBool("fireTriggerOn");
	speed = getKVFloat("speed");
}