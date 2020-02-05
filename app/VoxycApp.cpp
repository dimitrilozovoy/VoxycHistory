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

#include "VoxycApp.h"
#include "../engine/DDLUtils.hpp"
#include "../engine/platform.h"
#include "../editor/OrthoEditor.h"

#ifdef PLATFORM_ANDROID
#include "../platform/android/Voxyc/app/src/main/cpp/android.h"
#endif

void VoxycApp::init()
{
	Log("Voxyc");
	Log("*");
	Log("*");
	Log("*");

	g_engine2 = &engine;

	if (firstLaunch)
	{
#if defined PLATFORM_ANDROID
#ifdef USE_EXTERNAL_ASSETS
        char cdir[MAX_STR_LEN];
        sprintf(cdir, "%s/%s", g_externalFilesDir, EXTERNAL_PROJECT_DIR);
        g_assetsDir = cdir;
#else
		g_assetsDir = g_externalFilesDir;
#endif
#endif

		module = g_module;
	}

	if (module == "editor")
		editor.init();
	if (module == "editor2")
		editor2.init();
	else if (module == "orthoeditor")
        orthoEditor.init();
	else if (module == "modeleditor")
        modelEditor.init();
	else if (module == "luaprogram")
		luaProgram.init();

	firstLaunch = false;
}

void VoxycApp::load()
{
	g_engine2 = &engine;
	
	tickCount = 0;
	updatedTime = 0;

	if (module == "editor")
		editor.load();
	if (module == "editor2")
		editor2.load();
	else if (module == "orthoeditor")
		orthoEditor.load();
	else if (module == "modeleditor")
		modelEditor.load();
	else if (module == "luaprogram")
		luaProgram.load();
}

void VoxycApp::tick()
{	
	int numLoops = 0;

	// Mouse needs to be updated on every loop
	engine.getControls()->tickMouse();
	
	if (!g_common.fixedTimestep)
	{
	    fixedTick();
	}
    else
	{
	    long msecInterval = 1000 / targetFps;
	
	    if (updatedTime == 0)
	         updatedTime = PLAT_GetTime() - msecInterval;
		
	    unsigned long curTime = PLAT_GetTime();
	
	    while ((updatedTime < curTime) && (curTime - updatedTime) > msecInterval)
	    {
		    fixedTick();

		    updatedTime += msecInterval;
			
		    numLoops++;

			if (numLoops >= 100)
			{
				updatedTime = PLAT_GetTime();
				break;
			}
	    }
    }
}
	
void VoxycApp::fixedTick()
{
//	Log(module);
	
	if (loadingModule)
	{
		load();
		loadingModule = false;
		return;
	}
	
	engine.callObjTickStarts();

	// Tick controls before processing scripts
	engine.getControls()->tick();
	
	engine.callObjTickStartsPostControl();

	if (module == "editor")
		editor.tick();
	if (module == "editor2")
	    editor2.tick();	
	else if (module == "orthoeditor")
		orthoEditor.tick();
    else if (module == "modeleditor")
		modelEditor.tick();
	else if (module == "luaprogram")
		luaProgram.tick();

	engine.callObjTickEnds();
	
	engine.tick();
		
    // Switch module
	int switchModule = engine.getExtraInt("switchmodule");
	std::string nextModule = engine.getExtraStr("nextmodule");
	
	if (switchModule == 1)
	{
		lastModule = module;
		module = nextModule;
		engine.clear();
		engine.clearGUI();
		init();
		engine.setExtraInt("switchmodule", 0);
		loadingModule = true;
		skipFrame = true;
	}
	
	tickCount++;
}

void VoxycApp::draw(int eye)
{
	if (skipFrame)
	{
		skipFrame = false;
		return;
	}
	
	engine.draw(eye);	
}

void VoxycApp::setModule(std::string m)
{
	module = m;
}

void VoxycApp::calcFrameRate()
{
	// Calculate frame rate
	long long curTime = getTimeInMsec();

	if (lastFinishDrawTime != -1)
	{
		long long timeDiff = curTime - lastFinishDrawTime;

		fps = 0;

		if (timeDiff != 0)
			fps = 1000 / timeDiff;

		//	Log("FPS", timeDiff);
		//	Log("timeDiff", timeDiff);
	}

	lastFinishDrawTime = getTimeInMsec();
}

void VoxycApp::makeUpLostFramesOrWait()
{
	// Make up for lost frames

	const float goalTicksPerSec = 60;
	long long curTime = getTimeInMsec();
	static float totalMakeUpRatio = 0.0f;
	long long timeDiff = 0;
	float makeUpTicks = 0;

	if (lastFinishTickTime != -1)
	{
		timeDiff = curTime - lastFinishTickTime;

		float tps = 1000.0f / timeDiff;

		makeUpTicks = goalTicksPerSec - tps;
		
		float makeUpRatio = 0;

		if (makeUpTicks > 0)
		{
			float lagRatio = tps / goalTicksPerSec;
			makeUpRatio = 1 - lagRatio;
		}

		totalMakeUpRatio += makeUpRatio;
	}

	if (totalMakeUpRatio >= 1.0f)
	{
		tick();
		totalMakeUpRatio = 0.0f;
	}

	// Slow down if needed
	if (timeDiff < 16)
	{
		ddlsleep(abs(16 - timeDiff));
	}
}

void VoxycApp::free()
{
	engine.free();
}

void VoxycApp::touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2, int actionIndex)
{
	engine.touchEvent(count, action1, x1, y1, action2, x2, y2, actionIndex);
	
	if (module == "orthoeditor")
		orthoEditor.touchEvent(count, action1, x1, y1, action2, x2, y2, actionIndex);
	if (module == "modeleditor")
		modelEditor.touchEvent(count, action1, x1, y1, action2, x2, y2, actionIndex);
}
