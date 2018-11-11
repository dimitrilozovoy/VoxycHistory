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

#ifndef EditorController_h
#define EditorController_h

#include "Voxels.h"
#include "Object.h"

#define DEFAULT_BUILD_DISTANCE 14

class EditorController
{
public:
	void init();
	void setEditVoxelsObj(Object *obj);
	void setPlayerObj(Object *obj);
	void tap(float ndcx, float ndcy);
	void tap2(float ndcx, float ndcy);
	glm::vec3 getLastEmptyVoxel();
	glm::vec3 getFirstFullVoxel();
	void getTilesByVector(glm::vec3 *lastEmpty, glm::vec3 *firstFull);
	int GetTileAtXYZ(Voxels *tm, float x, float y, float z);
	void moveByYawAndPitchAtVel(Object *obj, double vel);
	void save(std::string fname);
	void load(std::string fname);
	void minus();
	void plus();
	void tick();
	int getCurTexture() { return curTexture; }

private:
    Object *playerObj;
    Object *voxelsObj;
	int curTexture = 1;
	int textureSwitchTimer = 0;
	const int textureSwitchTimerDelay = 20;
};

#endif
