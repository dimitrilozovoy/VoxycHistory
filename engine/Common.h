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

#ifndef COMMON_H
#define COMMON_H

#include <map>

typedef struct Common
{
	float playAreaMinX = 0;
	float playAreaMaxX = 0;
	float playAreaMinY = 0;
	float playAreaMaxY = 0;
	float playAreaMinZ = 0;
	float playAreaMaxZ = 0;
	bool touchCtrlLJDown = false;
	float touchCtrlLJDistX = 0;
	float touchCtrlLJDistY = 0;
	bool touchCtrlRJDown = false;
	float touchCtrlRJDistX = 0;
	float touchCtrlRJDistY = 0;
	float gamepadLeftX = 0;
	float gamepadLeftY = 0;
	float gamepadRightX = 0;
	float gamepadRightY = 0;
	std::map<std::string, int> extraInts;
    std::map<std::string, float> extraFloats;
	std::map<std::string, std::string> extraStrings;
	char readSCVersionA = '1';
	char readSCVersionB = '0';
	char readSCVersionC = '1';
	bool useTexturesOnSprites = true;
	float playerMoveSpeed = 0.1;
	float playerTurnSpeed = 1.0f / 30.0f;
	float ambientr = 1.0;
	float ambientg = 1.0;
	float ambientb = 1.0;
#ifdef PLATFORM_IOS
	bool doDynamicLights = false;
#else
    bool doDynamicLights = true;
#endif
	bool centerModelsX = true;
	bool centerModelsY = true;
	bool centerModelsZ = true;
	float cameraOffsetZ = 30.0;
	float cameraOffsetY = 5.0;
	float drawDistance = 1000;
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	bool fixedTimestep = true;
#else
	bool fixedTimestep = false;
#endif
	int physicsSubticks = 1;
	bool occlusionCheck = true;
	int graphics = 1;
	int windowWidth = 0;
	int windowHeight = 0;
	bool noMusic = false;
	bool showMouse = true;
	bool mouseLook = false;
    bool exit = false;
    float mouseSens = 0.5;
    float controllerSens = 0.5;
} Common;

typedef struct RenderStats
{
	int fps = 0;
	int drawCalls = 0;
	int polys = 0;
} RenderStats;

typedef struct VertexBuffer
{
	float *vertices = nullptr;
	int size = 0;
	int allocatedSize = 0;
	int cursor = 0;
} VertexBuffer;

#endif
