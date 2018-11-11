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

void Controls2::init(Object *camera, Object *mouseLook, TextureManager2 *texMan, EditorController *editorController)
{
    this->camera = camera;
	this->mouseLook = mouseLook;
    this->texMan = texMan;
	this->editorController = editorController;

    texMan->load("thumbstick.png");
}

void Controls2::tick()
{
	if (!enabled)
		return;

	screenWidth = PLAT_GetWindowWidth();
    screenHeight = PLAT_GetWindowHeight();
	
	if (jdown)
    {
        float xdiff = jlastmovex - jlastdownx;
        float ydiff = jlastmovey - jlastdowny;

		switch (controlScheme)
		{
		case CTRL_SCROLLINGSHOOTER:
            if (xdiff > 0.2)
                playerObj->MoveRight(abs(xdiff) * getMoveFactor() / 10.0);
            if (xdiff < -0.2)
                playerObj->MoveLeft(abs(xdiff) * getMoveFactor() / 10.0);
			break;
		case CTRL_SCROLLINGSHOOTERXY:
            if (xdiff > 0.2)
                playerObj->MoveRight(abs(xdiff) * getMoveFactor() / 30.0);
            if (xdiff < -0.2)
                playerObj->MoveLeft(abs(xdiff) * getMoveFactor() / 30.0);
            if (ydiff > 0.2)
                playerObj->MoveDown(abs(ydiff) * getMoveFactor() / 30.0);
            if (ydiff < -0.2)
                playerObj->MoveUp(abs(ydiff) * getMoveFactor() / 30.0);
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

    if (ljdown)
    {
        float xdiff = ljlastmovex - ljlastdownx;
        float ydiff = ljlastmovey - ljlastdowny;

		switch (controlScheme)
		{
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

    if (rjdown)
    {
        float xdiff = rjlastmovex - rjlastdownx;
        float ydiff = rjlastmovey - rjlastdowny;

//		Logf("rjdown ydiff ", ydiff);

		switch (controlScheme)
		{
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
            if (ydiff < -0.2)
                ArrowUp(abs(ydiff) / 140.0);
            if (ydiff > 0.2)
                ArrowDown(abs(ydiff) / 140.0);
            if (xdiff > 0.2)
                ArrowRight(abs(xdiff) / 140.0);
            if (xdiff < -0.2)
                ArrowLeft(abs(xdiff) / 150.0);
			break;
		}
    }
	
	if (afterGestureTimer > 0)
	{
		afterGestureTimer--;
		return;
	}
}

void Controls2::touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2)
//void Controls2::touchEvent(int action, float x, float y, int finger, int count)
{
    float width = screenWidth;
    float height = screenHeight;
	
//	Log("x1", x1);
//  Log("y1", y1);
	
	if (action1 == 3 || action2 == 3)
	{
//		Log("actionUp");
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
	
/*	if (count == 1 && lastCount == 2) {
        afterGestureTimer = 5;
		jdown = false;
        return;
    }*/
	
	int action = action1;
	float x = 0.0;
	float y = 0.0;
	
	if (count == 1)
	{
		x = x1;
		y = y1;
	}

    lastCount = count;
	
	static float lastx = 0;
	static float lasty = 0;
	static float lastdownx = 0;
	static float lastdowny = 0;
	
//	std::string log = "x1 " + ToString((int)x1) + " y1 " + ToString((int)y1) + " x2 " + ToString((int)x2) + " y2 " + ToString((int)y2);
//  Log(log);
	
//	Log("glToScrY(scrToGlY(y))", glToScrY(scrToGlY(y)));

//	Log("x", (int)glToScrX(x));
//	Log("y", (int)glToScrY(y));

//  if (action == 2)
//  {
//    rjdown = false;
//    ljdown = false;
//  }
	
	if (action == 1)
	{
		lastdownx = x;
		lastdowny = y;
	}
	
	float avgx = (x1 + x2) / 2;
	float avgy = (y1 + y2) / 2;
	static float lastavgx = 0;
	static float lastavgy = 0;
	
	// Pinches
	
	if (action == 2 && controlScheme == CTRL_EDITOR && count == 2)
	{
		static float lastdistx = 0;
		static float lastdisty = 0;
			
		float leftx;
		float rightx;

		if (x1 < x2)
		{
			leftx = x1;
			rightx = x2;
		}
		else
		{
			leftx = x2;
	        rightx = x1;
        }
		
		float uppery;
		float lowery;

		if (y1 < x2)
		{
			lowery = y1;
			uppery = y2;
		}
		else
		{
			lowery = x2;
	        uppery = x1;
        }
		
		float distx = rightx - leftx;
		float distxdiff = distx - lastdistx;
		float disty = uppery - lowery;
		float distydiff = disty - lastdisty;
		
		// Pinch side to side
		if (distxdiff > -3 && distxdiff < 3)
		{
			if (lastavgx != 0)
			{
				float diffx = avgx - lastavgx;
				
				if (diffx < 0)
					playerObj->MoveRight(diffx / 20.0);
				else
				    playerObj->MoveLeft(-diffx / 20.0);
			}
			
			lastavgx = avgx;
		}
		// Pinch up and down
		if (distydiff > -3 && distydiff < 3)
		{
			if (lastavgy != 0)
			{
				float diffy = avgy - lastavgy;
				
				if (diffy < 0)
					playerObj->MoveDown(diffy / 20.0);
				else
				    playerObj->MoveUp(-diffy / 20.0);
			}
			
			lastavgy = avgy;
		}
		// Pinch forward and backward
		else if (lastdistx != 0)
		{			
			if (distxdiff > 0)
			    playerObj->MoveForward(distx / 200);
			else
				playerObj->MoveBackward(distx / 200);
		}
			
		lastdistx = distx;
		lastdisty = disty;
		
		jdown = false;
	}
	else
    {
        lastavgx = 0;
        float hsize = 0.12;

	    // Main joystick
	    if (action == 1)
        {
            jlastdownx = x;
            jlastdowny = y;
            jdown = true;
        }

        if (action == 2)
        {
            jlastmovex = x;
            jlastmovey = y;
        }

        if (action == 3)
        {
	        jlastdownx = 0;
            jlastdowny = 0;
            jdown = false;
        }

        // Right joystick
        if (x > width / 3 * 2 && y > height / 3 * 1.8)
        {
            if (action ==1)
            {
                rjlastdownx = x;
                rjlastdowny = y;
                rjdown = true;
            }

            if (action == 2)
            {
                rjlastmovex = x;
                rjlastmovey = y;
            }

            if (action == 3)
            {
                rjdown = false;
            }
        }

        // Left joystick
        if (x < width / 3 * 1 && y > height / 3 * 1.8)
        {
            if (action == 1)
            {
                ljlastdownx = x;
                ljlastdowny = y;
                ljdown = true;
            }

            if (action == 2)
            {
                ljlastmovex = x;
                ljlastmovey = y;
            }

            if (action == 3)
            {
                ljdown = false;
            }
        }
    }
	
	// Fingers up
	
	if (action == 3)
	{
		lastavgx = 0;
		lastavgy = 0;
	}

	// Taps
	
	static int tapTimer = 0;
	
	if (count == 2)
		tapTimer = 50;
		
	if (tapTimer > 0)
		tapTimer--;
	
	if (tapTimer == 0 && action == 3 && controlScheme == CTRL_EDITOR && count == 1)
	{
//		editorController->tap(scrToGlX(x), scrToGlY(y));
	}
	
	lastx = x;
	lasty = y;
}

void Controls2::ArrowDown(float factor)
{
	if (!enabled)
		return;

	switch (controlScheme)
	{
	case CTRL_THIRDPERSON:
		playerObj->MoveBackward(factor * getMoveFactor());
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
		playerObj->MoveForward(factor * getMoveFactor());
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
		playerObj->MoveYaw(-2);
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
		playerObj->MoveYaw(2);
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
}

void Controls2::MoveUp(float factor)
{
	if (!enabled)
		return;

	switch (controlScheme)
	{
	case CTRL_EDITOR:
		playerObj->MoveUp(factor * getMoveFactor());
//			Logf("up", factor * getMoveFactor() * 1000.0);
		break;
	}
}

void Controls2::MoveDown(float factor)
{
	if (!enabled)
		return;

	switch (controlScheme)
	{
	case CTRL_EDITOR:
		playerObj->MoveDown(factor * getMoveFactor());
//			Logf("down", factor * getMoveFactor() * 1000.0);
		break;
	}
}

void Controls2::MoveYaw(float amount)
{
	switch (controlScheme)
	{
//	case CTRL_THIRDPERSON:
//		playerObj->MoveYaw(amount);
//		break;
	case CTRL_UFOSHOOTER_360:
		playerObj->MoveYaw(amount);
		break;
	case CTRL_EDITOR:
		playerObj->MoveYaw(amount);
		break;
	}
}

void Controls2::MovePitch(float amount)
{
	switch (controlScheme)
	{
//	case CTRL_THIRDPERSON:
//		playerObj->MovePitch(amount);
//		break;
	case CTRL_EDITOR:
		playerObj->MovePitch(amount);
		break;
	}
}

float Controls2::getMoveFactor()
{
	switch (controlScheme)
	{
	case CTRL_EDITOR:
#ifdef PLATFORM_ANDROID
		return 0.08;
#else
        return 0.5;
#endif
	case CTRL_FPS:
		if (g_largeScale)
			return 0.003;
		else
			return 0.03;
		break;
	case CTRL_UFOSHOOTER_360:
		return 2.2;
		break;
	case CTRL_SCROLLINGSHOOTER:
		return 0.3;
		break;
	}
    
    return 1.0;
}

void Controls2::setControlScheme(ControlSchemes scheme)
{
	controlScheme = scheme;
}

void Controls2::BtnDown(int which)
{
	if (!enabled)
		return;

	if (which == 0)
	{
		tap();
	}

	if (which == 1)
	{
		tap2();
	}
}

void Controls2::BtnUp(int which)
{
	if (!enabled)
		return;
}

void Controls2::minus()
{
	switch (controlScheme)
	{
	case CTRL_EDITOR:
		editorController->minus();
		break;
	}
}

void Controls2::equals()
{
	switch (controlScheme)
	{
	case CTRL_EDITOR:
		editorController->plus();
		break;
	}
}

void Controls2::plus()
{
	switch (controlScheme)
	{
	case CTRL_EDITOR:
		editorController->plus();
		break;
	}
}

void Controls2::Draw2D(SpriteRenderer2D *r)
{
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

float Controls2::scrToGlX(float screenX)
{
	return 2.0f * screenX / screenWidth - 1.0f;
}

float Controls2::scrToGlY(float screenY)
{
	return -(2.0f * screenY / screenHeight - 1.0f);
}

float Controls2::glToScrX(float glX)
{
	return ((glX + 1.0f) / 2.0f) * screenWidth;
}

float Controls2::glToScrY(float glY)
{
	return screenHeight - abs(((glY + 1.0f) / 2.0f) * screenHeight);
}

void Controls2::setTriggerAlwaysOn(bool value)
{
    triggerAlwaysOn = value;
}

void Controls2::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

void Controls2::moveForwardToYaw(float yaw, float factor)
{
	playerObj->setDeltaXZ(yaw, factor * 3.0);
	playerObj->move();
}

void Controls2::setFireYaw(float yaw)
{
	fireYaw = yaw;
}

void Controls2::setPlayerObj(Object *playerObj)
{
	this->playerObj = playerObj;
}

void Controls2::setPlayerRangeX(float min, float max)
{
	minPlayerX = min;
	maxPlayerX = max;
}

void Controls2::setPlayerRangeY(float min, float max)
{
	minPlayerY = min;
	maxPlayerY = max;
}

void Controls2::tap()
{
	switch (controlScheme)
	{
	case CTRL_EDITOR:
		editorController->tap(0.0, 0.0);
		break;
	}
}

void Controls2::tap2()
{
	switch (controlScheme)
	{
	case CTRL_EDITOR:
		editorController->tap2(0.0, 0.0);
		break;
	}
}

void Controls2::save()
{
	editorController->save(defaultVoxelsFname);
}

void Controls2::load()
{
	editorController->load(defaultVoxelsFname);
}

bool Controls2::checkActionUp()
{
	bool result = actionUp;
    actionUp = false;
	return result;
}
