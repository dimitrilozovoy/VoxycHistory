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

#ifndef MAIN_H
#define MAIN_H

#include "../engine/Globals.hpp"
#include "../engine/Audio.h"
#include "../engine/Engine2.h"
#include "../app/VoxycApp.h"

#ifdef PLATFORM_GVR
#include "OvrApp.h"
#include "GuiSys.h"
#include "OVR_Locale.h"
#endif
#ifdef PLATFORM_OPENVR
#include "..\shared\Matrices.h"
#endif

#ifdef PLATFORM_OPENVR
#include <openvr.h>
#endif

void StartApp(int step);
void LoadApp(int step);
void AppTick();
bool Draw(int eye);
void AppFree();
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
void processJoystickInput();
bool getJoyBtn(Controls2 *controls, const unsigned char *buttons, int inBtn, int outBtn, int count);
void processJoyAxis(Controls2 *controls, const float *axes, int inAxis, int outAxis, int count);
void processCharInput(char c);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
#endif
void updateControls();

#ifdef PLATFORM_OPENVR
void SetProjectionMatrix(Matrix4 projection);
void SetPoseEyeProjectionMatrix(Matrix4 poseEyeProjectionMatrix);
void SetPoseEyeMatrix(Matrix4 poseEyeMatrix);
void SetPoseMatrix(Matrix4 m);
void SetIVRSystem(vr::IVRSystem *ivrSystem);
void ProcessControllerStateAndPose(int which, vr::VRControllerState_t state, vr::TrackedDevicePose_t trackedDevicePose);
#endif
#ifdef PLATFORM_GVR
void SetEyeMatrix(OVR::Matrix4f eyeMatrix);
void SetProjectionMatrix(OVR::Matrix4f projectionMatrix);
void SetHeadVector(float yaw, float pitch, float roll);
#endif

extern VoxycApp *g_engineApp;

#endif
