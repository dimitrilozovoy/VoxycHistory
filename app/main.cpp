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

#include "../engine/Globals.hpp"
#include "main.h"

#ifdef PLATFORM_OPENVR
#include <GL/glew.h>
#include <GL/glu.h>
#include <thread>
#endif
#ifdef PLATFORM_WINDOWS
#include "../thirdparty/GL/glew.h"
#include "../thirdparty/GLFW/glfw3.h"
#endif
#ifdef PLATFORM_OSX
#include <thread>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif
#include "../engine/DDLUtils.hpp"
#include "../engine/platform.h"
#include "../engine/Globals.hpp"
#include "../engine/DDLUtils.hpp"
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
#include <iostream>
#include <thread>
#endif
#ifdef PLATFORM_GVR
#include <iostream>
#include <thread>
#include "OvrApp.h"
#include "GuiSys.h"
#include "OVR_Locale.h"
#endif
#ifdef PLATFORM_ANDROID
#include <iostream>
#include <thread>
#ifdef USE_JAVAGL
#include "../platform/android/Voxyc/app/src/main/cpp/javagl.h"
#else
#include "EGL/egl.h"
#include "GLES3/gl3.h"
#include "android.h"
#include "android/log.h"
#endif
#include "../platform/android/Voxyc/app/src/main/cpp/android.h"
#include "../engine/AndroidAudio.h"
#include <jni.h>
#endif

#include <stdio.h>
#include <signal.h>
#if not defined PLATFORM_WINDOWS && not defined PLATFORM_OPENVR
#include <unwind.h>
#include <dlfcn.h>
#endif
#include <sstream>

#ifdef PLATFORM_ANDROID
#include <android/log.h>
#endif

#include "VoxycApp.h"
#include "../engine/Audio.h"

#include "../thirdparty/clara/clara.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#ifndef PLATFORM_ANDROID
#include <conio.h>
#endif

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
GLFWwindow* g_glfwWindow = NULL;
void CheckConsoleInput(char c);
#endif

VoxycApp *g_engineApp;

#ifdef PLATFORM_GVR
int g_stereoMatrixHandle = 0;
#endif

#ifdef PLATFORM_ANDROID
#endif

#ifdef SOFTWARE_RENDER
SoftRasterizer g_softRasterizer;
#endif

#ifdef PLATFORM_OSX
#define KEYTIMER_VALUE  10
#else
#define KEYTIMER_VALUE  100
#endif

void OnGLFWError(int errorCode, const char* msg) {
	Log("GLFW error");
	Log((char *)msg, errorCode);
}

// =======================================================================
//
//   Desktop main
//
// =======================================================================

#ifndef PLATFORM_OPENVR

int main(int argc, const char * argv[]) {

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS

	//
	// Read command line params
	//

	std::string module = "";
	std::string assets = "";

	auto cli = clara::Opt(module, "module")
		["-m"]["--module"]
		| clara::Opt(assets, "assets")
		["-a"]["--assets"];

	auto result = cli.parse(clara::Args(argc, argv));
	if (!result) {
		std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
		exit(1);
	}

	if (module != "")
		g_module = module;
	if (assets != "")
	{
		// Make sure assets dir is valid
		struct stat info;

		if (stat(assets.c_str(), &info) != 0)
		{
			printf("Cannot access asset directory %s\n", assets.c_str());
			std::cin.ignore();
			return 0;
		}
		else if (info.st_mode & S_IFDIR)
		{
		}
		else
		{
			printf("Assets directory %s is not directory\n", assets.c_str());
			std::cin.ignore();
			return 0;
		}

		// Set assets dir
		g_assetsDir = assets;
	}
	else
		g_assetsDir = DEFAULT_ASSETS_DIR;

	//
	// Init graphics
	//

	// initialise GLFW
	glfwSetErrorCallback(OnGLFWError);
	if (!glfwInit())
		Log("glfwInit failed");

	// open a window with GLFW
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	g_glfwWindow = glfwCreateWindow(g_windowWidth, g_windowHeight, "Voxyc", NULL, NULL);
	if (!g_glfwWindow)
		Log("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");

	// GLFW settings
	glfwSetInputMode(g_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(g_glfwWindow, 0, 0);
	//    glfwSetScrollCallback(g_glfwWindow, OnScroll);
	glfwMakeContextCurrent(g_glfwWindow);
	glfwSetMouseButtonCallback(g_glfwWindow, mouse_button_callback);

	// Initialize GLEW
	glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
	if (glewInit() != GLEW_OK)
		Log("glewInit failed");

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	//	checkError("glGetString");
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	//	checkError("glGetString");
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	//	checkError("glGetString");
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	//	checkError("glGetString");

	// Make sure OpenGL version 3.2 API is available
	if (!GLEW_VERSION_3_2)
		Log("OpenGL 3.2 API is not available.");

	//
	// Start app
	//

	StartApp(0);
	LoadApp(0);

	//
	// Event loop
	//

	while (!glfwWindowShouldClose(g_glfwWindow))
	{
		updateControls();

		AppTick();
		Draw(0);

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
		glfwSwapBuffers(g_glfwWindow);
#endif

		glfwPollEvents();

	}

#endif

	return 0;
}

#endif

// =======================================================================
//
//   StartApp
//
// =======================================================================

void StartApp(int step)
{
    g_engineApp = new VoxycApp();
    g_engineApp->init();
	return;
}

// =======================================================================
//
//   LoadApp
//
// =======================================================================

void LoadApp(int step)
{
	g_engineApp->load();
	return;
}

// =======================================================================
//
//   AppTick
//
// =======================================================================

void AppTick()
{
//    processJoystickInput();

	g_engineApp->tick();
	return;
}

// =======================================================================
//
//   Draw
//
// =======================================================================

bool Draw(int eye)
{
	g_engineApp->draw(eye);
	return true;
}

// =======================================================================
//
//   AppFree
//
// =======================================================================

void AppFree()
{
	g_engineApp->free();
}

// =======================================================================
//
//   Controls
//
// =======================================================================

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
void processJoystickInput()
{
    int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
    
//    Log("present", present);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Controls2 *controls = g_engine2->getControls();

	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			controls->BtnDown(0);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			controls->BtnDown(1);
	}
	if (action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			controls->BtnUp(0);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			controls->BtnUp(1);
	}
}
#endif

void updateControls()
{
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS

	Controls2 *controls = g_engine2->getControls();

	if (g_keyTimer <= 0)
	{
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_RIGHT_SHIFT) || glfwGetKey(g_glfwWindow, GLFW_KEY_LEFT_SHIFT))
		{
		}
		if (glfwGetKey(g_glfwWindow, 'S') || glfwGetKey(g_glfwWindow, ';') || glfwGetKey(g_glfwWindow, GLFW_KEY_DOWN))
		{
			controls->ArrowDown();
			g_lastKey = 0;
		}
		else if (glfwGetKey(g_glfwWindow, 'W') || glfwGetKey(g_glfwWindow, 'P') || glfwGetKey(g_glfwWindow, GLFW_KEY_UP))
		{
			controls->ArrowUp();
			g_lastKey = 0;
		}
		if (glfwGetKey(g_glfwWindow, 'A') || glfwGetKey(g_glfwWindow, 'L') || glfwGetKey(g_glfwWindow, GLFW_KEY_LEFT))
		{
			controls->ArrowLeft();
			g_lastKey = 0;
		}
		else if (glfwGetKey(g_glfwWindow, 'D') || glfwGetKey(g_glfwWindow, '\'') || glfwGetKey(g_glfwWindow, GLFW_KEY_RIGHT))
		{
			controls->ArrowRight();
			g_lastKey = 0;
		}
		if (glfwGetKey(g_glfwWindow, 'Q'))
		{
			g_lastKey = 0;
		}
		else if (glfwGetKey(g_glfwWindow, 'E'))
		{
			g_lastKey = 0;
		}
		if (glfwGetKey(g_glfwWindow, 'Z') || glfwGetKey(g_glfwWindow, GLFW_KEY_PAGE_DOWN))
		{
			controls->MoveDown();
			g_lastKey = 0;
		}
		else if (glfwGetKey(g_glfwWindow, 'X') || glfwGetKey(g_glfwWindow, GLFW_KEY_PAGE_UP))
		{
			controls->MoveUp();
			g_lastKey = 0;
		}
		else if (glfwGetKey(g_glfwWindow, '`') && g_lastKey != '`')
		{
			g_lastKey = '`';
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_SPACE))
		{
			g_lastKey = 0;
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_MINUS))
		{
			controls->minus();
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_EQUAL))
		{
			controls->equals();
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_KP_SUBTRACT))
		{
			controls->minus();
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_KP_ADD))
		{
			controls->plus();
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_F2))
		{
			controls->save();
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_F3))
		{
			controls->load();
		}

		// Console input enabled; entering console commands
	/*	CheckConsoleInput('Q');
		CheckConsoleInput('W');
		CheckConsoleInput('E');
		CheckConsoleInput('R');
		CheckConsoleInput('T');
		CheckConsoleInput('Y');
		CheckConsoleInput('U');
		CheckConsoleInput('I');
		CheckConsoleInput('O');
		CheckConsoleInput('P');
		//        CheckConsoleInput('[');
		//        CheckConsoleInput(']');
		CheckConsoleInput('A');
		CheckConsoleInput('S');
		CheckConsoleInput('D');
		CheckConsoleInput('F');
		CheckConsoleInput('G');
		CheckConsoleInput('H');
		CheckConsoleInput('J');
		CheckConsoleInput('K');
		CheckConsoleInput('L');
		CheckConsoleInput(';');
		CheckConsoleInput('"');
		CheckConsoleInput('Z');
		CheckConsoleInput('X');
		CheckConsoleInput('C');
		CheckConsoleInput('V');
		CheckConsoleInput('B');
		CheckConsoleInput('N');
		CheckConsoleInput('M');
		//        CheckConsoleInput(',');
		//        CheckConsoleInput('.');
		//        CheckConsoleInput('/');
		CheckConsoleInput('0');
		CheckConsoleInput('1');
		CheckConsoleInput('2');
		CheckConsoleInput('3');
		CheckConsoleInput('4');
		CheckConsoleInput('5');
		CheckConsoleInput('6');
		CheckConsoleInput('7');
		CheckConsoleInput('8');
		CheckConsoleInput('9');
		CheckConsoleInput(' ');
		}

		if (glfwGetKey(g_glfwWindow, '`') && g_lastKey != '`')
		{
			g_lastKey = '`';
			g_console.CancelInput();
		}
		else if (!glfwGetKey(g_glfwWindow, '`') && g_lastKey == '`')
		{
			g_lastKey = 0;
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_ENTER) && g_lastKey != GLFW_KEY_ENTER)
		{
			g_lastKey = GLFW_KEY_ENTER;
			g_console.ProcessInput();
		}
		else if (glfwGetKey(g_glfwWindow, GLFW_KEY_BACKSPACE) && g_lastKey != GLFW_KEY_BACKSPACE)
		{
			if (g_keyTimer <= 0)
			{
				g_lastKey = GLFW_KEY_BACKSPACE;
				g_console.Backspace();
				g_keyTimer = KEYTIMER_VALUE * 2;
			}
			else
				g_keyTimer--;
			}
		}
		else
		{
			g_keyTimer--;
		}*/
	}

	// Rotate camera
	const float mouseSensitivity = 0.1f;
	double mouseX, mouseY;

	glfwGetCursorPos(g_glfwWindow, &mouseX, &mouseY);

	controls->MoveYaw((float)mouseX / 4.0);
	controls->MovePitch((float)-mouseY / 4.0);

	glfwSetCursorPos(g_glfwWindow, 0, 0);
#endif
}

#ifdef PLATFORM_OPENVR
void SetProjectionMatrix(Matrix4 projection)
{
	g_projectionMatrix = projection;
}

void SetPoseEyeProjectionMatrix(Matrix4 poseEyeProjectionMatrix)
{ 
	g_poseEyeProjectionMatrix = poseEyeProjectionMatrix;
}

void SetPoseEyeMatrix(Matrix4 poseEyeMatrix)
{
	g_poseEyeMatrix = poseEyeMatrix;
}

void SetPoseMatrix(Matrix4 m)
{
	g_poseMatrix = m;
}

void SetIVRSystem(vr::IVRSystem *ivrSystem)
{
	g_ivrSystem = ivrSystem;
}

inline glm::mat4 toGlm(const vr::HmdMatrix34_t& m) {
	glm::mat4 result = glm::mat4(
		m.m[0][0], m.m[1][0], m.m[2][0], 0.0,
		m.m[0][1], m.m[1][1], m.m[2][1], 0.0,
		m.m[0][2], m.m[1][2], m.m[2][2], 0.0,
		m.m[0][3], m.m[1][3], m.m[2][3], 1.0f);
	return result;
}

void ProcessControllerStateAndPose(int which, vr::VRControllerState_t state, vr::TrackedDevicePose_t trackedDevicePose)
{
	bool bits[64];
	bool foundTrue = false;
	int trueIdx = -1;
	bool touchpadPressed = false;
	bool leftController = false;

	Controls2 *controls = g_engine2->getControls();

	vr::TrackedPropertyError error;
	int32_t controllerRole;
	vr::TrackedDeviceIndex_t index = which;

	if (g_ivrSystem == nullptr)
		return;

	controllerRole = g_ivrSystem->GetControllerRoleForTrackedDeviceIndex(index);

	if (controllerRole == vr::TrackedControllerRole_LeftHand)
		leftController = true;
	else
		leftController = false;

	// Check ulButtonPressed, buttons

	static bool moving = false;
	static bool firing = false;
	static bool reverse = false;

//	controls->setFireTriggerDown(false);

	for (int i = 0; i < 64; i++)
	{
		bits[i] = CHECK_BIT(state.ulButtonPressed, i);

		if (i == 31)
		{
			if (bits[i] == true)
			{
				if (leftController)
					moving = true;
				else
					firing = true;
			}

			if (bits[i] == false)
			{
				if (leftController)
					moving = false;
				else
					firing = false;
			}
		}

		if (i == 34)
		{
			if (bits[i] == true)
				reverse = true;
			else if (bits[i] == false)
				reverse = false;
		}
	}

	if (trueIdx != -1)
	{
		Log("ulButtonPressed found true at", trueIdx);

		if (trueIdx == 63)
			touchpadPressed = true;
	}

	// Controller pose

	if (which > 0 && trackedDevicePose.bPoseIsValid)
	{
		if (leftController && moving)
		{
			glm::mat4 pose = toGlm(trackedDevicePose.mDeviceToAbsoluteTracking);
			float pitch, yaw, roll = 0.0;
			yaw = atan2(-pose[2][0], pose[0][0]);
		
			if (!reverse)
				controls->moveForwardToYaw(ToDegrees(-yaw) + 180, 1.0);
			else 
				controls->moveForwardToYaw(ToDegrees(-yaw), 1.0);
		}

		if (!leftController && firing)
		{
			glm::mat4 pose = toGlm(trackedDevicePose.mDeviceToAbsoluteTracking);
			float pitch, yaw, roll = 0.0;
			yaw = atan2(-pose[2][0], pose[0][0]);
			controls->setFireYaw(ToDegrees(-yaw) + 180);
			controls->setFireTriggerDown(true);
		}
		else if (leftController && !firing)
		{
			controls->setFireTriggerDown(false);
		}
	}

	// Check ulButtonTouched, touchpad

	for (int i = 0; i < 64; i++)
	{
		bits[i] = CHECK_BIT(state.ulButtonTouched, i);

		if (bits[i] == true)
		{
			bool foundTrue;
			trueIdx = i;
			break;
		}
	}

	if (trueIdx != -1 && trueIdx != 64 && trueIdx != 63 && trueIdx != 31)
	{
//		Log("ulButtonTouched found true at", trueIdx);

//		if (trueIdx == 2)
//			controls->MoveUp(2.0);
	}

	// Process touchpad

	if (touchpadPressed)
	{
		float x = state.rAxis[0].x;
		float y = state.rAxis[0].y;

//		controls->MoveXY(x, y);
	}
}
#endif
#ifdef PLATFORM_GVR
void SetEyeMatrix(OVR::Matrix4f eyeMatrix)
{
	g_eyeMatrix = eyeMatrix;
	g_skybox.m_eyeMatrix = eyeMatrix;
	g_frameAnimRenderer.m_eyeMatrix = eyeMatrix;
}

void SetProjectionMatrix(OVR::Matrix4f projectionMatrix)
{
	g_projectionMatrix = projectionMatrix;
	g_skybox.m_projectionMatrix = projectionMatrix;
	g_frameAnimRenderer.m_projectionMatrix = projectionMatrix;
}

void SetHeadVector(float yaw, float pitch, float roll)
{
	g_headYaw = yaw;
	g_headPitch = pitch;
	g_headRoll = roll;
}
#endif

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS
/*void CheckConsoleInput(char c)
{
	if (glfwGetKey(g_glfwWindow, c))
	{
//		g_console.Enter(c);
		g_lastKey = c;
		g_keyTimer = KEYTIMER_VALUE;
	}
	else if (!glfwGetKey(g_glfwWindow, c) && g_lastKey == c)
	{
		g_lastKey = 0;
	}
}*/
#endif
