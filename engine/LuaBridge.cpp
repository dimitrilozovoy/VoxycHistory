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

#include "DDLUtils.hpp"
#include "LuaBridge.h"
#include "Engine2.h"
#include "platform.h"

#ifdef PLATFORM_ANDROID
#ifdef GAME_RELEASE_DIR_TREE
#include "../android.h"
#else
#include "../platform/android/Voxyc/app/src/main/cpp/android.h"
#endif
#endif

static int isquare(lua_State *L) {
	float rtrn = lua_tonumber(L, -1);
	Log("Top of square(), nbr=%f\n", rtrn);
	lua_pushnumber(L,rtrn*rtrn);
	return 1;
}

static int isquare2(lua_State *L) {
	const char *rtrn = lua_tostring(L, -1);
	Log("square2\n", (char *)rtrn);
	lua_pushstring(L, "www");
	return 1;
}

static int addobj(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	g_engine2->addObject(name);
	
	return 0;
}

static int remobj(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	g_engine2->removeObject(name);
	
	return 0;
}

static int rename(lua_State *L)
{
	std::string from = lua_tostring(L, 1);
	std::string to = lua_tostring(L, 2);

	g_engine2->rename(from, to);

	return 0;
}

static int settype(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string typeStr = lua_tostring(L, 2);
	
	ObjType type;
	
    if (typeStr == "sprite")
	    type = OBJTYPE_SPRITE;
    if (typeStr == "shape")
	    type = OBJTYPE_SHAPE;
	else if (typeStr == "model")
	    type = OBJTYPE_MODEL;

	g_engine2->setType(name, type);
	
	return 0;
}

static int setshape(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string shapeStr = lua_tostring(L, 2);
	
//	Log(shapeStr);
	
	if (shapeStr == "sprite")
	{
//		Log("setshape quad");
	    g_engine2->setShape(name, SHAPE_SPRITE);
	}
	else if (shapeStr == "quad")
	{
//		Log("setshape quad");
	    g_engine2->setShape(name, SHAPE_QUAD);
	}
	else
	{
	    g_engine2->setShape(name, shapeStr);
	}
	
	return 0;
}

static int setmodel(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string model = lua_tostring(L, 2);
	
	g_engine2->setModel(name, model);
	
	return 0;
}

static int getmodel(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);
	std::string modelName = obj->modelName;

	lua_pushstring(L, modelName.c_str());

	return 1;
}

static int setmodelorient(lua_State *L)
{
	std::string model = lua_tostring(L, 1);
	float pitch = lua_tonumber(L, 2);
	float yaw = lua_tonumber(L, 3);
	float roll = lua_tonumber(L, 4);

	g_engine2->setModelOrientation(model, pitch, yaw, roll);

	return 0;
}

static int setcolor(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number r = lua_tonumber(L, 2);
    lua_Number g = lua_tonumber(L, 3);
    lua_Number b = lua_tonumber(L, 4);
	lua_Number a = lua_tonumber(L, 5);
	
    g_engine2->setColor(name, r, g, b, a);
	
	return 0;
}

static int setmeshcolor(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number mesh = lua_tonumber(L, 2);
	lua_Number r = lua_tonumber(L, 3);
    lua_Number g = lua_tonumber(L, 4);
    lua_Number b = lua_tonumber(L, 5);
	lua_Number a = lua_tonumber(L, 6);
	
    g_engine2->setMeshColor(name, mesh, r, g, b, a);
	
	return 0;
}

static int settex(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string tex = lua_tostring(L, 2);
	
	g_engine2->setTexture(name, tex);
	
	return 0;
}

static int gettex(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);

	if (obj == nullptr)
		return 0;

	lua_pushstring(L, obj->textureName.c_str());

	return 1;
}

static int settexspan(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);
    lua_Number y = lua_tonumber(L, 3);
	
    g_engine2->setTextureSpan(name, x, y);
	
	return 0;
}

static int setvisible(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	bool visible = lua_toboolean(L, 2);

    g_engine2->setVisible(name, visible);
	
	return 0;
}

static int getvisible(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	lua_Number visn;

	lua_pushboolean(L, obj->visible);

	return 1;
}

static int setsize(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);
    lua_Number y = lua_tonumber(L, 3);
    lua_Number z = lua_tonumber(L, 4);
	
    g_engine2->setSize(name, x, y, z);
	
	return 1;
}

static int getsize(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);

	if (obj == nullptr)
		return 0;

	glm::vec3 scale = obj->scale;

	lua_pushnumber(L, scale.x);
	lua_pushnumber(L, scale.y);
	lua_pushnumber(L, scale.z);

	return 3;
}

static int setphyssize(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);
	lua_Number y = lua_tonumber(L, 3);
	lua_Number z = lua_tonumber(L, 4);

	g_engine2->setPhysSize(name, x, y, z);

	return 1;
}

static int setpos(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);
    lua_Number y = lua_tonumber(L, 3);
    lua_Number z = lua_tonumber(L, 4);
	
    g_engine2->setPos(name, x, y, z);
	
	return 1;
}

static int getpos(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	
    glm::vec4 pos = g_engine2->getPos(name);
	
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
    lua_pushnumber(L, pos.z);
	
	return 3;
}

static int setx(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	obj->position.x = x;

	return 1;
}

static int getx(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	lua_pushnumber(L, obj->position.x);

	return 1;
}

static int sety(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number y = lua_tonumber(L, 2);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	obj->position.y = y;

	return 1;
}

static int gety(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	lua_pushnumber(L, obj->position.y);

	return 1;
}

static int setz(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number z = lua_tonumber(L, 2);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	obj->position.z = z;

	return 1;
}

static int getz(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	lua_pushnumber(L, obj->position.z);

	return 1;
}

static int setvelocity(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);
	lua_Number y = lua_tonumber(L, 3);
	lua_Number z = lua_tonumber(L, 4);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
	obj->velocity = glm::vec4(x, y, z, 1.0);

	return 1;
}

static int getvelocity(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;

	lua_pushnumber(L, obj->velocity.x);
	lua_pushnumber(L, obj->velocity.y);
	lua_pushnumber(L, obj->velocity.z);

	return 3;
}

static int setorient(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number pitch = lua_tonumber(L, 2);
    lua_Number yaw = lua_tonumber(L, 3);
    lua_Number roll = lua_tonumber(L, 4);
	
    g_engine2->setOrientation(name, pitch, yaw, roll);
	
	return 1;
}

static int getorient(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

   // glm::vec4 orient = g_engine2-getOrientation(name);
	
	//lua_pushnumber(L, pos.y);
	//lua_pushnumber(L, pos.x);
   // lua_pushnumber(L, pos.z);
	
	return 3;
}

static int setyaw(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number yaw = lua_tonumber(L, 2);
	
    g_engine2->setYaw(name, yaw);
	
	return 1;
}

static int setpitch(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number pitch = lua_tonumber(L, 2);
	
    g_engine2->setPitch(name, pitch);
	
	return 1;
}

static int setroll(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number roll = lua_tonumber(L, 2);
	
    g_engine2->setRoll(name, roll);
	
	return 1;
}

static int setsecondaryyaw(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number yaw = lua_tonumber(L, 2);

	Object *o = g_engine2->findObj(name);

	if (o == nullptr)
		return 0;

	o->secondaryYaw = yaw;

	return 0;
}

static int getyaw(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
    float val = g_engine2->getYaw(name);
	lua_pushnumber(L, val);
	
	return 1;
}

static int getpitch(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
    float val = g_engine2->getPitch(name);
	lua_pushnumber(L, val);
	
	return 1;
}

static int getroll(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
    float val = g_engine2->getRoll(name);
	lua_pushnumber(L, val);
	
	return 1;
}

static int getsecondaryyaw(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *o = g_engine2->findObj(name);

	if (o == nullptr)
		return 0;

	float val = o->secondaryYaw;
	lua_pushnumber(L, val);

	return 1;
}

static int moveforward(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number distance = lua_tonumber(L, 2);
	
	Object *o = g_engine2->findObj(name);
	
	if (o == nullptr)
		return 0;
		
    o->setDeltaXYZ(o->yaw, o->pitch, distance);
	o->move();
	
	return 0;
}

static int setmovesmoothly(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	bool val = lua_toboolean(L, 2);

	g_engine2->setMoveSmoothly(name, val);
	
	return 1;
}

static int setfade(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	int near = lua_tonumber(L, 2);
	int far = lua_tonumber(L, 2);

	g_engine2->setFade(name, near, far);

	return 1;
}

static int setfaceplayer(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	g_engine2->setAlwaysFacePlayer(name, true);
	
	return 0;
}

static int setobjstr(lua_State *L)
{
	std::string objname = lua_tostring(L, 1);
	std::string name = lua_tostring(L, 2);
	std::string val = lua_tostring(L, 3);
	
	Object *o = g_engine2->findObj(objname);
	
	if (o == nullptr)
		return 0;

	o->strings[name] = val;
	
	return 0;
}

static int getobjstr(lua_State *L)
{
	std::string objname = lua_tostring(L, 1);
	std::string name = lua_tostring(L, 2);
	
	Object *o = g_engine2->findObj(objname);
	
	if (o == nullptr)
		return 0;
	
    std::string val = o->strings[name];
	lua_pushstring(L, val.c_str());
	
	return 1;
}

static int setobjint(lua_State *L)
{
	std::string objname = lua_tostring(L, 1);
	std::string name = lua_tostring(L, 2);
	int val = lua_tonumber(L, 3);
	
	Object *o = g_engine2->findObj(objname);
	
	if (o == nullptr)
		return 0;

	o->ints[name] = val;
	
	return 0;
}

static int getobjint(lua_State *L)
{
	std::string objname = lua_tostring(L, 1);
	std::string name = lua_tostring(L, 2);

	Object *o = g_engine2->findObj(objname);
	
	if (o == nullptr)
		return 0;
	
    int val = o->ints[name];
	lua_pushnumber(L, val);
	
	return 1;
}

static int setobjfloat(lua_State *L)
{
	std::string objname = lua_tostring(L, 1);
	std::string name = lua_tostring(L, 2);
	float val = lua_tonumber(L, 3);
	
	Object *o = g_engine2->findObj(objname);
	
	if (o == nullptr)
		return 0;

	o->floats[name] = val;
	
	return 0;
}

static int getobjfloat(lua_State *L)
{
	std::string objname = lua_tostring(L, 1);
	std::string name = lua_tostring(L, 2);

	Object *o = g_engine2->findObj(objname);
	
	if (o == nullptr)
		return 0;
	
    float val = o->floats[name];
	lua_pushnumber(L, val);
	
	return 1;
}

static int newshape(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string shapeStr = lua_tostring(L, 2);
	
	ObjShapeType shape;
	
    if (shapeStr == "sprite")
	    shape = SHAPE_SPRITE;	
    if (shapeStr == "quad")
	    shape = SHAPE_QUAD;	
    if (shapeStr == "block")
	    shape = SHAPE_BLOCK;	
    if (shapeStr == "terrain")
	    shape = SHAPE_TERRAIN;	
    if (shapeStr == "buildingwall")
	    shape = SHAPE_BUILDINGWALL;	
	if (shapeStr == "voxels")
		shape = SHAPE_VOXELS;
	if (shapeStr == "custom")
	    shape = SHAPE_CUSTOM;
		
	lua_Number a = lua_tonumber(L, 3);
    lua_Number b = lua_tonumber(L, 4);
    lua_Number c = lua_tonumber(L, 5);
	lua_Number d = lua_tonumber(L, 6);
	lua_Number e = lua_tonumber(L, 7);
	
	g_engine2->newShape(name, shape, a, b, c, d, e);
	
	return 1;
}

static int delshape(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
		
	g_engine2->deleteShape(name);
	
	return 1;
}

static int loadmodel(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	
	g_engine2->loadModel(name);
	
	return 1;
}

static int setskybox(lua_State *L)
{
	std::string texname = lua_tostring(L, 1);
	
	g_engine2->setSkybox(texname);
	
	return 1;
}

static int setcampos(lua_State *L)
{
	lua_Number x = lua_tonumber(L, 1);
    lua_Number y = lua_tonumber(L, 2);
    lua_Number z = lua_tonumber(L, 3);
	
    g_engine2->setCameraPos(x, y, z);
	
	return 1;
}

static int setcamorient(lua_State *L)
{
	lua_Number yaw = lua_tonumber(L, 1);
    lua_Number pitch = lua_tonumber(L, 2);
    lua_Number roll = lua_tonumber(L, 3);
	
    g_engine2->setCameraOrientation(yaw, pitch, roll);
	
	return 1;
}

static int setplayerobj(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	
	g_engine2->setPlayerObj(name);
	
	return 1;
}

static int setplayerpos(lua_State *L)
{
	lua_Number x = lua_tonumber(L, 1);
    lua_Number y = lua_tonumber(L, 2);
    lua_Number z = lua_tonumber(L, 3);
	
    g_engine2->setPlayerPos(x, y, z);
	
	return 1;
}

static int getplayerpos(lua_State *L)
{
	Object *player = g_engine2->getPlayerObj();

	lua_pushnumber(L, player->position.x);
	lua_pushnumber(L, player->position.y);
	lua_pushnumber(L, player->position.z);

	return 3;
}

static int setplayersize(lua_State *L)
{
	Object *player = g_engine2->getPlayerObj();

	lua_Number x = lua_tonumber(L, 1);
	lua_Number y = lua_tonumber(L, 2);
	lua_Number z = lua_tonumber(L, 3);

	player->scale.x = x;
	player->scale.y = y;
	player->scale.z = z;

	player->physSize.x = x;
	player->physSize.y = y;
	player->physSize.z = z;

	return 0;
}

static int setplayerorient(lua_State *L)
{
	lua_Number pitch = lua_tonumber(L,1);
    lua_Number yaw = lua_tonumber(L, 2);
    lua_Number roll = lua_tonumber(L, 3);
	
    g_engine2->setPlayerOrientation(pitch, yaw, roll);
	
	return 1;
}

static int getplayerorient(lua_State *L)
{
	Object *player = g_engine2->getPlayerObj();

	lua_pushnumber(L, player->pitch);
	lua_pushnumber(L, player->yaw);
	lua_pushnumber(L, player->roll);

	return 3;
}

static int setplayerpitch(lua_State *L)
{
    lua_Number pitch = lua_tonumber(L, 1);
	
    g_engine2->setPlayerPitch(pitch);
	
	return 0;
}

static int setplayeryaw(lua_State *L)
{
    lua_Number yaw = lua_tonumber(L, 1);
	
    g_engine2->setPlayerYaw(yaw);
	
	return 0;
}

static int setplayerroll(lua_State *L)
{
    lua_Number roll = lua_tonumber(L, 1);
	
    g_engine2->setPlayerRoll(roll);
	
	return 0;
}

static int getplayerpitch(lua_State *L)
{
	lua_pushnumber(L, g_engine2->getPlayerPitch());
	
	return 1;
}

static int getplayeryaw(lua_State *L)
{
    lua_pushnumber(L, g_engine2->getPlayerYaw());
	
	return 1;
}

static int getplayerroll(lua_State *L)
{
    lua_pushnumber(L, g_engine2->getPlayerRoll());
	
	return 1;
}

static int setplayermovespeed(lua_State *L)
{
	lua_Number speed = lua_tonumber(L, 1);

	g_engine2->setPlayerMoveSpeed(speed);

	return 0;
}

static int setcontrolscheme(lua_State *L)
{
	std::string schemeStr = lua_tostring(L, 1);
	
	ControlSchemes scheme;
	
    if (schemeStr == "fps")
	    scheme = CTRL_FPS;	
	if (schemeStr == "thirdperson")
		scheme = CTRL_THIRDPERSON;
	if (schemeStr == "ufoshooter")
	    scheme = CTRL_UFOSHOOTER;	
    if (schemeStr == "ufoshooter360")
	    scheme = CTRL_UFOSHOOTER_360;	
    if (schemeStr == "scrollingshooter")
	    scheme = CTRL_SCROLLINGSHOOTER;
    if (schemeStr == "scrollingshooterxy")
	    scheme = CTRL_SCROLLINGSHOOTERXY;	
    if (schemeStr == "editor")
	    scheme = CTRL_EDITOR;	
	
	g_engine2->setControlScheme(scheme);
	
	return 1;
}

static int setcamerascheme(lua_State *L)
{
	std::string schemeStr = lua_tostring(L, 1);
	
	CameraSchemes scheme;
	
	if (schemeStr == "editor")
		scheme = CAMERA_EDITOR;
	if (schemeStr == "fps")
	    scheme = CAMERA_FPS;	
	if (schemeStr == "thirdperson")
		scheme = CAMERA_THIRDPERSON;
	if (schemeStr == "ufoshooter")
	    scheme = CAMERA_UFOSHOOTER;	
    if (schemeStr == "ufoshooter360")
	    scheme = CAMERA_UFOSHOOTER_360;	
    if (schemeStr == "ufoshootervr")
	    scheme = CAMERA_UFOSHOOTER_VR;	
	
	g_engine2->setCameraScheme(scheme);
	
	return 1;
}

static int moveobjsz(lua_State *L)
{
	lua_Number fromz = lua_tonumber(L, 1);
    lua_Number fromradius = lua_tonumber(L,2);
    lua_Number toz = lua_tonumber(L, 3);
	
    g_engine2->moveObjectsZ(fromz, fromradius, toz);
	
	return 1;
}

static int moveobjsx(lua_State *L)
{
	lua_Number fromx = lua_tonumber(L, 1);
    lua_Number fromradius = lua_tonumber(L,2);
    lua_Number tox = lua_tonumber(L, 3);
	
    g_engine2->moveObjectsX(fromx, fromradius, tox);
	
	return 1;
}

static int playsound(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	bool stereo = lua_toboolean(L, 2);

	g_engine2->playSound(name, stereo);

	return 1;
}

static int playtrack(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	bool stereo = lua_toboolean(L, 2);

	g_engine2->playTrack(name, stereo);

	return 1;
}

static int checkcoll(lua_State *L)
{
	std::string name1 = lua_tostring(L, 1);
	std::string name2 = lua_tostring(L, 2);
	float factorx = lua_tonumber(L, 3);
	float factory = lua_tonumber(L, 4);
	float factorz = lua_tonumber(L, 5);

	bool result = g_engine2->checkCollision(g_engine2->findObj(name1), g_engine2->findObj(name2), factorx, factory, factorz);

	lua_pushboolean(L, result);

	return 1;
}

static int checkvoxcoll(lua_State *L)
{
	std::string name1 = lua_tostring(L, 1);
	float multx = lua_tonumber(L, 2);
	float multy = lua_tonumber(L, 3);
	float multz = lua_tonumber(L, 4);

	bool result = g_engine2->checkVoxelCollision(g_engine2->findObj(name1), multx, multy, multz);

	lua_pushboolean(L, result);

	return 1;
}

static int collray(lua_State *L)
{
	std::string name1 = lua_tostring(L, 1);

	Object *obj2 = g_engine2->collisionRay(g_engine2->findObj(name1));

	if (obj2 != nullptr)
		lua_pushstring(L, obj2->name.c_str());
	else
		lua_pushstring(L, "");

	return 1;
}

static int checksight(lua_State *L)
{
	std::string name1 = lua_tostring(L, 1);
    std::string name2 = lua_tostring(L, 2);
	
	bool result = g_engine2->checkSight(g_engine2->findObj(name1), g_engine2->findObj(name2));

	lua_pushboolean(L, result);
		
	return 1;
}

static int sethitpts(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number pts = lua_tonumber(L, 2);

	g_engine2->setHitPoints(name, pts);

	return 1;
}

static int gethitpts(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	Object *obj = g_engine2->findObj(name);

	lua_pushnumber(L, obj->hitPoints);

	return 1;
}

static int addtext(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string text = lua_tostring(L, 2);
	lua_Number x = lua_tonumber(L, 3);
	lua_Number y = lua_tonumber(L, 4);
	lua_Number size = lua_tonumber(L, 5);
	std::string onclick = lua_tostring(L,6);

	g_engine2->addText(name, text, x, y, size, onclick);

	return 1;
}

static int settext(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string text = lua_tostring(L, 2);

	g_engine2->setText(name, text);

	return 1;
}

static int settextvisible(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	bool visible = lua_toboolean(L, 2);

	g_engine2->setTextVisible(name, visible);

	return 1;
}

static int setcontrolsenabled(lua_State *L)
{
	bool enabled = lua_tonumber(L, 1);

	g_engine2->setControlsEnabled(enabled);

	return 1;
}

static int sethealthbar(lua_State *L)
{
	lua_Number which = lua_tonumber(L, 1);
	lua_Number value = lua_tonumber(L, 2);

	g_engine2->setHealthBar(which, value);

	return 1;
}

static int fadeglobalcolor(lua_State *L)
{
	lua_Number r = lua_tonumber(L, 1);
	lua_Number g = lua_tonumber(L, 2);
	lua_Number b = lua_tonumber(L, 3);
	lua_Number a = lua_tonumber(L, 4);
	lua_Number time = lua_tonumber(L, 5);

	g_engine2->fadeGlobalColor(glm::vec4(r, g, b, a), time);

	return 1;
}

static int setglobalcolor(lua_State *L)
{
	lua_Number r = lua_tonumber(L, 1);
	lua_Number g = lua_tonumber(L, 2);
	lua_Number b = lua_tonumber(L, 3);
	lua_Number a = lua_tonumber(L, 4);

	g_engine2->setGlobalColor(glm::vec4(r, g, b, a));

	return 1;
}

static int sethealthbarsvisible(lua_State *L)
{
	bool visible = lua_toboolean(L, 1);

	g_engine2->setHealthBarsVisible(visible);

	return 1;
}

static int setcontrolsvisible(lua_State *L)
{
	bool visible = lua_toboolean(L, 1);

	g_engine2->setControlsVisible(visible);

	return 1;
}

static int log(lua_State *L)
{
	std::string str = lua_tostring(L, 1);

	Log(str);

	return 1;
}

static int setextraint(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	int val = lua_tonumber(L, 2);

	g_engine2->setExtraInt(name, val);

	return 1;
}

static int getextraint(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	
	int val = g_engine2->getExtraInt(name);
	
	lua_pushnumber(L, val);

	return 1;
}

static int getextraint1(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	
	int val = g_engine2->getExtraInt(name);
	
	lua_pushnumber(L, 1);

	return 1;
}

static int setextrastr(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string val = lua_tostring(L, 2);

	g_engine2->setExtraStr(name, val);

	return 1;
}

static int getextrastr(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string val = g_engine2->getExtraStr(name);

	lua_pushstring(L, val.c_str());

	return 1;
}

static int setvox(lua_State *L)
{
	std::string shapeName = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);
	lua_Number y = lua_tonumber(L, 3);
	lua_Number z = lua_tonumber(L, 4);
	lua_Number tex = lua_tonumber(L, 5);

	g_engine2->setVoxel(shapeName, x, y, z, tex);

	return 1;
}

static int getvox(lua_State *L)
{
	std::string shapeName = lua_tostring(L, 1);
	lua_Number x = lua_tonumber(L, 2);
	lua_Number y = lua_tonumber(L, 3);
	lua_Number z = lua_tonumber(L, 4);

	char texture = g_engine2->getVoxel(shapeName, x, y, z);

	lua_pushnumber(L, texture);

	return 1;
}

static int setvoxtex(lua_State *L)
{
	std::string shape = lua_tostring(L, 1);
	lua_Number voxel = lua_tonumber(L, 2);
	std::string texture = lua_tostring(L, 3);

	g_engine2->setVoxelTexture(shape, voxel, texture.c_str());

	return 1;
}

static int getvoxtex(lua_State *L)
{
	std::string shape = lua_tostring(L, 1);
	lua_Number voxel = lua_tonumber(L, 2);

	std::string texture = g_engine2->getVoxelTexture(shape, voxel);

	lua_pushstring(L, texture.c_str());

	return 1;
}

static int loadvox(lua_State *L)
{
	std::string shapeName = lua_tostring(L, 1);
	std::string filename = lua_tostring(L, 2);

	g_engine2->loadVoxels(shapeName, filename);

	return 0;
}

static int rand(lua_State *L)
{
	lua_Number from = lua_tonumber(L, 1);
	lua_Number to = lua_tonumber(L, 2);

	float val = RandomFloat(from, to);

	lua_pushnumber(L, val);

	return 1;
}

static int randint(lua_State *L)
{
	lua_Number from = lua_tonumber(L, 1);
	lua_Number to = lua_tonumber(L, 2);

	float val = RandomInt(from, to);

	lua_pushnumber(L, val);

	return 1;
}

static int setdeltaxz(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number yaw = lua_tonumber(L, 2);
    lua_Number distance = lua_tonumber(L, 3);
	
	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
    obj->setDeltaXZ(yaw, distance);
	
	return 0;
}

static int move(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	
	Object *obj = g_engine2->findObj(name);
	
	if (obj == nullptr)
		return 0;
	
    obj->move();
	
	return 0;
}

static int getyawto(lua_State *L)
{
	std::string fromname = lua_tostring(L, 1);
	std::string toname = lua_tostring(L, 2);
	
	Object *fromobj = g_engine2->findObj(fromname);
	Object *toobj = g_engine2->findObj(toname);

	if (fromobj == nullptr || toobj == nullptr)
		return 0;

    float val = fromobj->getYawTo(toobj);
	lua_pushnumber(L, val);
	
	return 1;
}

static int getyawtopoint(lua_State *L)
{
	std::string fromname = lua_tostring(L, 1);
	float tox = lua_tonumber(L, 2);
	float toz = lua_tonumber(L, 3);

	Object *fromobj = g_engine2->findObj(fromname);
	Object toobj;

	if (fromobj == nullptr)
		return 0;

	toobj.position.x = tox;
	toobj.position.z = toz;

	float val = fromobj->getYawTo(&toobj);
	lua_pushnumber(L, val);

	return 1;
}

static int getpitchto(lua_State *L)
{
	std::string fromname = lua_tostring(L, 1);
	std::string toname = lua_tostring(L, 2);
	
	Object *fromobj = g_engine2->findObj(fromname);
	Object *toobj = g_engine2->findObj(toname);

    float val = fromobj->getPitchTo(toobj);
	lua_pushnumber(L, val);
	
	return 1;
}

static int distance(lua_State *L)
{
	std::string fromname = lua_tostring(L, 1);
	std::string toname = lua_tostring(L, 2);

	Object *fromobj = g_engine2->findObj(fromname);
	Object *toobj = g_engine2->findObj(toname);

	float val = fromobj->distanceTo(toobj);
	lua_pushnumber(L, val);

	return 1;
}

static int setplayerrangex(lua_State *L)
{
	lua_Number min = lua_tonumber(L, 1);
	lua_Number max = lua_tonumber(L, 2);

	Controls2 *controls = g_engine2->getControls();
	controls->setPlayerRangeX(min, max);

	return 0;
}

static int setplayerrangey(lua_State *L)
{
	lua_Number min = lua_tonumber(L, 1);
	lua_Number max = lua_tonumber(L, 2);

	Controls2 *controls = g_engine2->getControls();
	controls->setPlayerRangeY(min, max);

	return 0;
}

static int setsun(lua_State *L)
{
	lua_Number x = lua_tonumber(L, 1);
	lua_Number y = lua_tonumber(L, 2);
	lua_Number z = lua_tonumber(L, 3);
	lua_Number pitch = lua_tonumber(L, 4);
	lua_Number yaw = lua_tonumber(L, 5);
	lua_Number roll = lua_tonumber(L, 6);
	lua_Number size = lua_tonumber(L, 7);

	g_engine2->setSun(glm::vec4(x, y, z, 1.0), yaw, pitch, roll, size);

	return 1;
}

static int clear(lua_State *L)
{
	g_engine2->clear();

	return 0;
}

static int cleargui(lua_State *L)
{
	g_engine2->clearGUI();

	return 0;
}

static int addwg(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	std::string typeStr = lua_tostring(L, 2);
	std::string texture = lua_tostring(L, 3);
	std::string text = lua_tostring(L, 4);
	std::string onclickextra = lua_tostring(L, 5);
	std::string group = lua_tostring(L, 6);
	lua_Number x = lua_tonumber(L, 7);
    lua_Number y = lua_tonumber(L, 8);
	lua_Number sizex = lua_tonumber(L, 9);
    lua_Number sizey = lua_tonumber(L, 10);
	
	WidgetType type = WG_BTN;
	
	if (typeStr == "btn")
	    type = WG_BTN;	
    if (typeStr == "menu")
	    type = WG_MENU;	
    if (typeStr == "menuitem")
	    type = WG_MENUITEM;	
	
	g_engine2->addWg(name, type, texture, text, onclickextra, group, x, y, sizex, sizey);

	return 0;
}

static int setwgvisible(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	bool visible = lua_toboolean(L, 2);

	g_engine2->setWgVisible(name, visible);

	return 1;
}

static int setwgcolor(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	lua_Number r = lua_tonumber(L, 2);
	lua_Number g = lua_tonumber(L, 3);
	lua_Number b = lua_tonumber(L, 4);
	lua_Number a = lua_tonumber(L, 5);

	g_engine2->getGUI()->setWgColor(name, r, g, b, a);

	return 0;
}

static int runscript(lua_State *L)
{
	std::string filename = lua_tostring(L, 1);
	std::string errorMsg;
	
#ifdef PLATFORM_ANDROID
    std::string filesDir = g_externalFilesDir;
	std::string fullFilename = filesDir + "/" + filename;	
	if (luaL_dofile(L, fullFilename.c_str()))
	{
		errorMsg = lua_tostring(L, -1);
		Log(errorMsg);
		PLAT_ShowText(errorMsg);
	}
	else
    {
		errorMsg = "";
	}
#endif
#ifdef PLATFORM_WINDOWS
	char fullFilename[MAX_STR_LEN];
	printFullResourceFilename((char *)filename.c_str(), fullFilename);

	luaL_dofile(L, fullFilename);
#endif

	return 0;
}

static int batch(lua_State *L)
{
	std::string batchobj = lua_tostring(L, 1);
	std::string addobj = lua_tostring(L, 2);
	
	g_engine2->batch(batchobj, addobj);

	return 0;
}

static int autobatch(lua_State *L)
{
    g_engine2->autoBatch();

    return 0;
}

static int loadscene(lua_State *L)
{
	std::string name = lua_tostring(L, 1);

	g_engine2->loadScene(name);

	return 0;
}

static int setsecondaryyawmesh(lua_State *L)
{
	std::string name = lua_tostring(L, 1);
	int meshIdx = lua_tonumber(L, 2);

	Object *obj = g_engine2->getObject(name);

	obj->secondaryYawMesh = meshIdx;

	return 0;
}

static int getbtn(lua_State *L)
{
	int which = lua_tonumber(L, 1);

	Controls2 *ctrl = g_engine2->getControls();

	lua_pushnumber(L, ctrl->getBtn((BtnNames)which));

	return 1;
}

static int setbtn(lua_State *L)
{
	int which = lua_tonumber(L, 1);
	int value = lua_tonumber(L, 2);

	Controls2 *ctrl = g_engine2->getControls();

	ctrl->setBtn(which, value);

	return 0;
}

static int getmousebtn(lua_State *L)
{
	int which = lua_tonumber(L, 1);

	Controls2 *ctrl = g_engine2->getControls();

	lua_pushnumber(L, ctrl->getMouseBtn((BtnNames)which));

	return 1;
}

static int getallobjs(lua_State *L)
{
	// Table for Lua to know what each object is approximately
	lua_newtable(L);
	int top = lua_gettop(L);
	int idx = 1;

	g_engine2->refreshObjectCategories();

	std::map<std::string, Object *> objs = g_engine2->getObjects();

	for (const auto &pair : objs) {
		Object *obj = pair.second;

		if (obj == nullptr)
			continue;

		if (obj->system)
			continue;

		lua_pushstring(L, obj->name.c_str());
		lua_pushstring(L, obj->category.c_str());

		lua_settable(L, -3);

		idx++;
	}

	lua_setglobal(L, "allobjs");

	return 1;
}

static int setplayarea(lua_State *L)
{
	g_common.playAreaMinX = lua_tonumber(L, 1);
	g_common.playAreaMaxX = lua_tonumber(L, 2);
	g_common.playAreaMinY = lua_tonumber(L, 3);
	g_common.playAreaMaxY = lua_tonumber(L, 4);
	g_common.playAreaMinZ = lua_tonumber(L, 5);
	g_common.playAreaMaxZ = lua_tonumber(L, 6);

	return 0;
}

static int addtouchbtnbind(lua_State *L)
{
	int btn = lua_tonumber(L, 1);
	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);
	float size = lua_tonumber(L, 4);
	
	Controls2 *ctrl = g_engine2->getControls();
	
    ctrl->addTouchBtnBind(btn, x, y, size);
	
	return 0;
}

static int rotateangletowards(lua_State *L) 
{
	float angle = lua_tonumber(L, 1);
	float targetAngle = lua_tonumber(L, 2);
	float step = lua_tonumber(L, 3);

	float val = RotateAngleTowards(angle, targetAngle, step);

	lua_pushnumber(L, val);

	return 1;
}

static int compareyaw(lua_State *L)
{
	float yaw = lua_tonumber(L, 1);
	float otherYaw = lua_tonumber(L, 2);
	float tolerance = lua_tonumber(L, 3);

	bool val = compareYaw(yaw, otherYaw, tolerance);
	lua_pushboolean(L, val);

	return 1;
}

static int loadtex(lua_State *L)
{
	std::string tex = lua_tostring(L, 1);
	
	TextureManager2 *texMan = g_engine2->getTextureManager();
	texMan->find(tex);

	return 0;
}

static int rebuild(lua_State *L)
{
	std::string shname = lua_tostring(L, 1);
	
    Shape *shape = g_engine2->findShape(shname);
	
	if (shape == nullptr)
		return 0;
		
	shape->needsRebuild = true;

	return 0;
}

void LuaBridge::init(Engine2 *engine)
{
    this->engine = engine;
	
	L = luaL_newstate();
	luaL_openlibs(L);
	
	luaopen_base(L);
	luaopen_table(L);
	luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);
	
	lua_register(
			L,               /* Lua state variable */
			"square",        /* func name as known in Lua */
			isquare          /* func name in this file */
			);
			
	lua_register(
			L,               /* Lua state variable */
			"square2",        /* func name as known in Lua */
			isquare2          /* func name in this file */
			);
			
	lua_register(L, "addobj", addobj);
	lua_register(L, "remobj", remobj);	
	lua_register(L, "rename", rename);
    lua_register(L, "settype", settype);
	lua_register(L, "setshape", setshape);
	lua_register(L, "setmodel", setmodel);	
	lua_register(L, "getmodel", getmodel);
	lua_register(L, "setmodelorient", setmodelorient);
	lua_register(L, "setcolor", setcolor);
	lua_register(L, "setmeshcolor", setmeshcolor);
	lua_register(L, "settex", settex);	
	lua_register(L, "gettex", gettex);
	lua_register(L, "settexspan", settexspan);
	lua_register(L, "setvisible", setvisible);	
	lua_register(L, "getvisible", getvisible);	
	lua_register(L, "setsize", setsize);
	lua_register(L, "getsize", getsize);
	lua_register(L, "setphyssize", setphyssize);
	lua_register(L, "setpos", setpos);
	lua_register(L, "getpos", getpos);
	lua_register(L, "setx", setx);
	lua_register(L, "getx", getx);
	lua_register(L, "sety", sety);
	lua_register(L, "gety", gety);
	lua_register(L, "setz", setz);
	lua_register(L, "getz", getz);
	lua_register(L, "setmaxx", setx);
	lua_register(L, "setminx", setx);
	lua_register(L, "setvelocity", setvelocity);
	lua_register(L, "getvelocity", getvelocity);
	lua_register(L, "setorient", setorient);
	lua_register(L, "getorient", getorient);
	lua_register(L, "setyaw", setyaw);	
	lua_register(L, "setpitch", setpitch);
    lua_register(L, "setroll", setroll);
	lua_register(L, "setsecondaryyaw", setsecondaryyaw);
	lua_register(L, "getyaw", getyaw);
	lua_register(L, "getpitch", getpitch);
    lua_register(L, "getroll", getroll);
	lua_register(L, "getyaw", getyaw);
	lua_register(L, "getsecondaryyaw", getsecondaryyaw);
	lua_register(L, "moveforward", moveforward);
	lua_register(L, "setmovesmoothly", setmovesmoothly);
	lua_register(L, "setfade", setfade);
	lua_register(L, "setfaceplayer", setfaceplayer);
	lua_register(L, "setobjstr", setobjstr);
	lua_register(L, "getobjstr", getobjstr);
	lua_register(L, "setobjint", setobjint);
	lua_register(L, "getobjint", getobjint);
	lua_register(L, "newshape", newshape);
	lua_register(L, "delshape", delshape);
	lua_register(L, "loadmodel", loadmodel);
	lua_register(L, "setskybox", setskybox);
	lua_register(L, "setcampos", setcampos);
	lua_register(L, "setcamorient", setcamorient);	
	lua_register(L, "setplayerobj", setplayerobj);
	lua_register(L, "setplayerpos", setplayerpos);
	lua_register(L, "getplayerpos", getplayerpos);
	lua_register(L, "setplayersize", setplayersize);
	lua_register(L, "setplayerorient", setplayerorient);
	lua_register(L, "getplayerorient", getplayerorient);
	lua_register(L, "setplayerpitch", setplayerpitch);
	lua_register(L, "setplayeryaw", setplayeryaw);
	lua_register(L, "setplayerroll", setplayerroll);
	lua_register(L, "getplayerpitch", getplayerpitch);
	lua_register(L, "getplayeryaw", getplayeryaw);
	lua_register(L, "getplayerroll", getplayerroll);
	lua_register(L, "setplayermovespeed", setplayermovespeed);
	lua_register(L, "setcontrolscheme", setcontrolscheme);
	lua_register(L, "setcamerascheme", setcamerascheme);
	lua_register(L, "moveobjsz", moveobjsz);	
	lua_register(L, "moveobjsx", moveobjsx);
	lua_register(L, "playsound", playsound);
	lua_register(L, "playtrack", playtrack);
	lua_register(L, "checkcoll", checkcoll);
	lua_register(L, "checkvoxcoll", checkvoxcoll);
	lua_register(L, "collray", collray);
	lua_register(L, "checksight", checksight);
	lua_register(L, "sethitpts", sethitpts);
    lua_register(L, "gethitpts", gethitpts);
	lua_register(L, "addtext", addtext);
	lua_register(L, "settext", settext);
	lua_register(L, "settextvisible", settextvisible);
	lua_register(L, "setcontrolsenabled", setcontrolsenabled);
	lua_register(L, "sethealthbar", sethealthbar);
	lua_register(L, "fadeglobalcolor", fadeglobalcolor);
	lua_register(L, "setglobalcolor", setglobalcolor);
	lua_register(L, "sethealthbarsvisible", sethealthbarsvisible);
	lua_register(L, "setcontrolsvisible", setcontrolsvisible);
	lua_register(L, "setextraint", setextraint);
	lua_register(L, "getextraint", getextraint);
	lua_register(L, "setextrastr", setextrastr);
	lua_register(L, "getextrastr", getextrastr);
	lua_register(L, "setextrafloat", setextraint);
	lua_register(L, "getextrafloat", getextraint);
	lua_register(L, "log", log);
	lua_register(L, "setvox", setvox);
	lua_register(L, "getvox", getvox);
	lua_register(L, "setvoxtex", setvoxtex);
	lua_register(L, "getvoxtex", getvoxtex);
	lua_register(L, "loadvox", loadvox);
	lua_register(L, "rand", rand);
	lua_register(L, "randint", randint);
	lua_register(L, "setdeltaxz", rand);
	lua_register(L, "move", rand);
	lua_register(L, "getyawto", getyawto);
	lua_register(L, "getyawtopoint", getyawtopoint);
	lua_register(L, "getpitchto", getpitchto);
	lua_register(L, "distance", distance);
	lua_register(L, "setplayerrangex", setplayerrangex);
	lua_register(L, "setplayerrangey", setplayerrangey);
	lua_register(L, "setsun", setsun);
	lua_register(L, "clear", clear);
	lua_register(L, "cleargui", cleargui);
	lua_register(L, "addwg", addwg);
	lua_register(L, "setwgvisible", setwgvisible);
	lua_register(L, "setwgcolor", setwgcolor);
	lua_register(L, "runscript", runscript);
	lua_register(L, "batch", batch);
    lua_register(L, "autobatch", autobatch);
	lua_register(L, "loadscene", loadscene);
	lua_register(L, "setsecondaryyawmesh", setsecondaryyawmesh);
	lua_register(L, "getbtn", getbtn);
	lua_register(L, "setbtn", setbtn);
	lua_register(L, "getmousebtn", getmousebtn);
	lua_register(L, "getallobjs", getallobjs);
	lua_register(L, "setplayarea", setplayarea);
	lua_register(L, "addtouchbtnbind", addtouchbtnbind);
	lua_register(L, "rotateangletowards", rotateangletowards);
	lua_register(L, "compareyaw", compareyaw);
	lua_register(L, "loadtex", loadtex);
	lua_register(L, "rebuild", rebuild);
}

void LuaBridge::exec(std::string filename)
{
	// Error? Don't run anything
	if (errorMsg != "")
		return;
	
#ifdef PLATFORM_IOS
    if (filename == "")
        return;
    
    int dotPos = filename.find(".");
    
    std::string nameNoExt = "";
    std::string ext = "";
    
    if (dotPos != -1)
    {
        nameNoExt = filename.substr(0, dotPos);
        ext = filename.substr(dotPos, filename.length());
    }
    else
    {
        return;
    }
    
    NSString *nsNameNoExt = [NSString stringWithFormat:@"%s", nameNoExt.c_str()];
    NSString *nsExt = [NSString stringWithFormat:@"%s", ext.c_str()];
    
    GLKTextureInfo *spriteTexture;
    NSError *theError;
    
    NSString *filePath = [[NSBundle mainBundle] pathForResource:nsNameNoExt ofType:nsExt];
//    NSURL *url = [NSURL fileURLWithPath:path];
    
    std::string stdFilePath = std::string([filePath UTF8String]);
    
    std::string fullFilename = stdFilePath;
    if (luaL_dofile(L, fullFilename.c_str()))
    {
        errorMsg = lua_tostring(L, -1);
        Log(errorMsg);
        PLAT_ShowText(errorMsg);
    }
    else
    {
        errorMsg = "";
    }
#endif
    
#ifdef PLATFORM_ANDROID
	std::string fullFilename = g_assetsDir + "/" + filename;	
	if (luaL_dofile(L, fullFilename.c_str()))
	{
		errorMsg = lua_tostring(L, -1);
		Log(errorMsg);
		PLAT_ShowText(errorMsg);
	}
	else
    {
		errorMsg = "";
	}
#endif
#ifdef PLATFORM_WINDOWS
	char fullFilename[MAX_STR_LEN];
	printFullResourceFilename((char *)filename.c_str(), fullFilename);

	if (luaL_dofile(L, fullFilename))
	{
		errorMsg = lua_tostring(L, -1);
		Log(errorMsg);
		PLAT_ShowText(errorMsg);
	}
	else
	{
		errorMsg = "";
	}
#endif
}
