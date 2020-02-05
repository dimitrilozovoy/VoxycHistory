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

#ifndef Editor_h
#define Editor_h

#include <stdio.h>
#include "../engine/Engine2.h"
#include "../engine/LuaBridge.h"

typedef enum
{
	EM_OBJ,
	EM_VOX
}
EditorMode;

class Editor
{
public:
    void init();
	void load();
	void tick();
	void tickMovement();
    void tickGuides();
	bool isVoxels(std::string objName);
	void ray(Object *src, float dist, float &x, float &y, float &z);
    void setVoxPreviewTextures();
	void hideSystemObjects();
	void showSystemObjects();
	void enterScreenShotMode();
	void exitScreenShotMode();
	void processHWButtons();
	int btn2key(int btn);
	void processExtraKeys();
	void worldToVoxelCoords(float wx, float wy, float wz, int &x, int &y, int &z);
	bool verifySourceDir(std::string filename);

private:
	const float snapFactor = 0.2;

    Engine2 *engine;
	LuaBridge luaBridge;
	const float defaultObjPreviewDist = 15.0;
	float objPreviewDist = defaultObjPreviewDist;
	const float selectAngleThresh = 5.0;
	const float defaultRayLength = 20.0;
	const float minRayLength = 4.0;
	const float maxRayLength = 50.0;
	const float rayDeltaMultiplier = 0.1;
	float moveSpeed = 0.1;
	float turnSpeed = 1.0;
	float rayLength = 20.0;
	EditorMode mode = EM_OBJ;
	Object *selectedObj = nullptr;
	Object *lastSelectedObj = nullptr;
	Object *lastLitUpGuide = nullptr;
	bool placingObj = false;
	bool movingSelected = false;
	float distToSelected = 0.0;
	Object *curVoxels = nullptr;
	int newVoxelResolution = 16;
	bool selectOnly = true;
	bool gotvox = false;
    int curVoxel = 1;
	bool screenshotMode = false;
	bool objCopy = false;
	bool linkCopy = false;
	bool touchControls = true;
	int msgTimer = 0;
	int msg2Timer = 0;
	bool showGuides = true;
	float lightr = 1.0;
	float lightg = 1.0;
	float lightb = 1.0;
	bool lightVoxels = false;
	bool mapLocation = false;
	bool modified = false;
};

#endif
