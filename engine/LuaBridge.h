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

#ifndef LuaBridge_h
#define LuaBridge_h

#include "Globals.hpp"
#include "Engine2.h"

extern "C"
{
#include "../thirdparty/lua/lua.h"
#include "../thirdparty/lua/lauxlib.h"
#include "../thirdparty/lua/lobject.h"
#include "../thirdparty/lua/lstate.h"
#include "../thirdparty/lua/lundump.h"
#include "../thirdparty/lua/lualib.h"
}

class LuaBridge
{
public:
    void init(Engine2 *engine);
	void exec(std::string filename);
	void execAsset(std::string filename);
	void execExternal(std::string filename);
	void execExternalFiles(std::string filename);

	std::string getErrorMsg() { return errorMsg; };
	
private:
    Engine2 *engine;
	lua_State *L;
	std::string errorMsg = "";
};

#endif
