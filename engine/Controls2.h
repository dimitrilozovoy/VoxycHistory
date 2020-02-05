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

#ifndef FATELESS_CONTROLS2_H
#define FATELESS_CONTROLS2_H

#include "Globals.hpp"
#include "Object.h"
#include "SpriteRenderer2D.hpp"
#include "TextureManager2.h"
#include "EditorController.h"
#include "GUI.h"

#define NUM_KBD_KEYS 1024
#define NUM_AXES 6
#define EASE_IN_TICKS 10

typedef enum BtnNames
{
	BTN_A,
	BTN_B,
	BTN_X,
	BTN_Y,
	BTN_LEFT_BUMPER,
	BTN_RIGHT_BUMPER,
	BTN_BACK,
	BTN_START,
	BTN_GUIDE,
	BTN_LEFT_THUMB,
	BTN_RIGHT_THUMB,
	BTN_UP,
	BTN_RIGHT,
	BTN_DOWN,
	BTN_LEFT,
	MAX_BUTTONS
} BtnNames;

typedef enum ENUM_AXES
{
	AX_LEFT_X,
	AX_LEFT_Y,
	AX_RIGHT_X,
	AX_RIGHT_Y,
	AX_LEFT_TRIGGER,
	AX_RIGHT_TRIGGER,
	MAX_AXES
} ENUM_AXES;

typedef enum ControlSchemes
{
	CTRL_EDITOR,
	CTRL_FPS,
	CTRL_THIRDPERSON,
	CTRL_SWIPEXY,
/*	CTRL_UFOSHOOTER,
	CTRL_UFOSHOOTER_360,
	CTRL_SCROLLINGSHOOTER,
	CTRL_SCROLLINGSHOOTERXY,*/
	CTRL_CUSTOM
} ControlSchemes;

typedef struct TouchBtnBind
{
	float x;
	float y;
	float size;
	int btn;
} TouchBtnBind;

class Controls2
{
public:
    void init(Object *camera, Object *mouseLook, TextureManager2 *texMan, EditorController *editor, GUI *gui);
    void tick();
	void tickMouse();
	void touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2, int actionIndex);
    void processTouchJoystick(bool left, int action, float x, float y);
	void processGUI();
	void resetGUI();
	void ArrowDown(float factor = 1.00);
    void ArrowUp(float factor = 1.00);
    void ArrowLeft(float factor = 1.00);
    void ArrowRight(float factor = 1.00);
	void MoveUp(float factor = 1.00);
	void MoveDown(float factor = 1.00);
	void MoveYaw(float amount);
    void MovePitch(float amount);
	void increaseThrottle();
	void decreaseThrottle();
	void rudderLeft();
	void rudderRight();
	float getMoveFactor();
	void setControlScheme(ControlSchemes scheme);
	void BtnDown(int which);
	void BtnUp(int which);
	void minus();
	void equals();
	void plus();
	void Draw2D(SpriteRenderer2D *r);
	void drawMouse(SpriteRenderer2D* r);
	float scrToGlX(float screenX);
	float scrToGlY(float screenY);
	float glToScrX(float glX);
	float glToScrY(float glY);
    void setTriggerAlwaysOn(bool value);
	void setEnabled(bool enabled);
	void moveForwardToYaw(float yaw, float factor);
	void setFireYaw(float yaw);
	float getFireYaw() { return fireYaw; };
	void setFireTriggerDown(bool down);
	void setPlayerObj(Object *playerObj);
	void setPlayerRangeX(float min, float max);
	void setPlayerRangeY(float min, float max);
	float getMinPlayerX() { return minPlayerX; };
	float getMaxPlayerX() { return maxPlayerX; };
	float getMinPlayerY() { return minPlayerY; };
	float getMaxPlayerY() { return maxPlayerY; };
	void tap(); 
	void tap2();
	void save();
	void load();
	bool checkActionUp();
	void mouse(float mouseX, float mouseY);
	void setMouseScroll(float val) { mouseScroll = val; };
	float getMouseScroll() { return mouseScroll; };
	void setBtn(int which, int state);
	int getBtn(BtnNames which);
	float getRJDistX() { return rjlastmovex - rjlastdownx; };
	float getRJDistY() { return rjlastmovey - rjlastdowny; };
	bool getRJDown() { return rjdown; };
	void addTouchBtnBind(int btn, float x, float y, float size);
	void remTouchBtnBind(int btn);
	std::vector<TouchBtnBind> getTouchBtnBinds() { return touchBtnBinds; };
	void setAxis(int axis, float value);
	float getAxis(int axis);
	void setKey(int key, int val);
	int getKey(int key);
	void setMouseBtn(int which, int state);
	int getMouseBtn(int which);
    float getMouseCursorX();
    float getMouseCursorY();

private:
	const char *defaultVoxelsFname = "C:/Users/dimit/voxels.vx";
	const float mouseMultiplier = 0.04f;
	const int mouseClickTimerDelay = 30;

    Object *camera = nullptr;
    Object *playerObj = nullptr;
	Object *mouseLook = nullptr;
    TextureManager2 *texMan = nullptr;
	EditorController *editorController = nullptr;
	GUI* gui = nullptr;

    int screenWidth = 0;
    int screenHeight = 0;

	// Touchscreen joysticks
	bool jdown = false;
	float jlastdownx = 0;
    float jlastdowny = 0;
    float jlastmovex = 0;
    float jlastmovey = 0;
    bool ljdown = false;
	int ljdownticks = 0;
    float rjlastdownx = 0;
    float rjlastdowny = 0;
    float rjlastmovex = 0;
    float rjlastmovey = 0;
    bool rjdown = false;
	int rjdownticks = 0;
    float ljlastdownx = 0;
    float ljlastdowny = 0;
    float ljlastmovex = 0;
    float ljlastmovey = 0;
	bool lrjwentup = false;

	float minPlayerX = 0.0;
	float maxPlayerX = 0.0;
	float minPlayerY = 0.0;
	float maxPlayerY = 0.0;

	ControlSchemes controlScheme = CTRL_FPS;

    bool triggerAlwaysOn = false;
	bool enabled = true;

	float fireYaw = 0.0;
	
	int afterGestureTimer = 0;
	bool actionUp = false;

	// Buttons
	int buttons[MAX_BUTTONS];
	std::vector<TouchBtnBind> touchBtnBinds;

	// Gamepad
	float axes[MAX_AXES];

	// Mouse
	float mouseX, mouseY = 0.0;
	float mouseScroll = 0.0;
	int mouseButtons[3];
	float mouseCursorX, mouseCursorY = 0.0;
	int mouseClickTimer = 0;
	bool mouseMoved = false;

	// Keyboard keys
	int keys[NUM_KBD_KEYS];
	int keyTimers[NUM_KBD_KEYS];
	const int keyDelay = 10;

	// GUI
	int numPressedA = 0;
	bool aWasPressed = false;
	int numPressedSpace = 0;
	bool spaceWasPressed = false;
	int numPressedEnter = 0;
	bool enterWasPressed = false;
};

#endif //FATELESS_CONTROLS2_H
