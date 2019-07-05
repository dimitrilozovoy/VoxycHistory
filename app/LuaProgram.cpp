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
#include "LuaProgram.h"
#include "../engine/Engine2.h"
#include "../engine/DDLUtils.hpp"

void LuaProgram::init()
{
	g_engine2->init();
	this->engine = g_engine2;
	
	luaBridge.init(g_engine2);

//	engine->setControlScheme(CTRL_EDITOR);
	
	// Draw splash while we are loading
/*	engine->addText("loading", "loading", 0.0, 0.0, 0.1);
	engine->setTextVisible("loading", true);
    engine->setControlsVisible(false);
    engine->setHealthBarsVisible(false);
    engine->setSkybox("skybox.png");*/
	
	g_engine2->draw(0);
}

void LuaProgram::load()
{
	std::string fname;

	if (engine->getExtraStr("assetsdir") != "")
		engine->setAssetsDir(engine->getExtraStr("assetsdir"));

	if (engine->getExtraStr("loadscript") == "")
		fname = "load.lua";
	else
		fname = engine->getExtraStr("loadscript");
		
    luaBridge.exec(fname);
}

void LuaProgram::tick()
{
	std::string fname;

	if (engine->getExtraStr("tickscript") == "")
		fname = "tick.lua";
	else
		fname = engine->getExtraStr("tickscript");
		
	engine->setTextVisible("loading", false);

//	Log(fname);
	luaBridge.exec(fname);
}
