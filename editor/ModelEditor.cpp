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
OUeT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "ModelEditor.h"
#include "../engine/SoftCanvas.h"
#include "../thirdparty/lodepng/lodepng.hpp"

void ModelEditor::init()
{
	g_engine2->init();
    this->engine = g_engine2;
	engine->clear();
}

void ModelEditor::load()
{
    // Button dimensions
    int numbtns = 6;
    float bw = 0.3;
    float hbw = 0.15;
    float bs =  0.25;
    float bsv = 0.25;
    float hw = 1.0 / numbtns;

    // Top buttons
	engine->addWg("filebtn", WG_BTN, "file.png", "", "filebtnclicked", "", -hw * 5, 0.8, hw, hw);
//	engine->addWg("modebtn", WG_BTN, "m.png", "", "modebtnclicked", "", -hw * 3, 0.8, hw, hw);
//	engine->addWg("colorbtn", WG_BTN, "palette.png", "", "colorbtnclicked", "", hw * 3, 0.8, hw, hw);
    engine->addWg("backbtn", WG_BTN, "redx.png", "", "backbtnclicked", "", hw * 5, 0.8, hw, hw);

    // Bottom buttons
    engine->addWg("xybtn", WG_BTN, "xy.png", "", "xybtnclicked", "", -hw * 5.25, -0.8, hw * 0.7, hw * 0.7);
    engine->addWg("zybtn", WG_BTN, "zy.png", "", "zybtnclicked", "", -hw * 3.50, -0.8, hw * 0.7, hw * 0.7);
    engine->addWg("xzbtn", WG_BTN, "xz.png", "", "xzbtnclicked", "", hw * -1.75, -0.8, hw * 0.7, hw * 0.7);
//    engine->addWg("down", WG_BTN, "downarrow.png", "", "downbtnclicked", "", hw * 0.0, -0.8, hw, hw);
//    engine->addWg("up", WG_BTN, "uparrow.png", "", "upbtnclicked", "", hw * 1.75, -0.8, hw, hw);
//    engine->addWg("prev", WG_BTN, "prevkit.png", "", "prevbtnclicked", "", hw * 3.50, -0.8, hw, hw);
//    engine->addWg("next", WG_BTN, "nextkit.png", "", "nextbtnclicked", "", hw * 5.25, -0.8, hw, hw);

	engine->setPhysicsEnabled(false);
	engine->setControlScheme(CTRL_CUSTOM);
	
	// Default voxels
	clear();
		
	engine->setPlayerPos(0.0, 0.0, 0.0);
	engine->setPlayerOrientation(0.0, 0.0, 0.0);
		
//	voxels.init(16, nullptr);
	create();
	setDefaultTextures();
	refresh();

    engine->addText("msg", "", 0.0, 0.6, 0.08);
    engine->addText("msg2", "", 0.0, 0.5, 0.08);
}

void ModelEditor::tick()
{
    static int msgTimer = 0;
    static const int msgTimerDelay = 50;

    if (needsRefresh)
    {
        refresh();
        needsRefresh = false;
    }

    engine->setPlayerPos(0.0, 0.0, 0.0);
    engine->setPlayerOrientation(0.0, 0.0, 0.0);

	GUI *gui = g_engine2->getGUI();
	
	screenWidth = PLAT_GetWindowWidth();
    screenHeight = PLAT_GetWindowHeight();
	
	if (btnTimer > 0)
		btnTimer--;
	
	if (btnTimer == 0 && (engine->getExtraInt("backbtnclicked") == 1
        || engine->getExtraInt("backbtnclicked") == 3)) {

	    if (!modified) {
            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "editor");
        } else {
            gui->clearListMenu();
            gui->addListMenuOption("Discard and Exit", "");
			gui->addListMenuOption("Cancel", "");
            gui->showListMenuInDialog("Voxels not saved!", "");
	    }
				
        engine->setExtraInt("backbtnclicked", 0);
		btnTimer = 30;
    }

    if (engine->getExtraStr("listmenuoptionclicked") == "Discard and Exit")
    {
        engine->setExtraInt("switchmodule", 1);
        engine->setExtraStr("nextmodule", "editor");

        engine->setExtraStr("listmenuoptionclicked", "");
    }

	if (btnTimer == 0 && (engine->getExtraInt("filebtnclicked") == 1
        || engine->getExtraInt("filebtnclicked") == 1)) {
		gui->clearListMenu();
		gui->addListMenuOption("New Mesh", "");
		gui->addListMenuOption("Load Mesh", "");
		gui->addListMenuOption("Save Mesh", "");
		gui->showListMenuInDialog("File", "");
		
		engine->setExtraInt("filebtnclicked", 0);
		btnTimer = 30;
    }
	
	if (btnTimer == 0 && (engine->getExtraInt("modebtnclicked") == 1
        || engine->getExtraInt("modebtnclicked") == 1)) {
			
/*		if (mode == MODE_VOXELS)
		{
			mode = MODE_PIXELS;
			refresh();

            engine->setText("msg", "pixel mode");
            msgTimer = msgTimerDelay;
		}
		else if (mode == MODE_PIXELS)
		{
			mode = MODE_VOXELS;
			refresh();

            engine->setText("msg", "voxel mode");
            msgTimer = msgTimerDelay;
		}*/
		
		engine->setExtraInt("modebtnclicked", 0);
		btnTimer = 30;
    }
	
	if (engine->getExtraStr("listmenuoptionclicked") == "New Mesh")
    {
		clear();
		
		engine->setPlayerPos(0.0, 0.0, 0.0);
	    engine->setPlayerOrientation(0.0, 0.0, 0.0);
		
		mesh.clear();
		create();
		setDefaultTextures();
		refresh();

        engine->setExtraStr("listmenuoptionclicked", "");
    }
		
	if (engine->getExtraStr("listmenuoptionclicked") == "Load Mesh")
    {
		std::string fname = PLAT_LoadPref("main", "mesh", "");
		if (fname == "")
            fname = g_assetsDir;
		
        gui->showFileSelector("ms", fname);
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "loadmesh";
    }
	
	if (engine->getExtraStr("listmenuoptionclicked") == "Save Mesh")
    {
		std::string fname = PLAT_LoadPref("main", "mesh", "");
		if (fname == "")
            fname = g_assetsDir;
		
        gui->showFileSelector("ms", fname);
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "savemesh";
    }

    if (engine->getExtraStr("listmenuoptionclicked") == "Export PNG")
    {
        std::string fname = PLAT_LoadPref("main", "png", "");
        if (fname == "")
            fname = g_assetsDir;

        gui->showFileSelector("png", fname);
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "exportpng";
    }
	
	if (engine->getExtraStr("listmenuoptionclicked") == "Set Texture")
    {
//        std::string fname = PLAT_LoadPref("main", "png", "");
//        if (fname == "")
//            fname = g_assetsDir;

        gui->showFileSelector("png", "");
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "settexture";
    }

    // File selector actions

    if (engine->getExtraStr("fileselected") != "") {
 
        if (fileSelectorAction == "loadmesh") {
            PLAT_SavePref("main", "mesh", engine->getExtraStr("fileselected"));
			loadMesh(engine->getExtraStr("fileselected"));

            char msg[1024];
            snprintf(msg, 1024, "loaded %s", GetFileName(engine->getExtraStr("fileselected")).c_str());
            engine->setText("msg", msg);
            msgTimer = msgTimerDelay;
		}
		
		if (fileSelectorAction == "savemesh") {
            PLAT_SavePref("main", "mesh", engine->getExtraStr("fileselected"));
			saveMesh(engine->getExtraStr("fileselected"));

            char msg[1024];
            snprintf(msg, 1024, "saved %s", GetFileName(engine->getExtraStr("fileselected")).c_str());
            engine->setText("msg", msg);
            msgTimer = msgTimerDelay;
		}

        if (fileSelectorAction == "exportpng") {
            PLAT_SavePref("main", "png", engine->getExtraStr("fileselected"));
            exportPNG(engine->getExtraStr("fileselected"));

            char msg[1024];
            snprintf(msg, 1024, "saved %s", GetFileName(engine->getExtraStr("fileselected")).c_str());
            engine->setText("msg", msg);
            msgTimer = msgTimerDelay;
        }
		
		if (fileSelectorAction == "settexture") {
			std::string fname = GetFileName(engine->getExtraStr("fileselected"));
			g_assetsDir = GetPath(engine->getExtraStr("fileselected"));
//			voxels.setVoxelTexture(texture, fname);
			needsRefresh = true;
		}

		engine->setExtraStr("fileselected", "");
	}
	
	// Color select button
	
	if (btnTimer == 0 && (engine->getExtraInt("colorbtnclicked") == 1
	    || engine->getExtraInt("colorbtnclicked") == 3))
	{
		gui->clearDialog();
		gui->addDialogPart("r", FloatToStr(colorr), "colorr");
        gui->addDialogPart("g", FloatToStr(colorg), "colorg");
		gui->addDialogPart("b", FloatToStr(colorb), "colorb");
		gui->addDialogPart("a", FloatToStr(colora), "colora");
		gui->showDialog("Set Color", "OK", "Cancel", "colorselected");
		
		engine->setExtraInt("colorbtnclicked", 0);
		btnTimer = 30;
    }
	
	if (engine->getExtraInt("colorselected") == 1)
	{
		colorr = PLAT_stof(engine->getExtraStr("colorr"), 1.0f);
        colorg = PLAT_stof(engine->getExtraStr("colorg"), 1.0f);
        colorb = PLAT_stof(engine->getExtraStr("colorb"), 1.0f);
        colora = PLAT_stof(engine->getExtraStr("colora"), 1.0f);
    }
	
	// Bottom button actions
	
	if (btnTimer == 0 && (engine->getExtraInt("xybtnclicked") == 1
        || engine->getExtraInt("xybtnclicked") == 1)) {

        view = VIEW_XY;			
		refresh();
		
		engine->setText("msg", "xy view");
        msgTimer = msgTimerDelay;
			
        engine->setExtraInt("xybtnclicked", 0);
		btnTimer = 5;
    }
	
	if (btnTimer == 0 && (engine->getExtraInt("zybtnclicked") == 1
        || engine->getExtraInt("zybtnclicked") == 1)) {

        view = VIEW_ZY;			
		refresh();
		
		engine->setText("msg", "zy view");
        msgTimer = msgTimerDelay;
			
        engine->setExtraInt("zybtnclicked", 0);
		btnTimer = 5;
    }

	if (btnTimer == 0 && (engine->getExtraInt("xzbtnclicked") == 1
        || engine->getExtraInt("xzbtnclicked") == 1)) {

        view = VIEW_XZ;			
		refresh();
		
		engine->setText("msg", "xz view");
        msgTimer = msgTimerDelay;
			
        engine->setExtraInt("xzbtnclicked", 0);
		btnTimer = 5;
    }
	
    if (btnTimer == 0 && (engine->getExtraInt("downbtnclicked") == 1
        || engine->getExtraInt("downbtnclicked") == 1)) {

        if (level > 0)
			level--;

        char msg[1024];
        snprintf(msg, 1024, "layer %d", level);
        engine->setText("msg", msg);
        msgTimer = msgTimerDelay;
			
		refresh();
			
        engine->setExtraInt("downbtnclicked", 0);
		btnTimer = 5;
    }
	
    if (btnTimer == 0 && (engine->getExtraInt("upbtnclicked") == 1
        || engine->getExtraInt("upbtnclicked") == 1)) {

//        if (level < voxels.getSize() - 1)
//			level++;
			
        char msg[1024];
        snprintf(msg, 1024, "layer %d", level);
        engine->setText("msg", msg);
        msgTimer = msgTimerDelay;
			
		refresh();
			
        engine->setExtraInt("upbtnclicked", 0);
		btnTimer = 5;
    }
	
	if (btnTimer == 0 && (engine->getExtraInt("prevbtnclicked") == 1
        || engine->getExtraInt("prevbtnclicked") == 1)) {

        if (texture > 0)
			texture--;

        char msg[1024];
        snprintf(msg, 1024, "texture %d", texture);
        engine->setText("msg", msg);
        msgTimer = msgTimerDelay;
			
		refresh();
			
        engine->setExtraInt("prevbtnclicked", 0);
		btnTimer = 5;
    }
	
    if (btnTimer == 0 && (engine->getExtraInt("nextbtnclicked") == 1
        || engine->getExtraInt("nextbtnclicked") == 1)) {

        if (texture < 255)
			texture++;

        char msg[1024];
        snprintf(msg, 1024, "texture %d", texture);
        engine->setText("msg", msg);
        msgTimer = msgTimerDelay;

        refresh();
			
        engine->setExtraInt("nextbtnclicked", 0);
		btnTimer = 5;
    }

    // Message indicator

    msgTimer--;

    if (msgTimer <= 0) {
        engine->setText("msg", "");
    }
}

void ModelEditor::loadMesh(std::string filename)
{
	clear();
	
	engine->setPlayerPos(0.0, 0.0, 0.0);
	engine->setPlayerOrientation(0.0, 0.0, 0.0);
	
	g_assetsDir = GetPath(filename);

	mesh.load(filename, nullptr);
	create();
    refresh();
}

void ModelEditor::saveMesh(std::string filename)
{
	mesh.save(filename, nullptr);
	modified = false;
}

void ModelEditor::clear()
{
	mesh.clear();

    modified = false;
}

void ModelEditor::create()
{
	for (int v = 0; v < maxVertices; v++)
	{
	    char newNamec[1024];
        snprintf(newNamec, 1024, "v%d", v);
        std::string name = std::string(newNamec);
			
	    engine->addObject(name);
		engine->setType(name, OBJTYPE_SPRITE);
	}
}

void ModelEditor::setDefaultTextures()
{
}

void ModelEditor::refresh()
{
//	Log(mesh.vertices.size());
	
	for (int v = 0; v < mesh.vertices.size(); v++)
    {
		char namec[1024];
        snprintf(namec, 1024, "v%d", v);
        std::string name = std::string(namec);

		Object *verto = engine->findObj(name);
		
		if (verto != nullptr)
		{
			glm::vec3 vert = mesh.vertices[v];
			float b = 1.0;
			
			switch (view)
			{
			case VIEW_XY:
			    verto->position.x = vert.x;
			    verto->position.y = vert.y;
				b = -vert.z / 1.0;
				break;
			case VIEW_ZY:
			    verto->position.x = vert.z;
			    verto->position.y = vert.y;
				b = -vert.x / 1.0;
				break;
			case VIEW_XZ:
			    verto->position.x = vert.x;
			    verto->position.y = -vert.z;
				b = vert.y / 1.0;
				break;
			}
			
			verto->position.z = -5;
			
			engine->setVisible(name, true);
			engine->setTexture(name, "redx.png");
			engine->setSize(name, 0.1, 0.1, 0.1);
			
			engine->setColor(name, 1.0 + b, 1.0 + b, 1.0 + b, 1.0);
		}
	}
	
	for (int v = mesh.vertices.size(); v < maxVertices; v++)
    {
		char namec[1024];
        snprintf(namec, 1024, "v%d", v);
        std::string name = std::string(namec);

		engine->setVisible(name, false);
	}
}

void ModelEditor::touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2, int actionIndex)
{
	float aspect = (float)screenWidth / (float)screenHeight;
	
	float glx = scrToGlX(x1);
	float gly = scrToGlY(y1) / aspect;
		
	if (gly < -1.0 || gly > 1.0
	    || glx < -1.0 || glx > 1.0)
	{
		if (dragVert)
		{
			dragVert = false;
			mesh.vertices.erase(mesh.vertices.begin() + dragVertIdx);
			modified = true;
            needsRefresh = true;
		}
			
		return;
	}
		
//	Log("action", action1);
		
	// See if we hit any existing vertices
	
	if (action1 == 1)
	{
	    for (int v = 0; v < mesh.vertices.size(); v++)
	    {
		    glm::vec3 vert = mesh.vertices[v];
		
		    float deltax = 0.0;
		    float deltay = 0.0;
			
			switch (view)
			{
			case VIEW_XY:
		        deltax = glx - vert.x;
		        deltay = gly - vert.y;
				break;
			case VIEW_ZY:
		        deltax = glx - vert.z;
		        deltay = gly - vert.y;
				break;
			case VIEW_XZ:
		        deltax = glx - vert.x;
		        deltay = gly + vert.z;
				break;
			}

			if (deltax < 0.0)
				deltax = -deltax;
			if (deltay < 0.0)
				deltay = -deltay;

		    if (deltax < dragThresh && deltay < dragThresh)
			{
				dragVert = true;
				dragVertIdx = v;
				
/*				Log("start drag v", v);
				Log("deltax", (int)(deltax * 100));
				Log("deltay", (int)(deltay * 100));
				Log("glx", (int)(glx * 100));
				Log("gly", (int)(gly * 100));
				Log("vert.x", (int)(vert.x * 100));
				Log("vert.y", (int)(vert.y * 100));*/
			}
	    }
	}
	
	// New vertex
	
//	if (newVertTimer > 0)
//		newVertTimer--;
		
	if (!dragVert && action1 == 1 && newVertTimer <= 0)
	{
	    glm::vec3 v;
		
		switch (view)
		{
		case VIEW_XY:
	        v.x = glx;
	        v.y = gly;
	        v.z = 0.0;
		    break;
		case VIEW_ZY:
	        v.z = glx;
	        v.y = gly;
	        v.x = 0.0;
			break;
		case VIEW_XZ:
	        v.x = glx;
	        v.z = -gly;
	        v.y = 0.0;
            break;
		}

	    mesh.vertices.push_back(v);
		
//		newVertTimer = newVertTimerDelay;
		
//		Log("new");
	}

	// Dragging vertex
	
	if (action1 == 3)
	{
		if (dragVert)
		{
			switch (view)
			{
			case VIEW_XY:
			    mesh.vertices[dragVertIdx].x = glx;
			    mesh.vertices[dragVertIdx].y = gly;		
				break;
			case VIEW_ZY:
			    mesh.vertices[dragVertIdx].z = glx;
			    mesh.vertices[dragVertIdx].y = gly;		
				break;
			case VIEW_XZ:
			    mesh.vertices[dragVertIdx].x = glx;
			    mesh.vertices[dragVertIdx].z = -gly;		
				break;
			}
		}
	}
	
	if (action1 == 4)
	{
		if (dragVert)
		{
			dragVert = false;
		}
	}
	
	modified = true;
    needsRefresh = true;
}

float ModelEditor::scrToGlX(float screenX) {
    return 2.0f * screenX / screenWidth - 1.0f;
}

float ModelEditor::scrToGlY(float screenY) {
    return -(2.0f * screenY / screenHeight - 1.0f);
}

float ModelEditor::glToScrX(float glX) {
    return ((glX + 1.0f) / 2.0f) * screenWidth;
}

float ModelEditor::glToScrY(float glY) {
    return screenHeight - abs(((glY + 1.0f) / 2.0f) * screenHeight);
}

void ModelEditor::exportPNG(std::string filename)
{
    SoftCanvas sc;
/*    sc.init(voxels.getSize(), voxels.getSize());

    for (int z = 0; z < voxels.getSize(); z++) {
        for (int x = 0; x < voxels.getSize(); x++) {
            int y = 0;

            if (voxels.get(x, y, z))
            {
                unsigned char ur, ug, ub, ua;
                voxels.getrgba(x, y, z, ur, ug, ub, ua);
                sc.setPx(x, z, ur, ug, ub, ua);
            }
        }
    }

    std::vector<unsigned char> image(sc.getData(), sc.getData() + sc.getWidth() * sc.getHeight() * 4);
    std::vector<unsigned char> png;

    unsigned error = lodepng::encode(png, image, sc.getWidth(), sc.getHeight());
    if(!error) lodepng::save_file(png, filename);*/
}
