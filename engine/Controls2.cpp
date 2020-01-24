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

#include "Globals.hpp"
#include "Controls2.h"
#include "platform.h"
#include <cstdlib>
#include "DDLUtils.hpp"
#include <sstream>
#include "EditorController.h"
#include "GUI.h"

void Controls2::init(Object *camera, Object *mouseLook, TextureManager2 *texMan,
                     EditorController *editorController, GUI *gui) {
    this->camera = camera;
    this->mouseLook = mouseLook;
    this->texMan = texMan;
    this->editorController = editorController;
	this->gui = gui;

    texMan->load("thumbstick.png");

	for (int k = 0; k < NUM_KBD_KEYS; k++)
	{
		keyTimers[k] = 0;
	}
}

void Controls2::tick() {

	screenWidth = PLAT_GetWindowWidth();
	screenHeight = PLAT_GetWindowHeight();

	// Mouse click timer - has to count on fixed tick
	if (mouseClickTimer > 0)
		mouseClickTimer--;

	// GUI
	processGUI();

	// If not enabled, do not do the rest (player controls, etc)
	if (!enabled)
        return;

	//
    // Process buttons
    //

    // New schemes

    float targetYaw = 1000000;

    switch (controlScheme) {
		case CTRL_FPS:

/*			if (keys[GLFW_KEY_W] == 1)
				playerObj->MoveForward(0.4);
			if (keys[GLFW_KEY_S] == 1)
				playerObj->MoveBackward(0.4);
			if (keys[GLFW_KEY_A] == 1)
				playerObj->MoveLeft(0.3);
			if (keys[GLFW_KEY_D] == 1)
				playerObj->MoveRight(0.3);*/
							
			if (buttons[BTN_UP] == 1)
				playerObj->MoveForward(0.4);
			if (buttons[BTN_DOWN] == 1)
				playerObj->MoveBackward(0.4);
			if (buttons[BTN_LEFT] == 1)
				playerObj->MoveLeft(0.3);
			if (buttons[BTN_RIGHT] == 1)
				playerObj->MoveRight(0.3);
			break;

        case CTRL_EDITOR:

/*			if (buttons[BTN_UP] == 1)
			{
				playerObj->MoveForward(getMoveFactor());
			}
			if (buttons[BTN_DOWN] == 1)
			{
				playerObj->MoveForward(-getMoveFactor());
			}

			if (buttons[BTN_LEFT] == 1)
                playerObj->MoveLeft(getMoveFactor());
            if (buttons[BTN_RIGHT] == 1)
                playerObj->MoveRight(getMoveFactor());

            if (buttons[BTN_UP] == 1 && buttons[BTN_LEFT] == 1) {
                playerObj->MoveUp(getMoveFactor());
                playerObj->MoveLeft(getMoveFactor());
            }
            if (buttons[BTN_UP] == 1 && buttons[BTN_RIGHT] == 1) {
                playerObj->MoveUp(getMoveFactor());
                playerObj->MoveRight(getMoveFactor());
            }
            if (buttons[BTN_DOWN] == 1 && buttons[BTN_LEFT] == 1) {
                playerObj->MoveDown(getMoveFactor());
                playerObj->MoveLeft(getMoveFactor());
            }
            if (buttons[BTN_DOWN] == 1 && buttons[BTN_RIGHT] == 1) {
                playerObj->MoveDown(getMoveFactor());
                playerObj->MoveRight(getMoveFactor());
            }*/

            break;
        case CTRL_THIRDPERSON:

            if (buttons[BTN_UP] == 1)
                targetYaw = camera->yaw;
            if (buttons[BTN_DOWN] == 1)
                targetYaw = camera->yaw + 180;
            if (buttons[BTN_LEFT] == 1)
                targetYaw = camera->yaw - 90;
            if (buttons[BTN_RIGHT] == 1)
                targetYaw = camera->yaw + 90;

            if (buttons[BTN_UP] == 1 && buttons[BTN_LEFT] == 1)
                targetYaw = camera->yaw - 45;
            if (buttons[BTN_UP] == 1 && buttons[BTN_RIGHT] == 1)
                targetYaw = camera->yaw + 45;
            if (buttons[BTN_DOWN] == 1 && buttons[BTN_LEFT] == 1)
                targetYaw = camera->yaw - 135;
            if (buttons[BTN_DOWN] == 1 && buttons[BTN_RIGHT] == 1)
                targetYaw = camera->yaw + 135;

            if (targetYaw != 1000000) {
                if (!compareYaw(playerObj->yaw, targetYaw, 10)) {
                    playerObj->yaw = RotateAngleTowards(playerObj->yaw, targetYaw, 5);
                    playerObj->MoveForward(getMoveFactor() * 0.3);
                } else {
                    playerObj->yaw = targetYaw;
                    playerObj->MoveForward(getMoveFactor() * 0.5);
                }
            }

            break;
    }

    // Old schemes

/*    if (buttons[BTN_UP] == 1) {
        switch (controlScheme) {
            case CTRL_SCROLLINGSHOOTER:
                playerObj->MoveForward(getMoveFactor() * 2.0);
                break;
            case CTRL_UFOSHOOTER_360:
                playerObj->MoveForward(getMoveFactor() * 2.0);
                break;
        }
    }

    if (buttons[BTN_DOWN] == 1) {
        switch (controlScheme) {
            case CTRL_SCROLLINGSHOOTER:
                playerObj->MoveBackward(getMoveFactor() * 2.0);
                break;
            case CTRL_UFOSHOOTER_360:
                playerObj->MoveBackward(getMoveFactor() * 2.0);
                break;
        }
    }

    if (buttons[BTN_LEFT] == 1) {
        switch (controlScheme) {
            case CTRL_SCROLLINGSHOOTER:
                playerObj->MoveLeft(getMoveFactor() * 10.0);
                break;
            case CTRL_UFOSHOOTER_360:
                playerObj->MoveLeft(getMoveFactor());
                break;
        }
    }

    if (buttons[BTN_RIGHT] == 1) {
        switch (controlScheme) {
            case CTRL_SCROLLINGSHOOTER:
                playerObj->MoveRight(getMoveFactor() * 10.0);
                break;
            case CTRL_UFOSHOOTER_360:
                playerObj->MoveRight(getMoveFactor());
                break;
        }
    }*/

    //
    // Touch control for new schemes
    //
	
	// Ease in due to extreme data on start
	float easeInMultL = 1.0f;
	float easeInMultR = 2.0f;
	
	if (ljdownticks < EASE_IN_TICKS)
		easeInMultL = (float)ljdownticks / (float)EASE_IN_TICKS;
	if (rjdownticks < EASE_IN_TICKS)
		easeInMultR = (float)rjdownticks / (float)EASE_IN_TICKS;

    switch (controlScheme) {
		
		case CTRL_FPS:
			
			if (ljdown) {
                float xdiff = ljlastmovex - ljlastdownx;
                float ydiff = ljlastmovey - ljlastdowny;
				
				playerObj->MoveBackward((ydiff / 300) * easeInMultL);
				playerObj->MoveRight((xdiff / 300) * easeInMultL);
            }
			
			if (rjdown) {
                float xdiff = rjlastmovex - rjlastdownx;
                float ydiff = rjlastmovey - rjlastdowny;
				
//				playerObj->MoveBackward(ydiff / 50);
				playerObj->yaw += (xdiff * g_common.playerTurnSpeed) * easeInMultR;
            }
			
			break;
		
        case CTRL_THIRDPERSON:

            // Movement

            float targetYaw = 1000000;

            if (ljdown) {
                float xdiff = ljlastmovex - ljlastdownx;
                float ydiff = ljlastmovey - ljlastdowny;

                targetYaw = camera->yaw + ToDegrees(atan2(xdiff, -ydiff));
            }

            if (targetYaw != 1000000) {
                if (!compareYaw(playerObj->yaw, targetYaw, 10)) {
                    playerObj->yaw = RotateAngleTowards(playerObj->yaw, targetYaw, 8);
                    playerObj->MoveForward(getMoveFactor() * 0.3);
                } else {
                    playerObj->yaw = targetYaw;
                    playerObj->MoveForward(getMoveFactor() * 0.5);
                }
            }

            break;
    }

    //
    // Process touch
    //

    if (jdown) {
        float xdiff = jlastmovex - jlastdownx;
        float ydiff = jlastmovey - jlastdowny;

        switch (controlScheme) {
            case CTRL_SWIPEXY:
                if (xdiff > 0.2)
                    playerObj->MoveRight(abs(xdiff) * getMoveFactor());
                if (xdiff < -0.2)
                    playerObj->MoveLeft(abs(xdiff) * getMoveFactor());
                if (ydiff > 0.2)
                    playerObj->MoveDown(abs(ydiff) * getMoveFactor());
                if (ydiff < -0.2)
                    playerObj->MoveUp(abs(ydiff) * getMoveFactor());
                break;
            case CTRL_EDITOR:
                if (ydiff < -0.2)
                    MovePitch(abs(ydiff) / 1000.0);
                if (ydiff > 0.2)
                    MovePitch(-abs(ydiff) / 1000.0);
                if (xdiff > 0.2)
                    MoveYaw(abs(xdiff) / 300.0);
                if (xdiff < -0.2)
                    MoveYaw(-abs(xdiff) / 300.0);
                break;
            default:
/*            if (ydiff < -0.2)
                MovePitch(abs(ydiff) / 280.0);
            if (ydiff > 0.2)
                MovePitch(-abs(ydiff) / 280.0);
            if (xdiff > 0.2)
                MoveYaw(abs(xdiff) / 140.0);
            if (xdiff < -0.2)
                MoveYaw(-abs(xdiff) / 140.0);*/
                break;
        }
    }

    g_common.touchCtrlLJDown = ljdown;

    if (ljdown) {
		ljdownticks += 1;
		
        float xdiff = ljlastmovex - ljlastdownx;
        float ydiff = ljlastmovey - ljlastdowny;

        g_common.touchCtrlLJDistX = xdiff;
        g_common.touchCtrlLJDistY = ydiff;

        switch (controlScheme) {
            case CTRL_EDITOR:
/*            if (ydiff < -0.2)
                playerObj->MoveForward(abs(ydiff) * getMoveFactor());
            if (ydiff > 0.2)
                playerObj->MoveBackward(abs(ydiff) * getMoveFactor());
            if (xdiff > 0.2)
                playerObj->MoveYaw(abs(xdiff) * getMoveFactor() / 2.0);
            if (xdiff < -0.2)
                playerObj->MoveYaw(-abs(xdiff) * getMoveFactor() / 2.0);*/
                break;
            default:
                if (ydiff < -0.2)
                    MovePitch(abs(ydiff) / 280.0);
                if (ydiff > 0.2)
                    MovePitch(-abs(ydiff) / 280.0);
                if (xdiff > 0.2)
                    MoveYaw(abs(xdiff) / 140.0);
                if (xdiff < -0.2)
                    MoveYaw(-abs(xdiff) / 140.0);
                break;
        }
    }

    g_common.touchCtrlRJDown = rjdown;

    if (rjdown) {
		rjdownticks += 1;
		
        float xdiff = rjlastmovex - rjlastdownx;
        float ydiff = rjlastmovey - rjlastdowny;

        g_common.touchCtrlRJDistX = xdiff;
        g_common.touchCtrlRJDistY = ydiff;

//		Logf("rjdown ydiff ", ydiff);

        switch (controlScheme) {
            case CTRL_EDITOR:
/*            if (ydiff < -0.2)
                playerObj->MoveUp(abs(ydiff) * getMoveFactor() / 10.0);
            if (ydiff > 0.2)
                playerObj->MoveDown(abs(ydiff) * getMoveFactor() / 10.0);
            if (xdiff > 0.2)
                playerObj->MoveRight(abs(xdiff) * getMoveFactor());
            if (xdiff < -0.2)
                playerObj->MoveLeft(abs(xdiff) * getMoveFactor());*/
                break;
            default:
/*            if (ydiff < -0.2)
                ArrowUp(abs(ydiff) / 140.0);
            if (ydiff > 0.2)
                ArrowDown(abs(ydiff) / 140.0);
            if (xdiff > 0.2)
                ArrowRight(abs(xdiff) / 140.0);
            if (xdiff < -0.2)
                ArrowLeft(abs(xdiff) / 150.0);*/
                break;
        }
    }

    //
    // Gamepad control for new schemes
    //

	float deadZone = 0.01;

    switch (controlScheme) {
		case CTRL_FPS:
			if (abs(axes[AX_LEFT_X]) > deadZone)
				playerObj->MoveRight(axes[AX_LEFT_X] / 4.0);
			if (abs(axes[AX_LEFT_Y]) > deadZone)
				playerObj->MoveForward(- axes[AX_LEFT_Y] / 2.0);
            if (abs(axes[AX_RIGHT_X]) > deadZone)
                playerObj->MoveYaw(axes[AX_RIGHT_X] * 1.5 + axes[AX_RIGHT_X] * g_common.controllerSens * 2.0);
			break;
		case CTRL_EDITOR:
            playerObj->MovePitch(axes[AX_RIGHT_Y] / 2.0);
            playerObj->MoveRight(axes[AX_LEFT_X] / 4.0);
            playerObj->MoveForward(axes[AX_LEFT_Y] / 2.0);
            break;
        case CTRL_THIRDPERSON:
            // Movement
            break;
    }

    if (afterGestureTimer > 0) {
        afterGestureTimer--;
        return;
    }

    // Write axes into common
    g_common.gamepadLeftX = axes[AX_LEFT_X];
    g_common.gamepadLeftY = axes[AX_LEFT_Y];
    g_common.gamepadRightX = axes[AX_RIGHT_X];
    g_common.gamepadRightY = axes[AX_RIGHT_Y];
}

void Controls2::tickMouse()
{
	//
	// Movement
	//

	if (!g_common.showMouse)
	{
		if (enabled)
		{
            float mult = mouseMultiplier - (mouseMultiplier / 2.0f) + mouseMultiplier * g_common.mouseSens;
            
			switch (controlScheme) {
			case CTRL_FPS:
				playerObj->yaw += mouseX * mult;

				if (g_common.mouseLook)
				{
					playerObj->pitch -= mouseY * mult;
				}
				break;
			case CTRL_EDITOR:
				playerObj->yaw += mouseX * mult;
				//playerObj->pitch -= mouseY;
				break;
			}
		}
	}
	else
	{
		//
		// Click?
		//

		if (mouseButtons[0] == 1 && mouseClickTimer == 0)
		{
			std::string extra = gui->getOnClickExtraIfClicked(1, (int)glToScrX(mouseCursorX), (int)glToScrY(mouseCursorY), 0, 1);

			if (extra != "")
				g_common.extraInts[extra] = 1;

			mouseClickTimer = mouseClickTimerDelay;
		}
	}
}

void Controls2::touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2,
                      int actionIndex) {
    float width = screenWidth;
    float height = screenHeight;

    if (action1 == 4 || action2 == 4) {
        actionUp = true;
    }

    static int lastCount = 0;

    if (count == 2 && lastCount == 1) {
        lastCount = count;
        return;
    }

    if (count == 1 && lastCount == 2) {
        lastCount = count;
        jdown = false;
        jlastdownx = 0;
        jlastdowny = 0;
        return;
    }

    int action = action1;
    float x = 0.0;
    float y = 0.0;

    if (count == 1) {
        x = x1;
        y = y1;
    }

    lastCount = count;

    static float lastx = 0;
    static float lasty = 0;
    static float lastdownx = 0;
    static float lastdowny = 0;

    if (action == 1) {
        lastdownx = x;
        lastdowny = y;
    }

    float avgx = (x1 + x2) / 2;
    float avgy = (y1 + y2) / 2;
    static float lastavgx = 0;
    static float lastavgy = 0;

    // Pinches

    if (action == 3 && controlScheme == CTRL_EDITOR && count == 2) {
        static float lastdistx = 0;
        static float lastdisty = 0;

        float leftx;
        float rightx;

        if (x1 < x2) {
            leftx = x1;
            rightx = x2;
        } else {
            leftx = x2;
            rightx = x1;
        }

        float uppery;
        float lowery;

        if (y1 < x2) {
            lowery = y1;
            uppery = y2;
        } else {
            lowery = x2;
            uppery = x1;
        }

        float distx = rightx - leftx;
        float distxdiff = distx - lastdistx;
        float disty = uppery - lowery;
        float distydiff = disty - lastdisty;
		
/*	    Log("distx ", (int)distx * 100);
		Log("distxdiff ", (int)distxdiff * 100);
        Log("lastavgx ", (int)lastavgx * 100);*/

        // Pinch side to side
        if (distxdiff > -3 && distxdiff < 3) {
            if (lastavgx != 0) {
                float diffx = avgx - lastavgx;
			
                if (diffx < 0)
                    playerObj->MoveRight(diffx / 20.0);
                else
                    playerObj->MoveLeft(-diffx / 20.0);
            }

            lastavgx = avgx;
        }
        // Pinch up and down
        if (distydiff > -3 && distydiff < 3) {
            if (lastavgy != 0) {
                float diffy = avgy - lastavgy;

                if (diffy < 0)
                    playerObj->MoveDown(diffy / 20.0);
                else
                    playerObj->MoveUp(-diffy / 20.0);
            }

            lastavgy = avgy;
        }
        // Pinch forward and backward
        else if (lastdistx != 0) {
            if (distxdiff > 0)
                playerObj->MoveForward(distx / 200);
            else
                playerObj->MoveBackward(distx / 200);
        }

        lastdistx = distx;
        lastdisty = disty;

        jdown = false;
    }

    float hsize = 0.12;

    // Main joystick
    if (action == 1 || action == 2) {
        jlastdownx = x;
        jlastdowny = y;
        jdown = true;
    }

    if (action == 3) {
        jlastmovex = x;
        jlastmovey = y;
    }

    if (action == 4) {
        jlastdownx = 0;
        jlastdowny = 0;
        jdown = false;
    }

    lrjwentup = false;

    if ((action != 2 && action != 5) || actionIndex == 0)
        processTouchJoystick(false, action, x1, y1);
    if ((action != 2 && action != 5) || actionIndex == 1)
        processTouchJoystick(true, action, x2, y2);
    if ((action != 2 && action != 5) || actionIndex == 1)
        processTouchJoystick(false, action, x2, y2);
    if ((action != 2 && action != 5) || actionIndex == 0)
        processTouchJoystick(true, action, x1, y1);

    // Fingers up

    if (action == 4) {
        lastavgx = 0;
        lastavgy = 0;
    }

    // Taps

    static int tapTimer = 0;

    if (count == 2)
        tapTimer = 50;

    if (tapTimer > 0)
        tapTimer--;

    lastx = x;
    lasty = y;
}

void Controls2::processTouchJoystick(bool which, int action, float x, float y) {
    float width = screenWidth;
    float height = screenHeight;

    if (which == true) {
        // Right joystick
        if (x > width / 3 * 2 && y > height / 3 * 1.8) {
            if (action == 1) {
                rjlastdownx = x;
                rjlastdowny = y;
                rjdown = true;
            }

            if (action == 2) {
                rjlastdownx = x;
                rjlastdowny = y;
                rjlastmovex = x;
                rjlastmovey = y;
                rjdown = true;
            }

            if (action == 3) {
                rjlastmovex = x;
                rjlastmovey = y;
                rjdown = true;
            }

            if (action == 4 || action == 5) {
                rjdown = false;
            }
        }
		if (x > width / 2) {
			if (action == 4 || action == 5)
				rjdown = false;
		}

    } else {
        // Left joystick
        if (x < width / 3 * 1 && y > height / 3 * 1.8) {
            if (action == 1) {
                ljlastdownx = x;
                ljlastdowny = y;
                ljdown = true;
            }

            if (action == 2) {
                ljlastdownx = x;
                ljlastdowny = y;
                ljlastmovex = x;
                ljlastmovey = y;
                ljdown = true;
            }

            if (action == 3) {
                ljlastmovex = x;
                ljlastmovey = y;
                ljdown = true;
            }

            if (action == 4 || action == 5) {
                ljdown = false;
            }
        }
        if (x < width / 2) {
			if (action == 4 || action == 5)
				ljdown = false;
		}
    }
}

void Controls2::processGUI()
{
#ifndef PLATFORM_ANDROID
	for (int k = 0; k < NUM_KBD_KEYS; k++)
	{
		if (keyTimers[k] > 0) keyTimers[k]--;
	}

//	if (axes[AX_LEFT_Y] < -0.1)
//		gui->down();

	if (buttons[BTN_UP])
		gui->up();

	if (buttons[BTN_DOWN])
		gui->down();

    if (buttons[BTN_RIGHT])
        gui->right();
    
    if (buttons[BTN_LEFT])
        gui->left();

    // Gamepad A to select
	if (buttons[BTN_A])
	{
		aWasPressed = true;

		if (numPressedA >= 1)
		{
			if (gui->getSelectedWidget() != nullptr && gui->getSelectedWidget()->visible && gui->getSelectedWidget()->onClickExtra != "")
				g_common.extraInts[gui->getSelectedWidget()->onClickExtra] = 1;
			gui->playMenuSelectSound();
		}
	}
	else
	{
		if (aWasPressed)
			numPressedA++;
	}

	// Press space to select
	if (keys[32])
	{
		spaceWasPressed = true;

		if (numPressedSpace >= 1)
		{
			if (gui->getSelectedWidget() != nullptr && gui->getSelectedWidget()->visible && gui->getSelectedWidget()->onClickExtra != "")
				g_common.extraInts[gui->getSelectedWidget()->onClickExtra] = 1;
		}
	}
	else
	{
		if (spaceWasPressed)
			numPressedSpace++;
	}

	// Press enter to select
	if (keys[GLFW_KEY_ENTER] && keyTimers[GLFW_KEY_ENTER] == 0)
	{
        if (!gui->isInternallyGeneratedShown())
        {
		if (gui->getSelectedWidget() != nullptr && gui->getSelectedWidget()->visible && gui->getSelectedWidget()->onClickExtra != "")
				g_common.extraInts[gui->getSelectedWidget()->onClickExtra] = 1;
        }
        
		gui->enter();
		keyTimers[GLFW_KEY_ENTER] = keyDelay;
	}

	// Press enter to select
	if (keys[GLFW_KEY_ESCAPE])
	{
		gui->escape();
	}
#endif
}

void Controls2::resetGUI()
{
	numPressedA = 0;
	aWasPressed = false;

	numPressedSpace = 0;
	spaceWasPressed = false;

	numPressedEnter = 0;
	enterWasPressed = false;

	mouseMoved = false;

	gui->setSelectedWidget(nullptr);
}

/*void Controls2::ArrowDown(float factor)
{
	if (!enabled)
		return;

	switch (controlScheme)
	{
	case CTRL_THIRDPERSON:
		if (!compareYaw(playerObj->yaw, camera->yaw + 180, 10))
			playerObj->rotateYawTowards(camera->yaw + 180, 5);
		else
		{
			playerObj->yaw = camera->yaw + 180;
			playerObj->MoveForward(factor * getMoveFactor());
		}
//		playerObj->MoveBackward(factor * getMoveFactor());
		break;
	case CTRL_SCROLLINGSHOOTER:
		playerObj->MoveBackward(factor * getMoveFactor() * 2.0);
		break;
	case CTRL_UFOSHOOTER_360:
		playerObj->MoveBackward(factor * getMoveFactor() * 2.0);
		break;
	case CTRL_EDITOR:
		playerObj->MoveBackward(factor * getMoveFactor());
		break;
	}
}

void Controls2::ArrowUp(float factor)
{
	if (!enabled)
		return;

	switch (controlScheme)
	{
	case CTRL_THIRDPERSON:
		if (!compareYaw(playerObj->yaw, camera->yaw, 10))
			playerObj->rotateYawTowards(camera->yaw, 5);
		else
		{
			playerObj->yaw = camera->yaw;
			playerObj->MoveForward(factor * getMoveFactor());
		}
//		playerObj->MoveForward(factor * getMoveFactor());
		break;
	case CTRL_SCROLLINGSHOOTER:
		playerObj->MoveForward(factor * getMoveFactor() * 2.0);
		break;
	case CTRL_UFOSHOOTER_360:
		playerObj->MoveForward(factor * getMoveFactor() * 2.0);
		break;
	case CTRL_EDITOR:
		playerObj->MoveForward(factor * getMoveFactor());
		break;
	}
}

void Controls2::ArrowLeft(float factor)
{
	if (!enabled)
		return;
		
	switch (controlScheme)
	{
	case CTRL_THIRDPERSON:
		if (!compareYaw(playerObj->yaw, camera->yaw - 90, 10))
			playerObj->rotateYawTowards(camera->yaw - 90, 5);
		else
		{
			playerObj->yaw = camera->yaw + -90;
			playerObj->MoveForward(factor * getMoveFactor());
		}
//		playerObj->MoveYaw(-2);
		break;
	case CTRL_SCROLLINGSHOOTER:
		playerObj->MoveLeft(factor * getMoveFactor() * 10.0);
		break;
	case CTRL_UFOSHOOTER_360:
		playerObj->MoveLeft(factor * getMoveFactor());
		break;
	case CTRL_EDITOR:
//		std::string str = "player xyz " + ToString(playerObj->position.x) + " " + ToString(playerObj->position.y) + " " + ToString(playerObj->position.z);
//		Log(str);
		playerObj->MoveLeft(factor * getMoveFactor());
		break;
	}
}

void Controls2::ArrowRight(float factor)
{
	if (!enabled)
		return;
		
	switch (controlScheme)
	{
	case CTRL_THIRDPERSON:
		if (!compareYaw(playerObj->yaw, camera->yaw + 90, 10))
			playerObj->rotateYawTowards(camera->yaw + 90, 5);
		else
		{
			playerObj->yaw = camera->yaw + 90;
			playerObj->MoveForward(factor * getMoveFactor());
		}
//		playerObj->MoveYaw(2);
		break;
	case CTRL_SCROLLINGSHOOTER:
		playerObj->MoveRight(factor * getMoveFactor() * 10.0);
		break;
	case CTRL_UFOSHOOTER_360:
		playerObj->MoveRight(factor * getMoveFactor());
		break;
	case CTRL_EDITOR:
		playerObj->MoveRight(factor * getMoveFactor());
		break;
	}
}*/

void Controls2::MoveUp(float factor) {
    if (!enabled)
        return;

    switch (controlScheme) {
        case CTRL_EDITOR:
            playerObj->MoveUp(factor * getMoveFactor());
//			Logf("up", factor * getMoveFactor() * 1000.0);
            break;
    }
}

void Controls2::MoveDown(float factor) {
    if (!enabled)
        return;

    switch (controlScheme) {
        case CTRL_EDITOR:
            playerObj->MoveDown(factor * getMoveFactor());
//			Logf("down", factor * getMoveFactor() * 1000.0);
            break;
    }
}

void Controls2::MoveYaw(float amount) {
    switch (controlScheme) {
//	case CTRL_THIRDPERSON:
//		playerObj->MoveYaw(amount);
//		break;
//        case CTRL_UFOSHOOTER_360:
//            playerObj->MoveYaw(amount);
//            break;
        case CTRL_EDITOR:
            playerObj->MoveYaw(amount);
            break;
    }
}

void Controls2::MovePitch(float amount) {
    switch (controlScheme) {
//	case CTRL_THIRDPERSON:
//		playerObj->MovePitch(amount);
//		break;
        case CTRL_EDITOR:
            playerObj->MovePitch(amount);
            break;
    }
}

float Controls2::getMoveFactor() {
    return g_common.playerMoveSpeed;
}

void Controls2::setControlScheme(ControlSchemes scheme) {
    controlScheme = scheme;
}

void Controls2::BtnDown(int which) {
    if (!enabled)
        return;

    if (which == 0)
        buttons[0] = 1;
    if (which == 1)
        buttons[1] = 1;
}

void Controls2::BtnUp(int which) {
    if (!enabled)
        return;

    if (which == 0)
        buttons[0] = 0;
    if (which == 1)
        buttons[1] = 0;
}

void Controls2::minus() {
    switch (controlScheme) {
        case CTRL_EDITOR:
            break;
    }
}

void Controls2::equals() {
    switch (controlScheme) {
        case CTRL_EDITOR:
            break;
    }
}

void Controls2::plus() {
    switch (controlScheme) {
        case CTRL_EDITOR:
            editorController->plus();
            break;
    }
}

void Controls2::Draw2D(SpriteRenderer2D *r) {
    Texture *thumbstick = texMan->find("thumbstick.png");

    if (thumbstick == nullptr)
        return;

    int glTexID = thumbstick->glTexID;

    if (ljdown)
        r->DrawSprite(scrToGlX(ljlastmovex), scrToGlY(ljlastmovey), 0.1, 0.1, glTexID);
    if (rjdown)
        r->DrawSprite(scrToGlX(rjlastmovex), scrToGlY(rjlastmovey), 0.1, 0.1, glTexID);

/*	r->DrawSprite(0.7, 0.0, 0.15, 0.15, glTexID, 0.0, 0.0, 0.0, 0.3); // A
	r->DrawSprite(0.55, 0.2, 0.15, 0.15, glTexID, 0.0, 0.0, 0.0, 0.3); // X
	r->DrawSprite(0.85, 0.2, 0.15, 0.15, glTexID, 0.0, 0.0, 0.0, 0.3); // B
	r->DrawSprite(0.7, 0.4, 0.15, 0.15, glTexID, 0.0, 0.0, 0.0, 0.3); //

	r->DrawSprite(-0.7, 0.2, 0.4, 0.4, glTexID, 1, 1, 1, 0.3);*/

    r->DrawSprite(-0.7, -0.6, 0.4, 0.4, glTexID, 1.0, 1.0, 1.0, 0.3);
    r->DrawSprite(0.7, -0.6, 0.4, 0.4, glTexID, 1.0, 1.0, 1.0, 0.3);
}

void Controls2::drawMouse(SpriteRenderer2D* r) {
	if (mouseMoved)
	{
		Texture* mouse = texMan->find("mouse.png");
		r->DrawSprite(mouseCursorX, mouseCursorY, 0.1, 0.1, mouse->glTexID);
	}
}

float Controls2::scrToGlX(float screenX) {
    return 2.0f * screenX / screenWidth - 1.0f;
}

float Controls2::scrToGlY(float screenY) {
    return -(2.0f * screenY / screenHeight - 1.0f);
}

float Controls2::glToScrX(float glX) {
    return ((glX + 1.0f) / 2.0f) * screenWidth;
}

float Controls2::glToScrY(float glY) {
    return screenHeight - abs(((glY + 1.0f) / 2.0f) * screenHeight);
}

void Controls2::setTriggerAlwaysOn(bool value) {
    triggerAlwaysOn = value;
}

void Controls2::setEnabled(bool enabled) {
    this->enabled = enabled;
}

void Controls2::moveForwardToYaw(float yaw, float factor) {
    playerObj->setDeltaXZ(yaw, factor * 3.0);
    playerObj->move();
}

void Controls2::setFireYaw(float yaw) {
    fireYaw = yaw;
}

void Controls2::setPlayerObj(Object *playerObj) {
    this->playerObj = playerObj;
}

void Controls2::setPlayerRangeX(float min, float max) {
    minPlayerX = min;
    maxPlayerX = max;
}

void Controls2::setPlayerRangeY(float min, float max) {
    minPlayerY = min;
    maxPlayerY = max;
}

void Controls2::tap() {
    switch (controlScheme) {
        case CTRL_EDITOR:
            editorController->tap(0.0, 0.0);
            break;
    }
}

void Controls2::tap2() {
    switch (controlScheme) {
        case CTRL_EDITOR:
            editorController->tap2(0.0, 0.0);
            break;
    }
}

void Controls2::save() {
    editorController->save(defaultVoxelsFname);
}

void Controls2::load() {
    editorController->load(defaultVoxelsFname);
}

bool Controls2::checkActionUp() {
    bool result = actionUp;
    actionUp = false;
    return result;
}

void Controls2::mouse(float mouseX, float mouseY) {

	if (mouseX != 0.0f || mouseY != 0.0f)
		mouseMoved = true;

    this->mouseX = mouseX;
    this->mouseY = mouseY;

	mouseCursorX += mouseX / g_common.windowWidth;
	mouseCursorY -= mouseY / g_common.windowHeight; 
}

void Controls2::setBtn(int which, int state) {
    if (which < MAX_BUTTONS)
        buttons[which] = state;
}

int Controls2::getBtn(BtnNames which) {
    if (which < MAX_BUTTONS)
         return buttons[which];
    else
        return 0;
}

void Controls2::addTouchBtnBind(int btn, float x, float y, float size) {
    TouchBtnBind tbb;

    tbb.btn = btn;
    tbb.x = x;
    tbb.y = y;
    tbb.size = size;

    touchBtnBinds.push_back(tbb); 
} 

void Controls2::remTouchBtnBind(int btn) {
	std::vector<TouchBtnBind>::iterator it;
	std::vector<TouchBtnBind>::iterator itRem;   
	int i = 0;
	int pos = -1;

	for (it = touchBtnBinds.begin(); it != touchBtnBinds.end(); it++, i++)
	{
		if (it->btn == btn)
		{
			pos = i;
			itRem = it;
		}
	}

	if (pos != -1)
		touchBtnBinds.erase(itRem);
}

void Controls2::setAxis(int axis, float value) {
    axes[axis] = value;
}

void Controls2::setKey(int key, int val) {
    if (key < NUM_KBD_KEYS)
        keys[key] = val;
}

int Controls2::getKey(int key) {
    if (key < NUM_KBD_KEYS)
        return keys[key];
    else
        return 0;
}

float Controls2::getAxis(int axis) {
	if (axis < NUM_AXES)
		return axes[axis];
	else
		return 0;
}

void Controls2::setMouseBtn(int which, int state) {
    if (which < 3)
        mouseButtons[which] = state;
}

int Controls2::getMouseBtn(int which) {
    if (which < 3)
        return mouseButtons[which];

    return 0;
}

float Controls2::getMouseCursorX()
{
    return mouseCursorX;
}

float Controls2::getMouseCursorY()
{
    return mouseCursorY;
}
