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
#include "../thirdparty/macos/GL/glew.h"
#include "../thirdparty/macos/GLFW/glfw3.h"
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
#ifdef GAME_RELEASE_DIR_TREE
#include "../javagl.h"
#else
#include "../platform/android/Voxyc/app/src/main/cpp/javagl.h"
#endif
#else
#include "EGL/egl.h"
#include "GLES3/gl3.h"
#include "android.h"
#include "android/log.h"
#endif
#ifdef GAME_RELEASE_DIR_TREE
#include "../android.h"
#else
#include "../platform/android/Voxyc/app/src/main/cpp/android.h"
#endif
#include "../engine/PlatformAudio.h"
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
#if defined PLATFORM_WINDOWS
#include <conio.h>
#endif

#include "../engine/ReadData.h"

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

#if !defined PLATFORM_OPENVR && !defined PLATFORM_IOS

int main(int argc, const char * argv[]) {

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS

	//
	// Read command line params
	//

	std::string module = "";
	std::string assets = "";
	std::string datafile = "";
	std::string noguides = "";
	std::string monitor = "";
	std::string fullscreen = "1";
	std::string nomusic = "0";

	auto cli = clara::Opt(module, "module")
		["-m"]["--module"]
		| clara::Opt(assets, "assets")
		["-a"]["--assets"]
		| clara::Opt(datafile, "datafile")
		["-d"]["--datafile"]
		| clara::Opt(noguides, "noguides")
		["-n"]["--noguides"]
		| clara::Opt(monitor, "monitor")
		["-s"]["--monitor"]
		| clara::Opt(fullscreen, "fullscreen")
		["-f"]["--fullscreen"]
		| clara::Opt(nomusic, "nomusic")
		["-m"]["--nomusic"];

	auto result = cli.parse(clara::Args(argc, argv));
	if (!result) {
		std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
		exit(1);
	}

	// Set nomusic
	if (nomusic == "1")
		g_common.noMusic = true;

	// Set startup module
	if (module != "")
		g_module = module;

	// Set assets dir
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
	if (noguides != "")
		g_noGuides = true;

	// Set datafile
	if (datafile != "")
	{
		g_dataFile = datafile;
		g_useDataFile = true;
		ReadDataOpenDAT(datafile);
	}

	//
	// Init graphics
	//

	// initialise GLFW
	glfwSetErrorCallback(OnGLFWError);
	if (!glfwInit())
		Log("glfwInit failed");

	// Get screen resolution
	int numMonitor = PLAT_stoi(monitor, 0);

	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);

	if (numMonitor >= count)
		numMonitor = count - 1;

	const GLFWvidmode* mode = glfwGetVideoMode(monitors[numMonitor]);

	g_common.windowWidth = mode->width;
	g_common.windowHeight = mode->height;

	// open a window with GLFW
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	if (fullscreen != "1")
	{
		const int windowWidth = mode->width * 0.8;
		const int windowHeight = mode->height * 0.8;
		g_common.windowWidth = windowWidth;
		g_common.windowHeight = windowHeight;
		g_glfwWindow = glfwCreateWindow(windowWidth, windowHeight, "Voxyc", 0, NULL);
	}
	else
		g_glfwWindow = glfwCreateWindow(mode->width, mode->height, "Voxyc", monitors[numMonitor], NULL);

	if (!g_glfwWindow)
		Log("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");

	// GLFW settings
	glfwSetInputMode(g_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(g_glfwWindow, 0, 0);
	glfwMakeContextCurrent(g_glfwWindow);
	glfwSetMouseButtonCallback(g_glfwWindow, mouse_button_callback);
	glfwSetScrollCallback(g_glfwWindow, scroll_callback);

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

	while (!glfwWindowShouldClose(g_glfwWindow) && !g_common.exit)
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
//	Log("AppTick");
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

//
// mouse_button_callback()
//

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Controls2 *controls = g_engine2->getControls();

	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			controls->setMouseBtn(0, 1);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			controls->setMouseBtn(1, 1);
	}
	if (action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			controls->setMouseBtn(0, 0);
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			controls->setMouseBtn(1, 0);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Controls2 *controls = g_engine2->getControls();

	controls->setMouseScroll(yoffset);
}
#endif

//
// updateControls()
//

void updateControls()
{
#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS

	Controls2 *controls = g_engine2->getControls();
	Camera *camera = g_engine2->getCamera();
	GUI *gui = g_engine2->getGUI();

	//
	// Gamepad and keyboard
	//

		int curJoystick = GLFW_JOYSTICK_1;

		bool joy = glfwJoystickPresent(curJoystick);
		int count = 0;

		if (joy)
		{
			const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

			processJoyAxis(controls, axes, 0, AX_LEFT_X, count);
			processJoyAxis(controls, axes, 1, AX_LEFT_Y, count);
			processJoyAxis(controls, axes, 2, AX_RIGHT_X, count);
			processJoyAxis(controls, axes, 3, AX_RIGHT_Y, count);
			processJoyAxis(controls, axes, 4, AX_LEFT_TRIGGER, count);
			processJoyAxis(controls, axes, 5, AX_RIGHT_TRIGGER, count);
		}

		const unsigned char *buttons = glfwGetJoystickButtons(curJoystick, &count);

		for (int i = 0; i < count; i++)
		{
//			if (buttons[i] == 1)
//				Log("btn ", i);
		}

		if ((joy && getJoyBtn(controls, buttons, 10, BTN_UP, count)) || (!gui->nonNativeWidgetsShown() && glfwGetKey(g_glfwWindow, 'W')) || glfwGetKey(g_glfwWindow, GLFW_KEY_UP))
			controls->setBtn(BTN_UP, 1);
		else
			controls->setBtn(BTN_UP, 0);

		if ((joy && getJoyBtn(controls, buttons, 12, BTN_DOWN, count)) || (!gui->nonNativeWidgetsShown() && glfwGetKey(g_glfwWindow, 'S')) || glfwGetKey(g_glfwWindow, GLFW_KEY_DOWN))
			controls->setBtn(BTN_DOWN, 1);
		else
			controls->setBtn(BTN_DOWN, 0);

		if ((joy && getJoyBtn(controls, buttons, 13, BTN_LEFT, count)) || (!gui->nonNativeWidgetsShown() && glfwGetKey(g_glfwWindow, 'A')) || glfwGetKey(g_glfwWindow, GLFW_KEY_LEFT))
			controls->setBtn(BTN_LEFT, 1);
		else
			controls->setBtn(BTN_LEFT, 0);

		if ((joy && getJoyBtn(controls, buttons, 11, BTN_RIGHT, count)) || (!gui->nonNativeWidgetsShown() && glfwGetKey(g_glfwWindow, 'D')) || glfwGetKey(g_glfwWindow, GLFW_KEY_RIGHT))
			controls->setBtn(BTN_RIGHT, 1);
		else
			controls->setBtn(BTN_RIGHT, 0);

		if ((joy && getJoyBtn(controls, buttons, 6, BTN_GUIDE, count)))
			controls->setBtn(BTN_GUIDE, 1);
		else
			controls->setBtn(BTN_GUIDE, 0);

		if ((joy && getJoyBtn(controls, buttons, 7, BTN_START, count)))
			controls->setBtn(BTN_START, 1);
		else
			controls->setBtn(BTN_START, 0);

		if ((joy && getJoyBtn(controls, buttons, 4, BTN_LEFT_BUMPER, count)))
			controls->setBtn(BTN_LEFT_BUMPER, 1);
		else
			controls->setBtn(BTN_LEFT_BUMPER, 0);

		if ((joy && getJoyBtn(controls, buttons, 5, BTN_RIGHT_BUMPER, count)))
			controls->setBtn(BTN_RIGHT_BUMPER, 1);
		else
			controls->setBtn(BTN_RIGHT_BUMPER, 0);

		if ((joy && getJoyBtn(controls, buttons, 8, BTN_LEFT_THUMB, count)))
			controls->setBtn(BTN_LEFT_THUMB, 1);
		else
			controls->setBtn(BTN_LEFT_THUMB, 0);

		if ((joy && getJoyBtn(controls, buttons, 9, BTN_RIGHT_THUMB, count)))
			controls->setBtn(BTN_RIGHT_THUMB, 1);
		else
			controls->setBtn(BTN_RIGHT_THUMB, 0);

		if ((joy && getJoyBtn(controls, buttons, 0, BTN_A, count)))
			controls->setBtn(BTN_A, 1);
		else
			controls->setBtn(BTN_A, 0);

		if ((joy && getJoyBtn(controls, buttons, 1, BTN_B, count)))
			controls->setBtn(BTN_B, 1);
		else
			controls->setBtn(BTN_B, 0);

		if ((joy && getJoyBtn(controls, buttons, 2, BTN_X, count)))
			controls->setBtn(BTN_X, 1);
		else
			controls->setBtn(BTN_X, 0);

		if ((joy && getJoyBtn(controls, buttons, 3, BTN_Y, count)))
			controls->setBtn(BTN_Y, 1);
		else
			controls->setBtn(BTN_Y, 0);

		if (glfwGetKey(g_glfwWindow, GLFW_KEY_SPACE))
		{
			g_lastKey = 0;
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_MINUS))
		{
			controls->minus();
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_EQUAL))
		{
			controls->equals();
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_KP_SUBTRACT))
		{
			controls->minus();
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_KP_ADD))
		{
			controls->plus();
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_F2))
		{
			controls->save();
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_F3))
		{
			controls->load();
		}
		if (glfwGetKey(g_glfwWindow, GLFW_KEY_BACKSPACE))
		{
			gui->backspace();
		}

		processCharInput('Q');
		processCharInput('W');
		processCharInput('E');
		processCharInput('R');
		processCharInput('T');
		processCharInput('Y');
		processCharInput('U');
		processCharInput('I');
		processCharInput('O');
		processCharInput('P');
		processCharInput('A');
		processCharInput('S');
		processCharInput('D');
		processCharInput('F');
		processCharInput('G');
		processCharInput('H');
		processCharInput('J');
		processCharInput('K');
		processCharInput('L');
		processCharInput(';');
		processCharInput('"');
		processCharInput('Z');
		processCharInput('X');
		processCharInput('C');
		processCharInput('V');
		processCharInput('B');
		processCharInput('N');
		processCharInput('M');
		processCharInput('.');
		processCharInput('0');
		processCharInput('1');
		processCharInput('2');
		processCharInput('3');
		processCharInput('4');
		processCharInput('5');
		processCharInput('6');
		processCharInput('7');
		processCharInput('8');
		processCharInput('9');
		processCharInput(' ');

		// Give all key presses to the controls
		for (int i = 32; i < GLFW_KEY_LAST; i++)
		{
			int val = glfwGetKey(g_glfwWindow, i);
			controls->setKey(i, val);
		}

	//
	// Mouse
	//

	const float mouseSensitivity = 0.1f;
	double mouseX, mouseY;

	glfwGetCursorPos(g_glfwWindow, &mouseX, &mouseY);

	controls->mouse(mouseX, mouseY);
	camera->mouse(mouseX, mouseY);

	glfwSetCursorPos(g_glfwWindow, 0, 0);
#endif
}

bool getJoyBtn(Controls2 *controls, const unsigned char *buttons, int inBtn, int outBtn, int count)
{
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
	if (count > MAX_BUTTONS)
		return false;

	if (buttons[inBtn] == GLFW_PRESS)
		return true;
	else
		return false;
#endif

	return false;
}

void processJoyAxis(Controls2 *controls, const float *axes, int inAxis, int outAxis, int count)
{
	if (count > MAX_AXES)
		return;

	controls->setAxis(outAxis, axes[inAxis]);
}

#if defined PLATFORM_OSX || defined PLATFORM_WINDOWS

void processCharInput(char c)
{
	GUI *gui = g_engine2->getGUI();
	Controls2 *ctrl = g_engine2->getControls();

	if (glfwGetKey(g_glfwWindow, c))
		gui->charEntered(c);
}

#endif

// =======================================================================
//
//   Virtual Reality
//
// =======================================================================

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
