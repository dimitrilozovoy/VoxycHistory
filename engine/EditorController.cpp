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

#include "DDLUtils.hpp"
#include "EditorController.h"

void EditorController::setEditVoxelsObj(Object *obj)
{
	voxelsObj = obj;
}

void EditorController::setPlayerObj(Object *obj)
{
	playerObj = obj;
}

void EditorController::tap(float ndcx, float ndcy)
{
	if (voxelsObj == nullptr)
	{
//		Log("voxelsObj is null");
		return;
	}
	
	Shape *shape = voxelsObj->shape;
	
	if (shape == nullptr)
		return;
	
	Voxels *voxels = shape->voxels;
	
	if (voxels == nullptr)
		return;
	
	glm::vec3 lastEmpty;
	glm::vec3 firstFull;
	
	getTilesByVector(&lastEmpty, &firstFull);
	
	voxels->set(lastEmpty.x, lastEmpty.y, lastEmpty.z, curTexture);
	shape->needsRebuild = true;
}

void EditorController::tap2(float ndcx, float ndcy)
{
	if (voxelsObj == nullptr)
	{
//		Log("voxelsObj is null");
		return;
	}

	Shape *shape = voxelsObj->shape;
	
	if (shape == nullptr)
		return;
	
	Voxels *voxels = shape->voxels;
	
	if (voxels == nullptr)
		return;

	glm::vec3 lastEmpty;
	glm::vec3 firstFull;

	getTilesByVector(&lastEmpty, &firstFull);

	voxels->set(firstFull.x, firstFull.y, firstFull.z, 0);
	shape->needsRebuild = true;
}

glm::vec3 EditorController::getLastEmptyVoxel()
{
	glm::vec3 result;
	
	if (voxelsObj == nullptr)
	{
//		Log("voxelsObj is null");
		return glm::vec3(0.0, 0.0, 0.0);
	}

	Shape *shape = voxelsObj->shape;
	
	if (shape == nullptr)
		return glm::vec3(0.0, 0.0, 0.0);
	
	Voxels *voxels = shape->voxels;
	
	if (voxels == nullptr)
		return glm::vec3(0.0, 0.0, 0.0);
		
	glm::vec3 lastEmpty;
	glm::vec3 firstFull;

	getTilesByVector(&lastEmpty, &firstFull);

	result.x = lastEmpty.x;
	result.y = lastEmpty.y;
	result.z = lastEmpty.z;
	
	return result;
}

glm::vec3 EditorController::getFirstFullVoxel()
{
	glm::vec3 result;
	
	if (voxelsObj == nullptr)
	{
//		Log("voxelsObj is null");
		return glm::vec3(0.0, 0.0, 0.0);
	}

	Shape *shape = voxelsObj->shape;
	
	if (shape == nullptr)
		return glm::vec3(0.0, 0.0, 0.0);
	
	Voxels *voxels = shape->voxels;

	if (voxels == nullptr)
		return glm::vec3(0.0, 0.0, 0.0);
		
	glm::vec3 lastEmpty;
	glm::vec3 firstFull;

	getTilesByVector(&lastEmpty, &firstFull);

	result.x = firstFull.x;
	result.y = firstFull.y;
	result.z = firstFull.z;
	
	return result;
}

void EditorController::getTilesByVector(glm::vec3 *lastEmpty, glm::vec3 *firstFull)
{
	if (voxelsObj == nullptr)
	{
//		Log("voxelsObj is null");
		return;
	}

	Shape *shape = voxelsObj->shape;
		
	if (shape == nullptr)
		return;

	Voxels *voxels = shape->voxels;
	
	if (voxels == nullptr)
		return;
    
    float voxelSize = voxelsObj->scale.x / voxels->getSize();
	
	Object vector;
	vector.position.x = (voxelsObj->scale.x / 2) + playerObj->position.x;
	vector.position.y = (voxelsObj->scale.y / 2) + playerObj->position.y;
	vector.position.z = (voxelsObj->scale.z / 2) + playerObj->position.z;
//	std::string str = "vector xyz " + ToString(vector.position.x) + " " + ToString(vector.position.y) + " " + ToString(vector.position.z);
//	Log(str);
	vector.yaw = playerObj->yaw;
	vector.pitch = playerObj->pitch;
	
	Object marker;
	marker.position.x = vector.position.x; // +voxelSize / 2;
	marker.position.y = vector.position.y; //+voxelSize / 2;
	marker.position.z = vector.position.z; //+voxelSize / 2;
    marker.yaw = vector.yaw;
    marker.pitch = vector.pitch;
    
    bool done = false;
    
    // COORDINATE LIMITER
    // if mBox == null, start and end indexes of
    // the below loops should be restricted by camera
    // position and angle
    int startZ = 0;
    int endZ = voxels->getSize();
    int startY = 0;
    int endY = voxels->getSize();
    int startX = 0;  
    int endX = voxels->getSize();

    int roughCameraX = (int)(voxels->getSize() * (vector.position.x / voxelsObj->scale.x));
    int roughCameraY = (int)(voxels->getSize() * (vector.position.y / voxelsObj->scale.y));
    int roughCameraZ = (int)(voxels->getSize() * (vector.position.z / voxelsObj->scale.z));

//	str = "roughcamera xyz " + ToString(roughCameraX) + " " + ToString(roughCameraY) + " " + ToString(roughCameraZ);
//	Log(str);

    startX = roughCameraX - DEFAULT_BUILD_DISTANCE;
    endX = roughCameraX + DEFAULT_BUILD_DISTANCE;
    startY = roughCameraY - DEFAULT_BUILD_DISTANCE;
    endY = roughCameraY + DEFAULT_BUILD_DISTANCE;
    startZ = roughCameraZ - DEFAULT_BUILD_DISTANCE;
    endZ = roughCameraZ + DEFAULT_BUILD_DISTANCE;
    
    if (startZ < 0) {
        startZ = 0;
    }
    if (startY < 0) {
        startY = 0;
    }
    if (startX < 0) {
        startX = 0;
    }
    
    if (endZ > voxels->getSize()) {
        endZ = voxels->getSize();
    }
    if (endY > voxels->getSize()) {
        endY = voxels->getSize();
    }
    if (endX > voxels->getSize()) {
        endX = voxels->getSize();
    }
    
    // Go at an angle through the tiles until we hit the max or visible tile box
    while (marker.position.x >= (float)startX * voxelSize && marker.position.x <= voxelSize * endX
           && marker.position.y >= (float)startY * voxelSize && marker.position.y <= voxelSize * endY
           && marker.position.z >= (float)startZ * voxelSize && marker.position.z <= voxelSize * endZ && !done)
	{
        moveByYawAndPitchAtVel(&marker, 1);
        
        int tile = GetTileAtXYZ(voxels, marker.position.x, marker.position.y, marker.position.z);
        
        if (tile == 0)
		{
            lastEmpty->x = (float) (marker.position.x / voxelsObj->scale.x * voxels->getSize());
            lastEmpty->y = (float) (marker.position.y / voxelsObj->scale.y * voxels->getSize());
            lastEmpty->z = (float) (marker.position.z / voxelsObj->scale.z * voxels->getSize());
        }
        if (tile != 0)
		{
            firstFull->x = (float) (marker.position.x / voxelsObj->scale.x * voxels->getSize());
            firstFull->y = (float) (marker.position.y / voxelsObj->scale.y * voxels->getSize());
            firstFull->z = (float) (marker.position.z / voxelsObj->scale.z * voxels->getSize());
            
            done = true;
        }
    }
    
    if (lastEmpty->x >= voxels->getSize()) {
        lastEmpty->x = voxels->getSize() - 1;
    }
    if (lastEmpty->y >= voxels->getSize()) {
        lastEmpty->y = voxels->getSize() - 1;
    }
    if (lastEmpty->y >= voxels->getSize()) {
        lastEmpty->y = voxels->getSize() - 1;
    }
	
            
	lastEmpty->x = (int)lastEmpty->x;
    lastEmpty->y = (int)lastEmpty->y;
    lastEmpty->z = (int)lastEmpty->z;
    firstFull->x = (int)firstFull->x;
    firstFull->y = (int)firstFull->y;
    firstFull->z = (int)firstFull->z;
}

int EditorController::GetTileAtXYZ(Voxels *tm, float x, float y, float z)
{
    int tile = 0;
    
    int tileX = (int) tm->getSize() * (x / voxelsObj->scale.x);
    int tileY = (int) tm->getSize() * (y / voxelsObj->scale.y);
    int tileZ = (int) tm->getSize() * (z / voxelsObj->scale.z);
    
    tile = tm->get(tileX, tileY, tileZ);
    
    return tile;
}

void EditorController::moveByYawAndPitchAtVel(Object *obj, double vel)
{
    obj->setDeltaXYZ(obj->yaw, obj->pitch, vel);
    obj->move();
}

void EditorController::save(std::string fname)
{
	if (voxelsObj == nullptr)
	{
//		Log("voxelsObj is null");
		return;
	}

	Shape *shape = voxelsObj->shape;
	Voxels *voxels = shape->voxels;

	voxels->save(fname, nullptr);
}

void EditorController::load(std::string fname)
{
/*	if (voxelsObj == nullptr)
	{
//		Log("voxelsObj is null");
		return;
	}

	Shape *shape = voxelsObj->shape;
	Voxels *voxels = shape->voxels;

	voxels->load(fname, nullptr);
	shape->needsRebuild = true;*/
}

void EditorController::minus()
{
	if (textureSwitchTimer > 0)
	{
		return;
	}

	if (curTexture > 1)
		curTexture--;

	textureSwitchTimer = textureSwitchTimerDelay;

	Log("curTexture", curTexture);
}

void EditorController::plus()
{
	if (textureSwitchTimer > 0)
	{
		return;
	}

	if (curTexture < 255)
		curTexture++;

	textureSwitchTimer = textureSwitchTimerDelay;

	Log("curTexture", curTexture);
}

void EditorController::tick()
{
	if (textureSwitchTimer > 0)
	{
		textureSwitchTimer--;
	}
}
