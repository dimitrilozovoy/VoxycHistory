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

#ifndef Camera_h
#define Camera_h

#include "Globals.hpp"
#include "Object.h"

typedef enum CameraSchemes
{
	CAMERA_EDITOR,
	CAMERA_FIRSTPERSON,
	CAMERA_THIRDPERSON_FREE,
	CAMERA_THIRDPERSON_FIXED
} CameraSchemes;

class Camera: public Object
{
public:
	void init(Object *mouseLook);
	void tick();
	void setCameraScheme(CameraSchemes scheme);
	void setPlayerObj(Object *playerObj);
	void mouse(float mouseX, float mouseY);

private:
#ifdef USE_PLAYEROBJ
    Object *playerObj;
#else
	Player *player;
#endif
	Object *mouseLook;
	CameraSchemes cameraScheme;
};

#endif
