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

#ifndef ModelEditor_h
#define ModelEditor_h
#include <stdio.h>
#include <string>
#include "../engine/Engine2.h"
#include "../engine/LuaBridge.h"
#include "../engine/Globals.hpp"
#include "Editor.h"
#include "../engine/Engine2.h"
#include "../engine/DDLUtils.hpp"
#include "../engine/platform.h"
#include "../engine/Voxels.h"
#include "../engine/Mesh.h"

typedef enum ModelMode
{
	MODE_VERTICES
} ModelMode;

typedef enum ModelView
{
	VIEW_XY,
	VIEW_ZY,
	VIEW_XZ
} ModelView;

class ModelEditor
{
public:
    const int maxVertices = 256;
	const float dragThresh = 0.1;
	const int newVertTimerDelay = 30;

    void init();
	void load();
	void tick();
	void loadMesh(std::string filename);
    void saveMesh(std::string filename);
	void clear();
	void create();
	void setDefaultTextures();
	void refresh();
	void touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2, int actionIndex);
    float scrToGlX(float screenX);
    float scrToGlY(float screenY);
    float glToScrX(float glX);
    float glToScrY(float glY);
    void exportPNG(std::string filename);
	
private:
    Engine2 *engine;
	Mesh mesh;
	
	int btnTimer = 0;
	std::string fileSelectorAction = "";
	int level = 0;
	int texture = 1;
	int screenWidth = 1;
	int screenHeight = 1;
	bool needsRefresh = false;
	ModelMode mode = MODE_VERTICES;
	ModelView view = VIEW_XY;
	float colorr = 1.0;
	float colorg = 1.0;
	float colorb = 1.0;
	float colora = 1.0;
	bool modified = false;
	
	bool dragVert = false;
	int dragVertIdx = 0;
	int newVertTimer = 0;
};

#endif
