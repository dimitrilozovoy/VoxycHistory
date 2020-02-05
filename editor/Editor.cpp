/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restrictibbj*v4j i44on, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyojright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
j
THE SOFTWARE IS PROVIDED " AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE O R THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <sstream>
#include <string>
#include <locale>
#include <algorithm>
#include "../engine/Globals.hpp"
#include "Editor.h"
#include "../engine/Engine2.h"
#include "../engine/DDLUtils.hpp"
#include "../engine/platform.h"
#include "../engine/TextureManager2.h"

#ifdef PLATFORM_ANDROID
#ifdef GAME_RELEASE_DIR_TREE
#include "../android.h"
#else
#include "../platform/android/Voxyc/app/src/main/cpp/android.h"
#endif
#endif

/*
========================================
init
========================================
*/

void Editor::init() {

    g_simpleMode = false;
//    g_assetsDirExplicit = false;

#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
	touchControls = true;
#else
	touchControls = false;
#endif

    g_engine2->init();
    this->engine = g_engine2;

    luaBridge.init(g_engine2);

    engine->setControlsVisible(false);
    engine->setControlScheme(CTRL_EDITOR);
    engine->setHealthBarsVisible(false);
//    engine->setSkybox("black.png");

	GUI *gui = g_engine2->getGUI();

#if defined PLATFORM_ANDROID || defined PLATFORM_IOS
	gui->setNativeWidgets(true);
#else
	gui->setNativeWidgets(false);
#endif
    
    // Preload font
    TextureManager2 *tm = engine->getTextureManager();
    tm->load("0.png", false);
    tm->load("1.png", false);
    tm->load("2.png", false);
    tm->load("3.png", false);
    tm->load("4.png", false);
    tm->load("5.png", false);
    tm->load("6.png", false);
    tm->load("7.png", false);
    tm->load("8.png", false);
    tm->load("9.png", false);
    tm->load("0.png", false);
    tm->load("a.png", false);
    tm->load("b.png", false);
    tm->load("c.png", false);
    tm->load("d.png", false);
    tm->load("e.png", false);
    tm->load("f.png", false);
    tm->load("g.png", false);
    tm->load("h.png", false);
    tm->load("i.png", false);
    tm->load("j.png", false);
    tm->load("k.png", false);
    tm->load("l.png", false);
    tm->load("m.png", false);
    tm->load("n.png", false);
    tm->load("o.png", false);
    tm->load("p.png", false);
    tm->load("q.png", false);
    tm->load("r.png", false);
    tm->load("s.png", false);
    tm->load("t.png", false);
    tm->load("u.png", false);
    tm->load("v.png", false);
    tm->load("w.png", false);
    tm->load("x.png", false);
    tm->load("y.png", false);
    tm->load("z.png", false);

    g_engine2->draw(0);
}

/*
========================================
load
========================================
*/

void Editor::load() {

    // Reset this in case we are coming back to an old instance
    // or resetting state
    selectedObj = nullptr;
    lastSelectedObj = nullptr;
    lastLitUpGuide = nullptr;
    curVoxels = nullptr;

	g_common.showMouse = true;

	// Turn off collision detection for player
    Object *player = engine->getPlayerObj();
    player->ints["ignorecollisions"] = 1;
    engine->setPlayerMoveSpeed(0.1);

    // Make main grid sprite
    engine->genTexture("maingrid", "grid", 32);

    // Create main grid obj
    std::string name = "grid";
    engine->addObject(name);
    engine->setType(name, OBJTYPE_SPRITE);
    engine->setTexture(name, "maingrid");
    engine->setPos(name, 0, 0, 0);
    engine->setSize(name, 100, 100, 100);
    engine->setOrientation(name, 90.0, 0.0, 0.0);
    engine->setColor(name, 0.5, 0.5, 0.5, 1.0);
    engine->setVisible(name, 1);
    engine->setSystem(name, true);

    engine->setPlayerPos(0.0, 2.0, 0.0);

    // Object preview
    name = "objpreview";
    engine->addObject(name);
    engine->setType(name, OBJTYPE_SPRITE);
    engine->setTexture(name, "");
    engine->setPos(name, -10, 0, -100);
    engine->setSize(name, 1, 1, 1);
    engine->setColor(name, 1.0, 1.0, 1.0, 1.0);
    engine->setVisible(name, 1);
    engine->setAlwaysFacePlayer(name, true);
    engine->setSystem(name, true);

    // Create voxels preview
    engine->newShape("voxpreview", SHAPE_VOXELS, 8, 0.0);
    engine->addObject("voxpreview");
    engine->setType("voxpreview", OBJTYPE_SHAPE);
    engine->setShape("voxpreview", "voxpreview");

    engine->setPos("voxpreview", 0.0, 0.0, 0.0);
    engine->setSize("voxpreview", 100, 100, 100);
    engine->setColor("voxpreview", 1.0, 1.0, 1.0, 1.0);
    engine->setTextureSpan("voxpreview", 8.0, 8.0);

    engine->setSystem("voxpreview", true);
    engine->setVisible("voxpreview", false);

    // Button dimensions
    int numbtns = 6;
    float bw = 0.3;
    float hbw = 0.15;
    float bs = 0.25;
    float bsv = 0.25;
    float hw = 1.0 / numbtns;

//	if (touchControls)
//	{
		// Top buttons
		engine->addWg("filebtn", WG_BTN, "file.png", "", "filebtnclicked", "", -hw * 5, 0.85, hw, hw);
		engine->addWg("objbtn", WG_BTN, "shapes.png", "", "objbtnclicked", "", -hw * 3, 0.85, hw, hw);
	    engine->addWg("orthobtn", WG_BTN, "pixels.png", "", "orthobtnclicked", "", -hw, 0.85, hw, hw);
		engine->addWg("modelbtn", WG_BTN, "poly.png", "", "modelbtnclicked", "", hw, 0.85, hw * 0.7, hw * 0.7);
		engine->addWg("prevbtn", WG_BTN, "prevkit.png", "", "prevbtnclicked", "", hw * 3, 0.85, hw, hw);
		engine->addWg("nextbtn", WG_BTN, "nextkit.png", "", "nextbtnclicked", "", hw * 5, 0.85, hw, hw);

		// Mid buttons
		engine->addWg("remove", WG_BTN, "redx.png", "", "removeclicked", "", -hw * 5, -0.5, hw, hw);
		engine->addWg("options", WG_BTN, "toolbox.png", "", "optionsclicked", "", -hw * 3, -0.5, hw,
			hw);
		engine->addWg("objmode", WG_BTN, "shapes.png", "", "objmodeclicked", "", -hw * 1, -0.5, hw, hw);
		engine->addWg("voxmode", WG_BTN, "cube.png", "", "voxmodeclicked", "", hw * 1, -0.5, hw, hw);
		engine->addWg("move", WG_BTN, "dpad.png", "", "moveclicked", "", hw * 3, -0.5, hw, hw);
		engine->addWg("add", WG_BTN, "nextkit.png", "", "addclicked", "", hw * 5, -0.5, hw * 1.4,
			hw * 1.4);

		// Ray length
		engine->addWg("shortenray", WG_BTN, "prevkit.png", "", "shortenrayclicked", "", -hw * 5, -0.1, hw, hw);
		engine->addWg("lengthenray", WG_BTN, "nextkit.png", "", "lengthenrayclicked", "", hw * 5, -0.1, hw, hw);
		engine->setWgColor("shortenray", 1.0, 1.0, 1.0, 0.5);
		engine->setWgColor("lengthenray", 1.0, 1.0, 1.0, 0.5);

		// Bottom buttons
		engine->addWg("moveleft", WG_BTN, "leftarrow.png", "", "moveleftclicked", "", -hw * 5, -0.8, hw,
			hw);
		engine->addWg("moveright", WG_BTN, "rightarrow.png", "", "moverightclicked", "", -hw * 3, -0.8,
			hw, hw);
		engine->addWg("movedown", WG_BTN, "downtriangle.png", "", "movedownclicked", "", -hw * 1, -0.8,
			hw, hw);
		engine->addWg("moveup", WG_BTN, "uptriangle.png", "", "moveupclicked", "", hw * 1, -0.8, hw,
			hw);
		engine->addWg("moveforward", WG_BTN, "uparrow.png", "", "moveforwardclicked", "", hw * 3, -0.8,
			hw, hw);
		engine->addWg("movebackward", WG_BTN, "downarrow.png", "", "movebackwardclicked", "", hw * 5,
			-0.8, hw, hw);
//	}

	engine->addText("msg", "", 0.0, 0.6, 0.08);
	engine->addText("msg2", "", 0.0, 0.5, 0.08);

/*	engine->addText("debug3", "debug3", 0.0, 0.4, 0.1);
	engine->addText("debug4", "debug4", 0.0, 0.3, 0.1);*/
}

/*
========================================
tick
========================================
*/

void Editor::tick() {
    static int timer = 0;
    static bool refreshPreview = false;
    static std::string fileSelectorAction = "";
    static const int msgTimerDelay = 50;
    static bool curVoxelChanged = false;

	GUI *gui = g_engine2->getGUI();
	
	// Show framedump if exists
    if (engine->getFrameDump() != "")
        gui->showLongText(engine->getFrameDump());

    if (engine->getExtraStr("tickscript") != "") {
        luaBridge.exec(engine->getExtraStr("tickscript"));
    }

    Controls2 *ctrl = engine->getControls();
    EditorController *ec = engine->getEditorController();

    //
    // UI CALLBACKS
    //

	if (!touchControls)
	{
		processExtraKeys();
//		processHWButtons();
	}

    static int fileBtnTimer = 0;

    if (timer == 0) {

		// File menu

		if (engine->getExtraInt("filebtnclicked") == 1
			|| engine->getExtraInt("filebtnclicked") == 3)
		{
//			if (!gui->nonNativeWidgetsShown())
//			{
				gui->clearListMenu();
				gui->addListMenuOption("New Scene", "");
				gui->addListMenuOption("Load Scene", "");
				gui->addListMenuOption("Save Scene", "");
				gui->addListMenuOption("Run script", "");
				gui->addListMenuOption("Clear and run script", "");
				gui->addListMenuOption("README", "");
#ifdef PLATFORM_ANDROID
                gui->addListMenuOption("Run Sample", "");
//				gui->addListMenuOption("Toggle Map Location", "");
#endif
				gui->showListMenuInDialog("File", "");
//			}

            engine->setExtraInt("filebtnclicked", 0);

            exitScreenShotMode();
            timer = 10;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "New Scene") {
            timer = 10;
            engine->clear();
            engine->clearGUI();
            load();
            engine->setExtraStr("lis1tmenuoptionclicked", "");
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Load Scene"
			|| engine->getExtraInt("Load Scene") == 1) {
            timer = 10;

			std::string fname = PLAT_LoadPref("main", "scene", "");
			if (fname == "")
                fname = g_assetsDir;

            gui->showFileSelector("sc", fname);
            engine->setExtraStr("listmenuoptionclicked", "");
			engine->setExtraInt("Load Scene", 0);
			fileSelectorAction = "loadscene";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Save Scene") {
            timer = 10;
            std::string fname = PLAT_LoadPref("main", "scene", "");

            if (fname == "")
                g_assetsDir + "/default.sc";

            gui->showFileSelector("sc", fname);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "savescene";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Run script") {
            timer = 10;
			
			std::string fname = PLAT_LoadPref("main", "script", "");
			if (fname == "")
		        fname = g_assetsDir;
				
            gui->showFileSelector("lua", fname);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "runscript";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Clear and run script") {
            timer = 10;
			
			std::string fname = PLAT_LoadPref("main", "script", "");
			if (fname == "")
		        fname = g_assetsDir;

            gui->showFileSelector("lua", fname);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "clearandrunscript";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "README") {
            timer = 10;
            std::string t;
            t = t + "Thank you for downloading Voxyc.\n";
            t = t + "Sample games have been included with this release.\n";
            t = t + "You can run the sample games by selecting File->\"Clear and run script\" and then selecting the load.lua file inside the sample's folder.\n";
            t = t + "This project is open-source.\n";
            t = t + "If there are certain features you would like,\n";
            t = t + "please feel free to contribute.\n";
            t = t + "\n";
            t = t + "GitHub: https://github.com/dimitrilozovoy/VoxycAndroid\n";
            t = t + "Contact e-mail: dimitrilozovoy@gmail.com";

            gui->showText(t);
            engine->setExtraStr("listmenuoptionclicked", "");
        }

#ifdef PLATFORM_ANDROID
        if (engine->getExtraStr("listmenuoptionclicked") == "Run Sample") {
            timer = 50;

            gui->clearListMenu();
            gui->addListMenuOption("Run FPS", "");
            gui->addListMenuOption("Run Scrolling Shooter", "");
            gui->addListMenuOption("Run Tanks", "");
            gui->showListMenuInDialog("Run Sample", "");

            engine->setExtraStr("listmenuoptionclicked", "");
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Run FPS") {
            timer = 50;

            std::string fname = PLAT_LoadPref("main", "script", "");
            if (fname == "")
                fname = g_assetsDir;

            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "luaprogram");
            engine->setExtraStr("loadscript", "load.lua");
            engine->setExtraStr("assetsdir", (std::string)g_externalFilesDir + "/samples/fps");

            engine->setExtraStr("listmenuoptionclicked", "");
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Run Scrolling Shooter") {
            timer = 50;

            std::string fname = PLAT_LoadPref("main", "script", "");
            if (fname == "")
                fname = g_assetsDir;

            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "luaprogram");
            engine->setExtraStr("loadscript", "load.lua");
            engine->setExtraStr("assetsdir", (std::string)g_externalFilesDir + "/samples/scrollingshooter");

            engine->setExtraStr("listmenuoptionclicked", "");
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Run Tanks") {
            timer = 50;

            std::string fname = PLAT_LoadPref("main", "script", "");
            if (fname == "")
                fname = g_assetsDir;

            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "luaprogram");
            engine->setExtraStr("loadscript", "load.lua");
            engine->setExtraStr("assetsdir", (std::string)g_externalFilesDir + "/samples/tanks");

            engine->setExtraStr("listmenuoptionclicked", "");
        }
#endif
		
        if (engine->getExtraStr("listmenuoptionclicked") == "Toggle Map Location") {
            timer = 50;

			if (!mapLocation)
			{
				PLAT_StartTrackLocation();
				mapLocation = true;
			}
			else
			{
				PLAT_StopTrackLocation();
				mapLocation = false;
			}

            engine->setExtraStr("listmenuoptionclicked", "");
        }

        // Object menu

        if (engine->getExtraInt("objbtnclicked") == 1
            || engine->getExtraInt("objbtnclicked") == 3) {

			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearListMenu();
				gui->addListMenuOption("Select Only", "");
				gui->addListMenuOption("Add Voxels", "");
				gui->addListMenuOption("Import Voxels", "");
				gui->addListMenuOption("Export Voxels", "");
				gui->addListMenuOption("Add Shape", "");
				gui->addListMenuOption("Add Model", "");
				gui->addListMenuOption("Add Sprite", "");
				gui->addListMenuOption("Set Sky", "");
				gui->addListMenuOption("Set Ambient Light", "");
                gui->addListMenuOption("Toggle Guides", "");
				gui->addListMenuOption("Hide GUI and Guides", "");
				gui->showListMenuInDialog("Object", "");
			}

            engine->setExtraInt("objbtnclicked", 0);

            exitScreenShotMode();
            timer = 50;
        }
		
		// 2D voxel editor button
		
        if (engine->getExtraInt("orthobtnclicked") == 1
            || engine->getExtraInt("orthobtnclicked") == 3) {

		    if (!modified)
			{
                engine->setExtraInt("switchmodule", 1);
                engine->setExtraStr("nextmodule", "orthoeditor");
				
            }
			else
			{
                gui->clearListMenu();
                gui->addListMenuOption("Discard and Enter 2D Voxel Editor", "");
                gui->addListMenuOption("Cancel", "");
                gui->showListMenuInDialog("Scene not saved!", "");
	        }
			
			exitScreenShotMode();
            timer = 50;

            engine->setExtraInt("orthobtnclicked", 0);
        }
		
		if (engine->getExtraStr("listmenuoptionclicked") == "Discard and Enter 2D Voxel Editor")
        {
            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "orthoeditor");
			
            engine->setExtraStr("listmenuoptionclicked", "");
        }
		
		// Model editor button
		
        if (engine->getExtraInt("modelbtnclicked") == 1
            || engine->getExtraInt("modelbtnclicked") == 3) {

		    if (!modified)
			{
                engine->setExtraInt("switchmodule", 1);
                engine->setExtraStr("nextmodule", "modeleditor");
            }
			else
			{
                gui->clearListMenu();
                gui->addListMenuOption("Discard And Enter Mesh Editor", "");
                gui->addListMenuOption("Cancel", "");
                gui->showListMenuInDialog("Scene not saved!", "");
	        }
				
            engine->setExtraInt("modelbtnclicked", 0);

            exitScreenShotMode();
            timer = 50;
        }
		
		if (engine->getExtraStr("listmenuoptionclicked") == "Discard And Enter Mesh Editor")
        {
            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "modeleditor");
			
            engine->setExtraStr("listmenuoptionclicked", "");
        }
		
		// Object menu continued

        if (engine->getExtraStr("listmenuoptionclicked") == "Select Only") {
            selectOnly = true;
            engine->setExtraStr("listmenuoptionclicked", "");
//            timer = 50;
        }

		if (engine->getExtraStr("listmenuoptionclicked") == "Import Voxels") {
            timer = 50;

			std::string fname = PLAT_LoadPref("main", "voxels", "");
			if (fname == "")
                fname = g_assetsDir;

            gui->showFileSelector("vx", fname);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importvoxels";
        }
		
		if (engine->getExtraStr("fileselected") != "") {
            timer = 50;
			
		    engine->setAssetsDir(GetPath(engine->getExtraStr("fileselected")));

            if (fileSelectorAction == "importvoxels") {
				PLAT_SavePref("main", "voxels", engine->getExtraStr("fileselected"));
				
		    selectOnly = false;
            mode = EM_OBJ;

/*			if (!gui->nonNativeWidgetsShown())
			{
					gui->clearDialog();
					gui->addDialogPart("Resolution", "16", "newvoxelsresolution");
                    gui->addDialogPart("Size", "10", "newvoxelssize");
					gui->showDialog("Voxel Parameters", "OK", "Cancel", "voxelparamsselected");
			}*/

            engine->newShape("voxels", SHAPE_VOXELS, 10.0, 0);

            std::string name = "objpreview";

            engine->setType(name, OBJTYPE_SHAPE);
            engine->setShape(name, "voxels");
            engine->setSize(name, 10, 10, 10);
            engine->setTexture(name, "bluecube.png");
            engine->setAlwaysFacePlayer(name, false);

			placingObj = true;
			objCopy = false;
			objPreviewDist = defaultObjPreviewDist;
							
                char newNamec[1024];
                snprintf(newNamec, 1024, "obj%d", RandomInt(0, 10000));
                std::string newName = std::string(newNamec);

                engine->copyObj("objpreview", std::string(newName));
                Object *newObj = engine->findObj(newName);
                newObj->name = newName;
                newObj->system = false;

                Shape *shape = newObj->shape;

                // Added voxels? Create new voxel shape for it
                if (shape != nullptr && shape->voxels != nullptr && !objCopy) {
                    engine->newShape(newName, SHAPE_VOXELS, newVoxelResolution, 0.0);
                    engine->setShape(newName, newName);

                    // Default voxel texture values
/*                    engine->setVoxelTexture(newName, 1, "brick3.png");
                    engine->setVoxelTexture(newName, 2, "brick4.png");
                    engine->setVoxelTexture(newName, 3, "concrete1.png");
                    engine->setVoxelTexture(newName, 4, "tile.png");
                    engine->setVoxelTexture(newName, 5, "stone1.png");
                    engine->setVoxelTexture(newName, 6, "stone2.png");
                    engine->setVoxelTexture(newName, 7, "beigestone.png");
                    engine->setVoxelTexture(newName, 8, "asphalt.png");
                    engine->setVoxelTexture(newName, 9, "graychips.png");
                    engine->setVoxelTexture(newName, 10, "leaves.png");
                    engine->setVoxelTexture(newName, 11, "bluecube.png");*/

                    engine->setTextureSpan("brick3.png", 2.0f, 2.0f);
                    engine->setTextureSpan("brick4.png", 2.0f, 2.0f);
                    engine->setTextureSpan("concrete1.png", 2.0f, 2.0f);
                    engine->setTextureSpan("tile.png", 2.0f, 2.0f);
                    engine->setTextureSpan("stone1.png", 2.0f, 2.0f);
                    engine->setTextureSpan("stone2.png", 2.0f, 2.0f);
                    engine->setTextureSpan("beigestone.png", 2.0f, 2.0f);
                    engine->setTextureSpan("asphalt.png", 2.0f, 2.0f);
                    engine->setTextureSpan("graychips.png", 2.0f, 2.0f);
                    engine->setTextureSpan("leaves.png", 2.0f, 2.0f);
                    engine->setTextureSpan("bluecube.png", 2.0f, 2.0f);
                }

                // Added voxels? Switch to voxels mode
                if (shape != nullptr && shape->voxels != nullptr && !objCopy) {
                    // Swtich to voxel mode
                    mode = EM_VOX;
                    engine->setText("msg2", "voxel mode");
                    msg2Timer = msgTimerDelay;
                    rayLength = defaultRayLength;
                }

				if (shape != nullptr && shape->voxels != nullptr)
				{
					// Not link copy? We need to copy the voxels
					engine->newShape(newName, SHAPE_VOXELS, shape->voxels->getSize(), 0.0);
					engine->setShape(newName, newName);
					newObj->shape->voxels->load(engine->getExtraStr("fileselected"), nullptr, engine->getTextureManager());
				}

				newObj->visible = true;

                char msg[1024];
                snprintf(msg, 1024, "%s placed", newName.c_str());
                engine->setText("msg", msg);
                msgTimer = msgTimerDelay;

                engine->setExtraStr("fileselected", "");
            }
        }
		
		if (engine->getExtraStr("listmenuoptionclicked") == "Export Voxels") {
            timer = 50;

			std::string fname = PLAT_LoadPref("main", "voxels", "");
			if (fname == "")
                fname = g_assetsDir + "/default.vx";

            gui->showFileSelector("vx", fname);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "exportvoxels";
        }
		
		if (engine->getExtraStr("fileselected") != "") {
            timer = 50;

            if (fileSelectorAction == "exportvoxels") {
				PLAT_SavePref("main", "voxels", engine->getExtraStr("fileselected"));
				
				if (curVoxels != nullptr)
			    {
					Voxels *v = curVoxels->shape->voxels;
					
					v->save(engine->getExtraStr("fileselected"), nullptr);
					
					char msg[1024];
                    snprintf(msg, 1024, "%s saved", GetFileName(engine->getExtraStr("fileselected")).c_str());
                    engine->setText("msg", msg);
                    msgTimer = msgTimerDelay;
				}
				
                engine->setExtraStr("fileselected", "");
            }
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Shape") {

			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearListMenu();
				gui->addListMenuOption("Add Terrain", "");
				gui->addListMenuOption("Add Block", "");
				gui->showListMenuInDialog("Add Shape", "");
			}

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Model") {
            timer = 50;
            gui->showFileSelector("obj", g_assetsDir);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "addmodel";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Sprite") {
            timer = 50;
            gui->showFileSelector("png", g_assetsDir);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "addsprite";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Set Sky") {
            timer = 50;
            gui->showFileSelector("png", g_assetsDir);
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "setskybox";
        }
		
		// Set ambient light
        if (engine->getExtraStr("listmenuoptionclicked") == "Set Ambient Light") {

        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

            if (!gui->nonNativeWidgetsShown())
            {
                gui->clearDialog();
                gui->addDialogPart("r (0.0-2.0)", "1.0", "lightr");
                gui->addDialogPart("g (0.0-2.0)", "1.0", "lightg");
                gui->addDialogPart("b (0.0-2.0)", "1.0", "lightb");
                gui->showDialog("Set Ambient Light", "OK", "Cancel", "setambientlight_entered");
            }
        }

        engine->setExtraStr("listmenuoptionclicked", "");
        }

        if (engine->getExtraInt("setambientlight_entered") == 1)
        {
        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            lightr = PLAT_stof(engine->getExtraStr("lightr"), 1.0f);
            lightg = PLAT_stof(engine->getExtraStr("lightg"), 1.0f);
            lightb = PLAT_stof(engine->getExtraStr("lightb"), 1.0f);

            g_common.ambientr = lightr;
			g_common.ambientg = lightg;
            g_common.ambientb = lightb;
			
            engine->setExtraInt("setambientlight_entered", 0);
        }
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Toggle Guides") {
            showGuides = !showGuides;

            if (showGuides)
                showSystemObjects();
            else
                hideSystemObjects();

            engine->setExtraStr("listmenuoptionclicked", "");
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Hide GUI and Guides") {
            enterScreenShotMode();
            engine->setExtraStr("listmenuoptionclicked", "");
        }

        // File selector actions

        if (engine->getExtraStr("fileselected") != "") {
            timer = 50;

            if (fileSelectorAction == "loadscene") {
				PLAT_SavePref("main", "scene", engine->getExtraStr("fileselected"));
                engine->clear();
                engine->clearGUI();
                load();
                engine->setAssetsDir(GetPath(engine->getExtraStr("fileselected")));
                engine->loadScene(GetFileName(engine->getExtraStr("fileselected")));
				modified = false;
				
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "savescene") {
				PLAT_SavePref("main", "scene", engine->getExtraStr("fileselected"));
				engine->setAssetsDir(GetPath(engine->getExtraStr("fileselected")));
				engine->saveScene(engine->getExtraStr("fileselected"));

                char msg[1024];
                snprintf(msg, 1024, "%s saved", GetFileName(engine->getExtraStr("fileselected")).c_str());
                engine->setText("msg", msg);
                msgTimer = msgTimerDelay;
				modified = false;

				engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "importvoxeltexture") {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                engine->setVoxelTexture("voxels", ec->getCurTexture(), fname);
                engine->rebuildShape("voxels");
                engine->setVoxelTexture("preview", ec->getCurTexture(), fname);
                engine->rebuildShape("preview");
                engine->setExtraStr("fileselected", "");
				modified = true;
            }

            if (fileSelectorAction == "importterraintexture") {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));

                engine->setTexture("terrain", fname);
                engine->setExtraStr("terraintexture", fname);
                engine->setExtraStr("fileselected", "");
				modified = true;
            }

            if (fileSelectorAction == "importskytexture") {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                engine->setSkybox(fname);
                engine->setExtraStr("skytexture", fname);
                engine->setExtraStr("fileselected", "");
				modified = true;
            }

            if (fileSelectorAction == "writeobj") {
                engine->writeObj("voxels", engine->getExtraStr("fileselected"));
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "runscript") {
                PLAT_SavePref("main", "script", engine->getExtraStr("fileselected"));
				engine->setAssetsDir(GetPath(engine->getExtraStr("fileselected")));
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                luaBridge.exec(fname);
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "clearandrunscript") {
				PLAT_SavePref("main", "script", engine->getExtraStr("fileselected"));
                engine->setExtraInt("switchmodule", 1);
                engine->setExtraStr("nextmodule", "luaprogram");
                engine->setExtraStr("loadscript", GetFileName(engine->getExtraStr("fileselected")));
                engine->setExtraStr("assetsdir", GetPath(engine->getExtraStr("fileselected")));
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "importobj") {
                // engine->getExtraStr("fileselected"));
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "addmodel") {

				if (verifySourceDir(engine->getExtraStr("fileselected")))
				{
					selectOnly = false;

					mode = EM_OBJ;
					std::string name = "objpreview";
					engine->removeObject(name);
					engine->addObject(name);
					engine->setSystem(name, true);
					engine->setVisible(name, true);
					engine->setColor(name, 1.0, 1.0, 1.0, 1.0);
					engine->setType(name, OBJTYPE_MODEL);
					engine->setModel(name, GetFileName(engine->getExtraStr("fileselected")));
					engine->setTexture(name, "bluecube.png");
					engine->setAlwaysFacePlayer(name, false);
					engine->setSize(name, 1.0, 1.0, 1.0);

					placingObj = true;
					objCopy = false;
                    objPreviewDist = defaultObjPreviewDist;
				}

				engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "addsprite") {
				if (verifySourceDir(engine->getExtraStr("fileselected")))
				{
					selectOnly = false;

					mode = EM_OBJ;
					std::string name = "objpreview";
					engine->removeObject(name);
					engine->addObject(name);
					engine->setSystem(name, true);
					engine->setVisible(name, true);
					engine->setColor(name, 1.0, 1.0, 1.0, 1.0);
					engine->setType(name, OBJTYPE_SPRITE);
					engine->setTexture(name, GetFileName(engine->getExtraStr("fileselected")));
					engine->setAlwaysFacePlayer(name, true);
					engine->setSize(name, 1.0, 1.0, 1.0);

					placingObj = true;
					objCopy = false;
					objPreviewDist = defaultObjPreviewDist;
				}
				
				engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "setskybox") {

				if (verifySourceDir(engine->getExtraStr("fileselected")))
				{
					std::string fname = GetFileName(engine->getExtraStr("fileselected"));
					engine->setSkybox(fname);
				}

                engine->setExtraStr("fileselected", "");
            }
        }

        //
        // Add terrain
        //

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Terrain") {
            selectOnly = false;
            mode = EM_OBJ;

			if (!gui->nonNativeWidgetsShown())
			{
				if (!gui->nonNativeWidgetsShown())
				{
					gui->clearDialog();
					gui->addDialogPart("size", "1000", "size");
					gui->addDialogPart("detail", "16", "detail");
					gui->addDialogPart("valley x", "8", "valleysizex");
					gui->addDialogPart("valley z", "8", "valleysizez");
					gui->addDialogPart("drop x", "0", "dropsizex");
					gui->addDialogPart("drop z", "0", "dropsizez");
					gui->addDialogPart("bottom x", "0", "bottomsizex");
					gui->addDialogPart("bottom z", "0", "bottomsizez");
				    gui->addDialogPart("height", "25", "height");
					gui->showDialog("New Terrain", "OK", "Cancel", "newterrainparams_entered");
				}
			}

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }

        if (engine->getExtraInt("newterrainparams_entered") == 1)
        {
            float size = PLAT_stof(engine->getExtraStr("size"), 1000);
            float detail = PLAT_stof(engine->getExtraStr("detail"), 16);
            float valleysizex = PLAT_stof(engine->getExtraStr("valleysizex"), 8);
            float valleysizez = PLAT_stof(engine->getExtraStr("valleysizez"), 8);
            float dropsizex = PLAT_stof(engine->getExtraStr("dropsizex"), 0);
            float dropsizez = PLAT_stof(engine->getExtraStr("dropsizez"), 0);
            float bottomsizex = PLAT_stof(engine->getExtraStr("bottomsizex"), 0);
            float bottomsizez = PLAT_stof(engine->getExtraStr("bottomsizez"), 0);
            float height = PLAT_stof(engine->getExtraStr("height"), 25);

            char newNamec[1024];
            snprintf(newNamec, 1024, "obj%d", RandomInt(0, 10000));
            std::string newName = std::string(newNamec);

            engine->newShape(newName, SHAPE_TERRAIN, detail, valleysizex, valleysizez, dropsizex, dropsizez, bottomsizex, bottomsizez);

            std::string name = "objpreview";

            engine->setType(name, OBJTYPE_SHAPE);
            engine->setShape(name, newName);
            engine->setSize(name, size, height, size);
            engine->setTexture(name, "grass1.png");
            engine->setAlwaysFacePlayer(name, false);

			placingObj = true;
			objCopy = false;
			objPreviewDist = defaultObjPreviewDist;

            engine->setExtraInt("newterrainparams_entered", 0);
        }

        //
        // Add voxels
        //

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Voxels") {
            selectOnly = false;
            mode = EM_OBJ;

			if (!gui->nonNativeWidgetsShown())
			{
					gui->clearDialog();
					gui->addDialogPart("Resolution", "16", "newvoxelsresolution");
                    gui->addDialogPart("Size", "10", "newvoxelssize");
					gui->showDialog("Voxel Parameters", "OK", "Cancel", "voxelparamsselected");
			}

            engine->newShape("voxels", SHAPE_VOXELS, 10.0, 0);

            std::string name = "objpreview";

            engine->setType(name, OBJTYPE_SHAPE);
            engine->setShape(name, "voxels");
            engine->setSize(name, 10, 10, 10);
            engine->setTexture(name, "bluecube.png");
            engine->setAlwaysFacePlayer(name, false);

			placingObj = true;
			objCopy = false;
			objPreviewDist = defaultObjPreviewDist;

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }
		
		//
		// Add block
		//

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Block") {
            selectOnly = false;
            mode = EM_OBJ;

            engine->newShape("block", SHAPE_BLOCK, 1.0, 1.0, 1.0);

            std::string name = "objpreview";

            engine->setType(name, OBJTYPE_SHAPE);
            engine->setShape(name, "block");
            engine->setSize(name, 1, 1, 1);
            engine->setTexture(name, "bluecube.png");
            engine->setAlwaysFacePlayer(name, false);

			placingObj = true;
			objCopy = false;
			objPreviewDist = defaultObjPreviewDist;

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }

        // Other buttons

        static int switchVoxTimer = 0;

        if (switchVoxTimer > 0)
            switchVoxTimer--;

        if ((engine->getExtraInt("prevbtnclicked") == 1
             || engine->getExtraInt("prevbtnclicked") == 3)) {
            if (mode == EM_VOX && curVoxel > 0 && switchVoxTimer == 0) {
                curVoxel--;
                switchVoxTimer = 10;
				engine->setExtraInt("prevbtnclicked", 0);
            } else if (mode == EM_OBJ && selectedObj != nullptr) {
                selectedObj->scale *= 0.99;
            } else if (mode == EM_OBJ && selectedObj == nullptr) {
                Object *objPreview = engine->findObj("objpreview");
                if (objPreview != nullptr)
                    objPreview->scale *= 0.99;
            }

            exitScreenShotMode();
            refreshPreview = true;
            curVoxelChanged = true;
        }

        if ((engine->getExtraInt("nextbtnclicked") == 1
             || engine->getExtraInt("nextbtnclicked") == 3)) {
            if (mode == EM_VOX && curVoxel < 255 && switchVoxTimer == 0) {
                curVoxel++;
                switchVoxTimer = 10;
				engine->setExtraInt("nextbtnclicked", 0);
            } else if (mode == EM_OBJ && selectedObj != nullptr) {
                selectedObj->scale *= 1.01;
            } else if (mode == EM_OBJ && selectedObj == nullptr) {
                Object *objPreview = engine->findObj("objpreview");
                if (objPreview != nullptr)
                    objPreview->scale *= 1.01;
            }

            exitScreenShotMode();
            refreshPreview = true;
            curVoxelChanged = true;
        }

        if ((engine->getExtraInt("camerabtnclicked") == 1
             || engine->getExtraInt("camerabtnclicked") == 3)) {
            std::string fname = PLAT_GetCameraPic();

            engine->setExtraStr("camerapicfilename", fname);
            engine->setExtraInt("camerabtnclicked", 0);

            timer = 50;
        }

        if (engine->getExtraInt("camerapicready") == 1) {
            engine->setVoxelTexture("voxels", ec->getCurTexture(),
                                    engine->getExtraStr("camerapicfilename"));
            engine->rebuildShape("voxels");
            engine->setExtraInt("camerapicready", 0);
        }
    } else
        timer--;

    if (engine->getExtraInt("voxelparamsselected") == 1) {
        selectOnly = false;

        // Resolution
        newVoxelResolution = PLAT_stoi(engine->getExtraStr("newvoxelsresolution"));

        if (newVoxelResolution <= 0)
            newVoxelResolution = 16;
        if (newVoxelResolution > 64)
            newVoxelResolution = 64;

        // Size
        std::string name = "objpreview";
        int size = PLAT_stoi(engine->getExtraStr("newvoxelssize"));
        engine->setSize(name, size, size, size);

        engine->setExtraInt("voxelparamsselected", 0);
    }

    float rayDelta = 2.0;

    if (engine->getExtraInt("shortenrayclicked") == 1
        || engine->getExtraInt("shortenrayclicked") == 3) {
        if (mode == EM_VOX)
        {
            if (rayLength > minRayLength)
                rayLength -= rayDelta * rayDeltaMultiplier;
        }

        if (mode == EM_OBJ)
        {
            if (distToSelected > minRayLength)
                distToSelected -= rayDelta * rayDeltaMultiplier;
        }

        if (placingObj)
        {
            if (objPreviewDist > minRayLength)
                objPreviewDist -= rayDelta * rayDeltaMultiplier;
        }

        engine->setExtraInt("shortenrayclicked", 0);
    }

    if (engine->getExtraInt("lengthenrayclicked") == 1
        || engine->getExtraInt("lengthenrayclicked") == 3) {
        if (mode == EM_VOX)
        {
            if (rayLength > minRayLength)
                rayLength += rayDelta * rayDeltaMultiplier;
        }

        if (mode == EM_OBJ)
        {
            if (distToSelected > minRayLength)
                distToSelected += rayDelta * rayDeltaMultiplier;
        }

        if (placingObj)
        {
            if (objPreviewDist > minRayLength)
                objPreviewDist += rayDelta * rayDeltaMultiplier;
        }
        engine->setExtraInt("lengthenrayclicked", 0);
    }

    //
    // MOVEMENT
    //

	tickMovement();
	
/*    static bool movingLeft = false;
    static bool movingRight = false;
    static bool movingBackward = false;
    static bool movingForward = false;
    static bool movingDown = false;
    static bool movingUp = false;

    // Moving left

    if (engine->getExtraInt("moveleftclicked") == 1
        || engine->getExtraInt("moveleftclicked") == 3) {
        movingLeft = true;
        movingRight = false;
        movingBackward = false;
        movingForward = false;
        movingDown = false;
        movingUp = false;
        exitScreenShotMode();
    }

    // Moving right

    else if (engine->getExtraInt("moverightclicked") == 1
             || engine->getExtraInt("moverightclicked") == 3) {
        movingLeft = false;
        movingRight = true;
        movingBackward = false;
        movingForward = false;
        movingDown = false;
        movingUp = false;
        exitScreenShotMode();
    }

    // Moving backward

    else if (engine->getExtraInt("movebackwardclicked") == 1
             || engine->getExtraInt("movebackwardclicked") == 3) {
        movingLeft = false;
        movingRight = false;
        movingBackward = true;
        movingForward = false;
        movingDown = false;
        movingUp = false;
        exitScreenShotMode();
    }

    // Moving forward

    else if (engine->getExtraInt("moveforwardclicked") == 1
             || engine->getExtraInt("moveforwardclicked") == 3) {
        movingLeft = false;
        movingRight = false;
        movingBackward = false;
        movingForward = true;
        movingDown = false;
        movingUp = false;
        exitScreenShotMode();
    }

    // Moving down

    else if (engine->getExtraInt("movedownclicked") == 1
             || engine->getExtraInt("movedownclicked") == 3) {
        movingLeft = false;
        movingRight = false;
        movingBackward = false;
        movingForward = false;
        movingDown = true;
        movingUp = false;
        exitScreenShotMode();
    }

    // Moving up

    else if (engine->getExtraInt("moveupclicked") == 1
             || engine->getExtraInt("moveupclicked") == 3) {
        movingLeft = false;
        movingRight = false;
        movingBackward = false;
        movingForward = false;
        movingDown = false;
        movingUp = true;
        exitScreenShotMode();
    }

    // Stop if finger up

    if (engine->getExtraInt("touchaction1") == 4
        || engine->getExtraInt("touchaction2") == 4) {
        movingLeft = false;
        movingRight = false;
        movingBackward = false;
        movingForward = false;
        movingDown = false;
        movingUp = false;
    }

    if (engine->getExtraInt("movedownclicked") == 4)
        movingDown = false;
    if (engine->getExtraInt("moveleftclicked") == 4)
        movingLeft = false;
    if (engine->getExtraInt("moverightclicked") == 4)
        movingRight = false;
    if (engine->getExtraInt("movebackwardclicked") == 4)
        movingBackward = false;
    if (engine->getExtraInt("moveforwardclicked") == 4)
        movingForward = false;
    if (engine->getExtraInt("moveupclicked") == 4)
        movingUp = false;

    Object *playerObj = engine->getPlayerObj();
	Controls2 *controls = engine->getControls();

    if (movingForward)
        playerObj->MoveForward(controls->getMoveFactor());
//		ctrl->setBtn(BTN_UP, 1);
//	else
//		ctrl->setBtn(BTN_UP, 0);

    if (movingBackward)
        playerObj->MoveForward(-controls->getMoveFactor());
//		ctrl->setBtn(BTN_DOWN, 1);
//	else
//		ctrl->setBtn(BTN_DOWN, 0);

    if (movingLeft)
        ctrl->setBtn(BTN_LEFT, 1);
	else
		ctrl->setBtn(BTN_LEFT, 0);

	if (movingRight)
		ctrl->setBtn(BTN_RIGHT, 1);
	else
		ctrl->setBtn(BTN_RIGHT, 0);

    if (movingUp)
        ctrl->MoveUp();

    if (movingDown)
        ctrl->MoveDown();*/

    //
    // PREVIEW
    //

    static int putvoxx = 0;
    static int putvoxy = 0;
    static int putvoxz = 0;

    static bool voxPreviewSetup = false;

    Object *player = engine->getPlayerObj();

    // Update voxel preview

    static int refreshPreviewTimer = 0;

    if (refreshPreviewTimer > 0)
        refreshPreviewTimer--;

    Shape *previewShape = engine->findShape("voxpreview");

    if (previewShape != nullptr) {
        previewShape->getVoxels()->clear();

        setVoxPreviewTextures();

        float wx = 0.0;
        float wy = 0.0;
        float wz = 0.0;

        ray(player, rayLength, wx, wy, wz);

        // See if the point is in a voxel object
        Object *target = nullptr;

        std::map<std::string, Object *> objs = engine->getObjects();

        for (const auto &pair: objs) {
            Object *obj = pair.second;

            if (isVoxels(pair.first) && !obj->system) {
                if (wx > (obj->position.x - obj->scale.x / 2.0)
                    && wx < (obj->position.x + obj->scale.x / 2.0)
                    && wy > (obj->position.y - obj->scale.y / 2.0)
                    && wy < (obj->position.y + obj->scale.y / 2.0)
                    && wz > (obj->position.z - obj->scale.z / 2.0)
                    && wz < (obj->position.z + obj->scale.z / 2.0)) {
                    target = obj;
                }
            }
        }

        // See if target is not curVoxels
        if (target != nullptr && target != curVoxels) {
            curVoxels = target;
            engine->setEditVoxelsObj(curVoxels->name);
            voxPreviewSetup = false;
        }

        if (!voxPreviewSetup && curVoxels != nullptr) {
            // Move preview onto the new vox object
            engine->deleteShape("voxpreview");

            engine->newShape("voxpreview", SHAPE_VOXELS, curVoxels->shape->voxels->getSize(), 0.0);
            engine->setShape("voxpreview", "voxpreview");

            Object *voxPreview = engine->findObj("voxpreview");

            voxPreview->position = curVoxels->position;
            voxPreview->scale = curVoxels->scale;
            voxPreview->visible = true;

            setVoxPreviewTextures();

            voxPreviewSetup = true;
        }

        if (curVoxels != nullptr) {
            int size = curVoxels->shape->voxels->getSize();

            static int lastx = -1, lasty = -1, lastz = -1;
			int x = 0, y = 0, z = 0;

			worldToVoxelCoords(wx, wy, wz, x, y, z);

            if (x >= 0 && x < size
                && y >= 0 && y < size
                && z >= 0 && z < size
                && (x != lastx || y != lasty || z != lastz || curVoxelChanged)) {
                engine->setVoxel("voxpreview", x, y, z, curVoxel);

                putvoxx = x;
                putvoxy = y;
                putvoxz = z;

                previewShape->needsRebuild = true;
                refreshPreview = false;
                refreshPreviewTimer = 0;
                curVoxelChanged = false;
            }

            lastx = x;
            lasty = y;
            lastz = z;
        }

    }

    // Move vox preview onto voxels
    if (curVoxels != nullptr) {
        Object *voxPreview = engine->findObj("voxpreview");
        voxPreview->position = curVoxels->position;
        voxPreview->scale = curVoxels->scale;
    }

    // Update object preview
    Object *objPreview = engine->findObj("objpreview");

    if (objPreview != nullptr) {
        objPreview->position = player->position;
        objPreview->setDeltaXYZ(player->yaw, player->pitch, objPreviewDist);
        objPreview->move();
    }

    if (selectOnly) {
        engine->setVisible("objpreview", false);
        engine->setVisible("voxpreview", false);
        mode = EM_OBJ;
    }

    if (mode == EM_OBJ) {
        engine->setVisible("voxpreview", false);
    }

    if (mode == EM_VOX) {
        engine->setVisible("objpreview", false);
    }

    //
    // BUTTONS
    //

	//
    // Add button
	//

	Object* playerObj = engine->getPlayerObj();
	Controls2* controls = engine->getControls();
	
	static int addTimer = 0;

    if (addTimer > 0)
        addTimer--;

    static int placeObjTimer = 0;

    if (placeObjTimer > 0)
        placeObjTimer--;

    if ((engine->getExtraInt("addclicked") == 1
         || engine->getExtraInt("addclicked") == 3)
        && addTimer == 0 && !selectOnly) {

        if (mode == EM_VOX && curVoxels != nullptr) {

            //
            // Place voxel / light voxel
            //

            if (lightVoxels)
            {
                Voxels *voxels = curVoxels->shape->voxels;
                voxels->setrgba(putvoxx, putvoxy, putvoxz, FloatToUChar(lightr), FloatToUChar(lightg), FloatToUChar(lightb), 127);
                curVoxels->shape->needsRebuild = true;
            }
            else
            {
                Shape *shape = curVoxels->shape;
                int vox = engine->getVoxel(shape->name, putvoxx, putvoxy, putvoxz);

                if (vox != curVoxel) {
                    engine->setVoxel(shape->name, putvoxx, putvoxy, putvoxz, curVoxel);
                    shape->needsRebuild = true;

                    Object *voxPreview = engine->findObj("voxpreview");
                    voxPreview->shape->needsRebuild = true;
                }

                objCopy = false;
            }
			
			modified = true;

        } else if (mode == EM_OBJ) {

            //
            // Place object
            //

            if (placeObjTimer == 0 && placingObj) {
                char newNamec[1024];
                snprintf(newNamec, 1024, "obj%d", RandomInt(0, 10000));
                std::string newName = std::string(newNamec);

                engine->copyObj("objpreview", std::string(newName));
                Object *newObj = engine->findObj(newName);
                newObj->name = newName;
                newObj->system = false;

                Shape *shape = newObj->shape;

                // Added voxels? Create new voxel shape for it
                if (shape != nullptr && shape->voxels != nullptr && !objCopy) {
                    engine->newShape(newName, SHAPE_VOXELS, newVoxelResolution, 0.0);
                    engine->setShape(newName, newName);

                    // Default voxel texture values
                    engine->setVoxelTexture(newName, 1, "brick3.png");
                    engine->setVoxelTexture(newName, 2, "brick4.png");
                    engine->setVoxelTexture(newName, 3, "concrete1.png");
                    engine->setVoxelTexture(newName, 4, "tile.png");
                    engine->setVoxelTexture(newName, 5, "stone1.png");
                    engine->setVoxelTexture(newName, 6, "stone2.png");
                    engine->setVoxelTexture(newName, 7, "beigestone.png");
                    engine->setVoxelTexture(newName, 8, "asphalt.png");
                    engine->setVoxelTexture(newName, 9, "graychips.png");
                    engine->setVoxelTexture(newName, 10, "leaves.png");
                    engine->setVoxelTexture(newName, 11, "bluecube.png");

                    engine->setTextureSpan("brick3.png", 2.0f, 2.0f);
                    engine->setTextureSpan("brick4.png", 2.0f, 2.0f);
                    engine->setTextureSpan("concrete1.png", 2.0f, 2.0f);
                    engine->setTextureSpan("tile.png", 2.0f, 2.0f);
                    engine->setTextureSpan("stone1.png", 2.0f, 2.0f);
                    engine->setTextureSpan("stone2.png", 2.0f, 2.0f);
                    engine->setTextureSpan("beigestone.png", 2.0f, 2.0f);
                    engine->setTextureSpan("asphalt.png", 2.0f, 2.0f);
                    engine->setTextureSpan("graychips.png", 2.0f, 2.0f);
                    engine->setTextureSpan("leaves.png", 2.0f, 2.0f);
                    engine->setTextureSpan("bluecube.png", 2.0f, 2.0f);
                }

                // Added voxels? Switch to voxels mode
                if (shape != nullptr && shape->voxels != nullptr && !objCopy) {
                    // Swtich to voxel mode
                    mode = EM_VOX;
                    engine->setText("msg2", "voxel mode");
                    msg2Timer = msgTimerDelay;
                    rayLength = defaultRayLength;
                }

				if (shape != nullptr && shape->voxels != nullptr && objCopy && !linkCopy)
				{
					// Not link copy? We need to copy the voxels
					engine->newShape(newName, SHAPE_VOXELS, shape->voxels->getSize(), 0.0);
					engine->setShape(newName, newName);
					newObj->shape->voxels->copyFrom(objPreview->shape->voxels);
				}

				newObj->visible = true;

                char msg[1024];
                snprintf(msg, 1024, "%s placed", newName.c_str());
                engine->setText("msg", msg);
                msgTimer = msgTimerDelay;
				modified = true;

                placeObjTimer = 30;
            }
		}

        exitScreenShotMode();
    }

	//
    // Remove button
	//

    static int rmObjTimer = 0;

    if (rmObjTimer > 0)
        rmObjTimer--;

    if (engine->getExtraInt("removeclicked") != 0) {

		if (placingObj)
		{
			placingObj = false;

			if (engine->getVisible("voxpreview"))
				engine->setVisible("voxpreview", false);
			if (engine->getVisible("objpreview"))
				engine->setVisible("objpreview", false);
		}

		if (objCopy)
		{
			objCopy = false;
			rmObjTimer = 30;
		}

        if (mode == EM_VOX) {

			//
            // Delete voxel
			//

            if (curVoxels != nullptr) {
                Shape *shape = curVoxels->shape;
                engine->setVoxel(shape->name, putvoxx, putvoxy, putvoxz, 0);

				modified = true;
                shape->needsRebuild = true;
            }
        } else if (mode == EM_OBJ) {

			//
            // Delete object
			//

            if (selectedObj != nullptr && rmObjTimer == 0) {
                // If voxels, remove guides
                if (isVoxels(selectedObj->name)) {
                    std::string name = selectedObj->name;

                    std::string gnameb = name + "_guide_bottom";
                    std::string gnamet = name + "_guide_top";
                    std::string gnamel = name + "_guide_left";
                    std::string gnamer = name + "_guide_right";
                    std::string gnamebk = name + "_guide_back";
                    std::string gnamef = name + "_guide_front";

                    std::string gnamec = name + "_guide_center";

                    engine->removeObject(gnameb);
                    engine->removeObject(gnamet);
                    engine->removeObject(gnamel);
                    engine->removeObject(gnamer);
                    engine->removeObject(gnamebk);
                    engine->removeObject(gnamef);

                    engine->removeObject(gnamec);
				}

                engine->removeObject(selectedObj);
                engine->setText("msg", "object removed");

				modified = true;
				rmObjTimer = 30;
            }
		}

		if (lightVoxels)
		    lightVoxels = false;

        exitScreenShotMode();
    }

	//
    // Options button
	//

    static int optTimer = 0;

    if (optTimer > 0)
        optTimer--;

    if (engine->getExtraInt("optionsclicked") != 0 && optTimer == 0)
    {
        if (mode == EM_OBJ) {

			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearListMenu();
				gui->addListMenuOption("Set name", "");
				gui->addListMenuOption("Set texture", "");
				gui->addListMenuOption("Set position", "");
				gui->addListMenuOption("Set orientation", "");
				gui->addListMenuOption("Set scale", "");
				gui->addListMenuOption("Copy", "");
				gui->addListMenuOption("Link copy", "");
				gui->showListMenuInDialog("Options", "");
			}

            optTimer = 30;
        } else if (mode == EM_VOX) {

			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearListMenu();
				gui->addListMenuOption("Set texture", "");
				gui->addListMenuOption("Set texture span", "");
                gui->addListMenuOption("Set texture light", "");
                gui->addListMenuOption("Light room", "");
                gui->addListMenuOption("Light voxels", "");
				gui->showListMenuInDialog("Options", "");
			}

            optTimer = 30;
        }

        exitScreenShotMode();
    }
	
	// Set name
	
    if (engine->getExtraStr("listmenuoptionclicked") == "Set name") {
		
		if (selectedObj != nullptr)
		{
			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearDialog();
				gui->addDialogPart("Name", selectedObj->name, "newname");
				gui->showDialog("Set Name", "OK", "Cancel", "setobjname_entered");
			}
		}
		
        engine->setExtraStr("listmenuoptionclicked", "");
    }
	
	if (engine->getExtraInt("setobjname_entered") == 1)
	{
        if (selectedObj != nullptr)
        {
            std::string oldname = selectedObj->name;
		    std::string newname = engine->getExtraStr("newname");
			std::transform(newname.begin(), newname.end(), newname.begin(), ::tolower);

			if (oldname != newname)
			{
				std::map<std::string, Object *> objs = engine->getObjects();

				objs[newname] = objs[oldname];
				objs[newname]->name = newname;
				objs.erase(oldname);

				engine->setObjects(objs);

				engine->setExtraInt("setobjname_entered", 0);
			}
        }
    }
	
	// Set texture

    if (engine->getExtraStr("listmenuoptionclicked") == "Set texture") {
        gui->showFileSelector("png", g_assetsDir);
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "settexture";
    }

    if (engine->getExtraStr("fileselected") != "") {
        if (fileSelectorAction == "settexture") {

			if (verifySourceDir(engine->getExtraStr("fileselected")))
			{
				if (mode == EM_OBJ) {

					if (selectedObj != nullptr) {
						std::string fname = GetFileName(engine->getExtraStr("fileselected"));
						engine->setTexture(selectedObj->name, fname);
						modified = true;
					}
					else if (selectedObj == nullptr) {
						std::string fname = GetFileName(engine->getExtraStr("fileselected"));
						engine->setTexture("objpreview", fname);
						modified = true;
					}

				}
				else if (mode == EM_VOX) {
					std::string fname = GetFileName(engine->getExtraStr("fileselected"));

					if (curVoxels != nullptr) {
						Shape *shape = curVoxels->shape;

						if (shape != nullptr) {
							engine->setVoxelTexture(shape->name, curVoxel, fname);
							shape->needsRebuild = true;
							setVoxPreviewTextures();
							modified = true;
						}
					}
				}
			}

            engine->setExtraStr("fileselected", "");
        }
    }

    // Set position

    if (engine->getExtraStr("listmenuoptionclicked") == "Set position") {

        if (selectedObj != nullptr)
        {
			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearDialog();
				gui->addDialogPart("x", FloatToStr(selectedObj->position.x), "newx");
				gui->addDialogPart("y", FloatToStr(selectedObj->position.y), "newy");
				gui->addDialogPart("z", FloatToStr(selectedObj->position.z), "newz");
				gui->showDialog("Set Position", "OK", "Cancel", "setobjposition_entered");
			}
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    if (engine->getExtraInt("setobjposition_entered") == 1)
    {
        if (selectedObj != nullptr)
        {
            selectedObj->position.x = PLAT_stof(engine->getExtraStr("newx"), 0.0f);
            selectedObj->position.y = PLAT_stof(engine->getExtraStr("newy"), 0.0f);
            selectedObj->position.z = PLAT_stof(engine->getExtraStr("newz"), 0.0f);

            engine->setExtraInt("setobjposition_entered", 0);
        }
    }

    // Set orientation

    if (engine->getExtraStr("listmenuoptionclicked") == "Set orientation") {

        if (selectedObj != nullptr)
        {
			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearDialog();
				gui->addDialogPart("pitch", FloatToStr(selectedObj->pitch), "pitch");
				gui->addDialogPart("yaw", FloatToStr(selectedObj->yaw), "yaw");
				gui->addDialogPart("roll", FloatToStr(selectedObj->roll), "roll");
				gui->showDialog("Set Orientation", "OK", "Cancel", "setobjorientation_entered");
			}
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    if (engine->getExtraInt("setobjorientation_entered") == 1)
    {
        if (selectedObj != nullptr)
        {
            selectedObj->pitch = PLAT_stof(engine->getExtraStr("pitch"), 0.0f);
            selectedObj->yaw = PLAT_stof(engine->getExtraStr("yaw"), 0.0f);
            selectedObj->roll = PLAT_stof(engine->getExtraStr("roll"), 0.0f);
            modified = true;

            engine->setExtraInt("setobjorientation_entered", 0);
        }
    }

    // Set scale

    if (engine->getExtraStr("listmenuoptionclicked") == "Set scale") {

        if (selectedObj != nullptr)
        {
			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearDialog();
				gui->addDialogPart("x", FloatToStr(selectedObj->scale.x), "scalex");
				gui->addDialogPart("y", FloatToStr(selectedObj->scale.y), "scaley");
				gui->addDialogPart("z", FloatToStr(selectedObj->scale.z), "scalez");
				gui->showDialog("Set Scale", "OK", "Cancel", "setobjscale_entered");
			}
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    if (engine->getExtraInt("setobjscale_entered") == 1)
    {
        if (selectedObj != nullptr)
        {
            selectedObj->scale.x = PLAT_stof(engine->getExtraStr("scalex"), 1.0f);
            selectedObj->scale.y = PLAT_stof(engine->getExtraStr("scaley"), 1.0f);
            selectedObj->scale.z = PLAT_stof(engine->getExtraStr("scalez"), 1.0f);
            modified = true;

            engine->setExtraInt("setobjscale_entered", 0);
        }
    }
	
	// Copy

	if (engine->getExtraStr("listmenuoptionclicked") == "Copy"
		|| engine->getExtraStr("listmenuoptionclicked") == "Link copy") {

        if (selectedObj != nullptr)
        {
			if (engine->getExtraStr("listmenuoptionclicked") == "Link copy")
				linkCopy = true;
				
			engine->removeObject("objpreview");
            engine->copyObj(selectedObj->name, "objpreview");
            objPreview = engine->findObj("objpreview");
            objPreview->system = true;
            engine->setObjInt("objpreview", "hasguides", 0);
            engine->setColor("objpreview", 1.0f, 1.0f, 1.0f, 1.0f);
			objPreviewDist = playerObj->distanceTo(selectedObj);
			selectedObj = nullptr;
            selectOnly = false;
            objCopy = true;
			placingObj = true;
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    // Set texture span

    if (engine->getExtraStr("listmenuoptionclicked") == "Set texture span") {

        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

			if (!gui->nonNativeWidgetsShown())
			{
				gui->clearDialog();
				gui->addDialogPart("x", FloatToStr(tex->texSpanX), "texturespanx");
				gui->addDialogPart("y", FloatToStr(tex->texSpanY), "texturespany");
				gui->showDialog("Set Texture Span", "OK", "Cancel", "settexturespan_entered");
			}
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    if (engine->getExtraInt("settexturespan_entered") == 1)
    {
        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

            float tsx = PLAT_stof(engine->getExtraStr("texturespanx"), 1.0f);
            float tsy = PLAT_stof(engine->getExtraStr("texturespany"), 1.0f);

            if (tex != nullptr) {
                engine->setTextureSpan(tex->name, tsx, tsy);
				modified = true;
            }

            // Rebuild all voxel shapes since texture spans changed
            std::map<std::string, Object *> objs = engine->getObjects();

            for (const auto &pair: objs) {
                Object *obj = pair.second;

                if (isVoxels(obj->name))
                {
                    if (obj->shape != nullptr)
                        obj->shape->needsRebuild = true;
                }
            }

            engine->setExtraInt("settexturespan_entered", 0);
        }
    }

    // Set texture light

    if (engine->getExtraStr("listmenuoptionclicked") == "Set texture light") {

        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

            if (!gui->nonNativeWidgetsShown())
            {
                gui->clearDialog();
                gui->addDialogPart("radius (voxels)", FloatToStr(tex->lightRadius), "lightradius");
                gui->addDialogPart("r (0.0-2.0)", FloatToStr(tex->lightr), "lightr");
                gui->addDialogPart("g (0.0-2.0)", FloatToStr(tex->lightg), "lightg");
                gui->addDialogPart("b (0.0-2.0)", FloatToStr(tex->lightb), "lightb");
                gui->showDialog("Set Texture Light", "OK", "Cancel", "settexturelight_entered");
            }
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    if (engine->getExtraInt("settexturelight_entered") == 1)
    {
        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

            float radius = PLAT_stof(engine->getExtraStr("lightradius"), 0.0f);
            float r = PLAT_stof(engine->getExtraStr("lightr"), 1.0f);
            float g = PLAT_stof(engine->getExtraStr("lightg"), 1.0f);
            float b = PLAT_stof(engine->getExtraStr("lightb"), 1.0f);

            if (tex != nullptr) {
                if (radius == 0.0 || (r == 1.0 && g == 1.0 && b == 1.0))
                    tex->lightEnabled = false;
                else
                    tex->lightEnabled = true;
					
                tex->lightRadius = radius;
                tex->lightr = r;
                tex->lightg = g;
                tex->lightb = b;
				
				modified = true;
            }

            // Rebuild all voxel shapes since texture lights changed
            std::map<std::string, Object *> objs = engine->getObjects();

            for (const auto &pair: objs) {
                Object *obj = pair.second;

                if (isVoxels(obj->name))
                {
                    if (obj->shape != nullptr)
                        obj->shape->needsRebuild = true;
                }
            }

            engine->setExtraInt("settexturelight_entered", 0);
        }
    }

    // Light room

    if (engine->getExtraStr("listmenuoptionclicked") == "Light room") {

        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

            if (!gui->nonNativeWidgetsShown())
            {
                gui->clearDialog();
                gui->addDialogPart("r (0.0-2.0)", "1.0", "lightr");
                gui->addDialogPart("g (0.0-2.0)", "1.0", "lightg");
                gui->addDialogPart("b (0.0-2.0)", "1.0", "lightb");
                gui->showDialog("Light Room", "OK", "Cancel", "lightroom_entered");
            }
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    if (engine->getExtraInt("lightroom_entered") == 1)
    {
        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            float r = PLAT_stof(engine->getExtraStr("lightr"), 1.0f);
            float g = PLAT_stof(engine->getExtraStr("lightg"), 1.0f);
            float b = PLAT_stof(engine->getExtraStr("lightb"), 1.0f);

            Voxels *voxels = curVoxels->shape->voxels;

            // Figure out dimensions of the room the cursor is in
            int minx, miny, minz = 0;
            int maxx, maxy, maxz = voxels->getSize();

            // Figure out x room coords
            int x = putvoxx;
            while (x > 0 && voxels->get(x, putvoxy, putvoxz) == 0)
                x--;
            minx = x;

            x = putvoxx;
            while (x < voxels->getSize() && voxels->get(x, putvoxy, putvoxz) == 0)
                x++;
            maxx = x;

            // Figure out y room coords
            int y = putvoxy;
            while (y > 0 && voxels->get(putvoxx, y, putvoxz) == 0)
                y--;
            miny = y;

            y = putvoxy;
            while (y < voxels->getSize() && voxels->get(putvoxx, y, putvoxz) == 0)
                y++;
            maxy = y;

            // Figure out z room coords
            int z = putvoxy;
            while (z > 0 && voxels->get(putvoxx, putvoxy, z) == 0)
                z--;
            minz = z;

            z = putvoxz;
            while (z < voxels->getSize() && voxels->get(putvoxx, putvoxy, z) == 0)
                z++;
            maxz = z;

            // Light the room
            for (int xx = minx + 1; xx <= maxx; xx++)
                for (int yy = miny + 1; yy <= maxy; yy++)
                    for (int zz = minz + 1; zz <= maxz; zz++)
                    {
                        voxels->setrgba(xx, yy, zz, FloatToUChar(r), FloatToUChar(g), FloatToUChar(b), 127);
                    }

            // Make sure voxels are rebuilt
            curVoxels->shape->needsRebuild = true;
            modified = true;
			
            engine->setExtraInt("lightroom_entered", 0);
        }
    }

    // Light voxels

    if (engine->getExtraStr("listmenuoptionclicked") == "Light voxels") {

        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

            if (!gui->nonNativeWidgetsShown())
            {
                gui->clearDialog();
                gui->addDialogPart("r (0.0-2.0)", "1.0", "lightr");
                gui->addDialogPart("g (0.0-2.0)", "1.0", "lightg");
                gui->addDialogPart("b (0.0-2.0)", "1.0", "lightb");
                gui->showDialog("Light Voxels", "OK", "Cancel", "lightvoxels_entered");
            }
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    if (engine->getExtraInt("lightvoxels_entered") == 1)
    {
        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            lightr = PLAT_stof(engine->getExtraStr("lightr"), 1.0f);
            lightg = PLAT_stof(engine->getExtraStr("lightg"), 1.0f);
            lightb = PLAT_stof(engine->getExtraStr("lightb"), 1.0f);

            lightVoxels = true;

            engine->setExtraInt("lightvoxels_entered", 0);
        }
    }

    //
    // Move button
	//

    static int mvTimer = 0;

    if (mvTimer > 0)
        mvTimer--;

    if (engine->getExtraInt("moveclicked") != 0 && mvTimer == 0)
    {

        if (mode == EM_VOX) {
            // Nothing
        } else if (mode == EM_OBJ) {
            if (selectedObj != nullptr && !movingSelected) {
                Object *player = engine->getPlayerObj();
                movingSelected = true;
                engine->setWgColor("move", 1.5, 1.5, 1.5, 1.0);
                distToSelected = player->distanceTo(selectedObj);
                engine->setText("msg", "move selected");
                msgTimer = msgTimerDelay;
            } else if (movingSelected) {
                movingSelected = false;
                engine->setWgColor("move", 1.0, 1.0, 1.0, 1.0);

                engine->setText("msg", "move off");
                msgTimer = msgTimerDelay;
            }

            mvTimer = 30;
        }

        exitScreenShotMode();
    }

	//
    // Mode button
	//

    if (engine->getExtraInt("objmodeclicked") != 0) {

        objPreviewDist = defaultObjPreviewDist;
        mode = EM_OBJ;
        engine->setText("msg2", "object mode");
        msg2Timer = msgTimerDelay;
//        engine->setVisible("voxpreview", false);
//        engine->setVisible("objpreview", true);
        exitScreenShotMode();

        if (lightVoxels)
            lightVoxels = false;

		engine->setExtraInt("objmodeclicked", 0);
    }

//    int aa = g_common.extraInts["voxmodeclicked"];

    if (engine->getExtraInt("voxmodeclicked") != 0) {

        rayLength = defaultRayLength;
        selectOnly = false;
        mode = EM_VOX;
        engine->setText("msg2", "voxel mode");
        msg2Timer = msgTimerDelay;
        engine->setVisible("voxpreview", true);
        engine->setVisible("objpreview", false);
        exitScreenShotMode();

        if (lightVoxels)
            lightVoxels = false;

		engine->setExtraInt("voxmodeclicked", 0);
	}

    //
    // SELECT OBJECT
    //

    // See if we are pointing at an object; select it if so
     if (!movingSelected && mode == EM_OBJ && !screenshotMode && !objCopy) {
        lastSelectedObj = selectedObj;
        selectedObj = nullptr;

        std::map<std::string, Object *> objs = engine->getObjects();

        Object *player = engine->getPlayerObj();

        for (const auto &pair: objs) {
            Object *obj = pair.second;

            if (obj != nullptr && obj != player &&
                obj->system == false/* && player->distanceTo(obj) <= objPreviewDist * 1.2*/) {
                float yawToObj = player->getYawTo(obj);
                float pitchToObj = player->getPitchTo(obj);

/*                while (yawToObj > 360.0)
                    yawToObj -= 360.0;

                while (yawToObj < 0.0)
                    yawToObj += 360.0;

                while (player->yaw > 360.0)
                    player->yaw -= 360.0;

                while (player->yaw < 0.0)
                    player->yaw += 360.0;

                while (pitchToObj > 360.0)
                    pitchToObj -= 360.0;

                while (pitchToObj < 0.0)
                    pitchToObj += 360.0;

                while (player->pitch > 360.0)
                    player->pitch -= 360.0;

                while (player->pitch < 0.0)
                    player->pitch += 360.0;*/

                if (compareAngle(yawToObj, player->yaw, selectAngleThresh)
                    && compareAngle(pitchToObj, player->pitch, selectAngleThresh)) {
                    selectedObj = obj;
                    char msg[1024];
                    snprintf(msg, 1024, "%s selected", selectedObj->name.c_str());
                    engine->setText("msg", msg);
                    msgTimer = 50;
                }
            }
        }

//        if (selectedObj == nullptr) {
//            engine->setText("msg", "");
//        }
    }

    // Guides; they must exist by now so they can be lit up
    tickGuides();

    // Light up selected
    if (selectedObj != nullptr && !screenshotMode) {
        engine->setVisible("objpreview", false);
        selectedObj->color = glm::vec4(1.5, 1.5, 1.5, 1.0);

        // If its voxels, they could be empty,
        // so light up the center guide
        if (isVoxels(selectedObj->name)) {
            std::string gnamec = selectedObj->name + "_guide_center";

            Object *guidec = engine->findObj(gnamec);
            if (guidec != nullptr)
                guidec->color = glm::vec4(1.5, 1.5, 1.5, 0.5);

            lastLitUpGuide = guidec;
        }
    } else if (!selectOnly) {
//        engine->setVisible("objpreview", true);
    }

    if (lastSelectedObj != nullptr && selectedObj != lastSelectedObj) {
        lastSelectedObj->color = glm::vec4(1.0, 1.0, 1.0, 1.0);

        if (lastLitUpGuide != nullptr)
            lastLitUpGuide->color = glm::vec4(1.0, 1.0, 1.0, 1.0);
    }

    // Move object if moving

    if (selectedObj != nullptr && movingSelected) {
        selectedObj->position = player->position;
        selectedObj->setDeltaXYZ(player->yaw, player->pitch, distToSelected);
        selectedObj->move();

		//
		// Snap voxels to nearby voxels
		//

		bool snapX = false;
		bool snapY = false;
		bool snapZ = false;
		float snapPosX = 0.0;
		float snapPosY = 0.0;
		float snapPosZ = 0.0;

		if (isVoxels(selectedObj->name))
		{
			for (const auto &pair: engine->getObjects()) {
				std::string name = pair.first;
				Object *obj = pair.second;

				if (obj != nullptr && !obj->system && isVoxels(obj->name) && obj != selectedObj
					&& obj->distanceTo(selectedObj) < (obj->scale.x + obj->scale.y + obj->scale.z) / 1.5)
				{
					// X

					float gapX = abs(selectedObj->position.x - obj->position.x) - selectedObj->scale.x / 2.0 - obj->scale.x / 2.0;

					if (abs(gapX) < selectedObj->scale.x * snapFactor)
					{
						snapX = true;
						if (snapX)
							if (selectedObj->position.x > obj->position.x)
								snapPosX = obj->position.x + obj->scale.x / 2.0 + selectedObj->scale.x / 2.0;
							else
								snapPosX = obj->position.x - obj->scale.x / 2.0 - selectedObj->scale.x / 2.0;
					}
					else if (abs(selectedObj->position.x - obj->position.x) < selectedObj->scale.x * snapFactor)
					{
						snapX = true;
						if (snapX)
							snapPosX = obj->position.x;
					}

					// Y

					float gapy = abs(selectedObj->position.y - obj->position.y) - selectedObj->scale.y / 2.0 - obj->scale.y / 2.0;

					if (abs(gapy) < selectedObj->scale.y * snapFactor)
					{
						snapY = true;
						if (snapY)
							if (selectedObj->position.y > obj->position.y)
								snapPosY = obj->position.y + obj->scale.y / 2.0 + selectedObj->scale.y / 2.0;
							else
								snapPosY = obj->position.y - obj->scale.y / 2.0 - selectedObj->scale.y / 2.0;
					}
					else if (abs(selectedObj->position.y - obj->position.y) < selectedObj->scale.y * snapFactor)
					{
						snapY = true;
						if (snapY)
							snapPosY = obj->position.y;
					}

					// Z

					float gapz = abs(selectedObj->position.z - obj->position.z) - selectedObj->scale.z / 2.0 - obj->scale.z / 2.0;

					if (abs(gapz) < selectedObj->scale.z * snapFactor)
					{
						snapZ = true;
						if (snapZ)
							if (selectedObj->position.z > obj->position.z)
								snapPosZ = obj->position.z + obj->scale.z / 2.0 + selectedObj->scale.z / 2.0;
							else
								snapPosZ = obj->position.z - obj->scale.z / 2.0 - selectedObj->scale.z / 2.0;
					}
					else if (abs(selectedObj->position.z - obj->position.z) < selectedObj->scale.z * snapFactor)
					{
						snapZ = true;
						if (snapZ)
							snapPosZ = obj->position.z;
					}
				}
			}
		}

		if (snapX)
			selectedObj->position.x = snapPosX;
		if (snapY)
			selectedObj->position.y = snapPosY;
		if (snapZ)
			selectedObj->position.z = snapPosZ;
			
		modified = true;
	}

    // Message indicator
    msgTimer--;
    msg2Timer--;

    if (msgTimer <= 0) {
        engine->setText("msg", "");
    }

    if (msg2Timer <= 0) {
        engine->setText("msg2", "");
    }

    // HACKS

    // Never ever show the other mode's preview

    if (mode == EM_OBJ) {
        engine->setVisible("voxpreview", false);
		if (placingObj)
	        engine->setVisible("objpreview", true);
    }
    if (mode == EM_VOX) {
        engine->setVisible("voxpreview", true);
        engine->setVisible("objpreview", false);
    }
    if (selectOnly || screenshotMode) {
        engine->setVisible("voxpreview", false);
        engine->setVisible("objpreview", false);
    }

    // Never ever show the ugly black sprite as preview

    if (objPreview != nullptr) {
        if (objPreview->type == OBJTYPE_SPRITE && objPreview->textureName == "")
            objPreview->visible = false;
    }
}

/*
========================================
tickMovement
========================================
*/

void Editor::tickMovement()
{
#ifndef PLATFORM_ANDROID
	Object* playerObj = engine->getPlayerObj();
	Controls2* controls = engine->getControls();
	GUI* gui = engine->getGUI();

	if (!gui->nonNativeWidgetsShown())
	{
		if (controls->getKey(GLFW_KEY_UP))
			playerObj->MoveForward(moveSpeed);
		if (controls->getKey(GLFW_KEY_DOWN))
			playerObj->MoveForward(-moveSpeed);
		if (controls->getKey(GLFW_KEY_LEFT))
			playerObj->MoveLeft(moveSpeed);
		if (controls->getKey(GLFW_KEY_RIGHT))
			playerObj->MoveRight(moveSpeed);

		if (controls->getKey(GLFW_KEY_W))
			playerObj->MoveForward(moveSpeed);
		if (controls->getKey(GLFW_KEY_S))
			playerObj->MoveForward(-moveSpeed);
		if (controls->getKey(GLFW_KEY_A))
			playerObj->MoveLeft(moveSpeed);
		if (controls->getKey(GLFW_KEY_D))
			playerObj->MoveRight(moveSpeed);

		if (controls->getKey(GLFW_KEY_Q))
			playerObj->MoveYaw(-turnSpeed);
		if (controls->getKey(GLFW_KEY_E))
			playerObj->MoveYaw(turnSpeed);

		if (controls->getKey(GLFW_KEY_PAGE_UP))
			playerObj->MoveUp(moveSpeed);
		if (controls->getKey(GLFW_KEY_PAGE_DOWN))
			playerObj->MoveUp(-moveSpeed);
	}
#endif
}

/*
========================================
tickGuides
========================================
*/

void Editor::tickGuides() {

	if (g_noGuides || !showGuides)
		return;

    // Go through objects and see if any of them need
    // guides created.

    std::map<std::string, Object *> objs = engine->getObjects();

    for (const auto &pair: objs) {
        std::string name = pair.first;
        Object *obj = pair.second;

        if (obj == nullptr || obj->system)
            continue;

        Shape *shape = obj->shape;

        if (shape == nullptr)
            continue;

        if (shape->voxels != nullptr) {
            std::string gshape = name + "_shape";

            std::string gnameb = name + "_guide_bottom";
            std::string gnamet = name + "_guide_top";
            std::string gnamel = name + "_guide_left";
            std::string gnamer = name + "_guide_right";
            std::string gnamebk = name + "_guide_back";
            std::string gnamef = name + "_guide_front";

            std::string gnamec = name + "_guide_center";

            if (engine->getObjInt(name, "hasguides") != 1) {
                // Create guides
                Log("creating guides");
				
				if (shape->voxels->getSize() == 0)
					continue;

                // Create texture for guides
                engine->genTexture(gshape, "grid", shape->voxels->getSize());

                // Bottom
                engine->addObject(gnameb);
                engine->setType(gnameb, OBJTYPE_SPRITE);
                engine->setTexture(gnameb, gshape);

                engine->setColor(gnameb, 1.0, 1.0, 1.0, 1.0);
                engine->setVisible(gnameb, 1);
                engine->setAlwaysFacePlayer(gnameb, false);
                engine->setSystem(gnameb, true);

                // Top
                engine->addObject(gnamet);
                engine->setType(gnamet, OBJTYPE_SPRITE);
                engine->setTexture(gnamet, gshape);

                engine->setColor(gnamet, 1.0, 1.0, 1.0, 1.0);
                engine->setVisible(gnamet, 1);
                engine->setAlwaysFacePlayer(gnamet, false);
                engine->setSystem(gnamet, true);

                // Left
                engine->addObject(gnamel);
                engine->setType(gnamel, OBJTYPE_SPRITE);
                engine->setTexture(gnamel, gshape);

                engine->setColor(gnamel, 1.0, 1.0, 1.0, 1.0);
                engine->setVisible(gnamel, 1);
                engine->setAlwaysFacePlayer(gnamel, false);
                engine->setSystem(gnamel, true);

                // Right
                engine->addObject(gnamer);
                engine->setType(gnamer, OBJTYPE_SPRITE);
                engine->setTexture(gnamer, gshape);

                engine->setColor(gnamer, 1.0, 1.0, 1.0, 1.0);
                engine->setVisible(gnamer, 1);
                engine->setAlwaysFacePlayer(gnamer, false);
                engine->setSystem(gnamer, true);

                // Back
                engine->addObject(gnamebk);
                engine->setType(gnamebk, OBJTYPE_SPRITE);
                engine->setTexture(gnamebk, gshape);

                engine->setColor(gnamebk, 1.0, 1.0, 1.0, 1.0);
                engine->setVisible(gnamebk, 1);
                engine->setAlwaysFacePlayer(gnamebk, false);
                engine->setSystem(gnamebk, true);

                // Front
                engine->addObject(gnamef);
                engine->setType(gnamef, OBJTYPE_SPRITE);
                engine->setTexture(gnamef, gshape);

                engine->setColor(gnamef, 1.0, 1.0, 1.0, 1.0);
                engine->setVisible(gnamef, 1);
                engine->setAlwaysFacePlayer(gnamef, false);
                engine->setSystem(gnamef, true);

                // Center
                engine->addObject(gnamec);
                engine->setType(gnamec, OBJTYPE_SPRITE);
                engine->setTexture(gnamec, "nextkit.png");

                engine->setColor(gnamec, 1.0, 1.0, 1.0, 1.0);
                engine->setVisible(gnamec, 1);
                engine->setAlwaysFacePlayer(gnamec, true);
                engine->setSystem(gnamec, true);

                // Mark obj as having guides
                engine->setObjInt(name, "hasguides", 1);
            }

            // Get guides
            Object *guideb = engine->findObj(gnameb);
            Object *guidet = engine->findObj(gnamet);
            Object *guidel = engine->findObj(gnamel);
            Object *guider = engine->findObj(gnamer);
            Object *guidebk = engine->findObj(gnamebk);
            Object *guidef = engine->findObj(gnamef);

            Object *guidec = engine->findObj(gnamec);

            // Move guides
            if (guideb != nullptr) {
                guideb->position = obj->position;
                guideb->position.y -= obj->scale.y / 2.0;
                guideb->scale = obj->scale;
                guideb->pitch = 90.0;
            }

            if (guidet != nullptr) {
                guidet->position = obj->position;
                guidet->position.y += obj->scale.y / 2.0;
                guidet->scale = obj->scale;
                guidet->pitch = -90.0;
            }

            if (guidel != nullptr) {
                guidel->position = obj->position;
                guidel->position.x -= obj->scale.x / 2.0;
                guidel->scale = obj->scale;
                guidel->yaw = -90.0;
            }

            if (guider != nullptr) {
                guider->position = obj->position;
                guider->position.x += obj->scale.x / 2.0;
                guider->scale = obj->scale;
                guider->yaw = 90.0;
            }

            if (guidebk != nullptr) {
                guidebk->position = obj->position;
                guidebk->position.z -= obj->scale.z / 2.0;
                guidebk->scale = obj->scale;
                guidebk->yaw = 0.0;
            }

            if (guidef != nullptr) {
                guidef->position = obj->position;
                guidef->position.z += obj->scale.z / 2.0;
                guidef->scale = obj->scale;
                guidef->yaw = 180.0;
            }

            if (guidec != nullptr)
                guidec->position = obj->position;
        }
    }
}

/*
========================================
isVoxels
========================================
*/

bool Editor::isVoxels(std::string objName) {
    Object *obj = engine->findObj(objName);

    if (obj == nullptr)
        return false;

    if (obj->shape != nullptr) {
        if (obj->shape->voxels != nullptr) {
            return true;
        }

        return false;
    }

    return false;
}

/*
========================================
ray
========================================
*/

void Editor::ray(Object *src, float dist, float &x, float &y, float &z) {
    Object dst;

    dst.position = src->position;
    dst.pitch = src->pitch;
    dst.yaw = src->yaw;
    dst.roll = src->roll;

    dst.setDeltaXYZ(src->yaw, src->pitch, dist);
    dst.move();

    x = dst.position.x;
    y = dst.position.y;
    z = dst.position.z;
}

/*
========================================
setVoxPreviewTextures
========================================
*/

void Editor::setVoxPreviewTextures() {
    if (curVoxels == nullptr)
        return;
    if (curVoxels->shape == nullptr)
        return;

    std::string curVoxelsShapeName = curVoxels->shape->name;

    bool changed = false;

    for (int i = 0; i < 255; i++) {
        if (engine->getVoxelTexture(curVoxelsShapeName, i) != engine->getVoxelTexture("voxpreview", i))
        {
            engine->setVoxelTexture("voxpreview", i, engine->getVoxelTexture(curVoxelsShapeName, i));
            changed = true;
        }
    }

    Object *voxPreview = engine->findObj("voxpreview");

    if (voxPreview == nullptr)
        return;
    if (voxPreview->shape == nullptr)
        return;

    if (changed)
        voxPreview->shape->needsRebuild = true;
}

/*
========================================
hideSystemObjects
========================================
*/

void Editor::hideSystemObjects() {
    for (const auto &pair: engine->getObjects()) {
        Object *obj = pair.second;

        if (obj->system)
            obj->visible = false;
    }
}

/*
========================================
showSystemObjects
========================================
*/

void Editor::showSystemObjects() {
    for (const auto &pair: engine->getObjects()) {
        Object *obj = pair.second;

        if (obj->system)
            obj->visible = true;
    }
}

/*
========================================
enterScreenShotMode
========================================
*/

void Editor::enterScreenShotMode() {
    screenshotMode = true;
    engine->getGUI()->hide();
    hideSystemObjects();
}

/*
========================================
exitScreenShotMode
========================================
*/

void Editor::exitScreenShotMode() {

    if (screenshotMode) {
        screenshotMode = false;
        engine->getGUI()->show();
        showSystemObjects();
    }
}

/*
========================================
processHWButtons
========================================
*/

void Editor::processHWButtons()
{
	Controls2 *ctrl = g_engine2->getControls();
	GUI *gui = g_engine2->getGUI();

	static int timer = 0;

	if (timer > 0)
		timer--;

	if (timer == 0)
	{
		if (ctrl->getBtn(BTN_A) || ctrl->getKey(btn2key(BTN_A)) || ctrl->getMouseBtn(0))
		{
			if (gui->nonNativeWidgetsShown())
			{
				gui->enter();
				timer = 50;
			}
			else
				g_engine2->setExtraInt("addclicked", 1);			
		}
		else
			g_engine2->setExtraInt("addclicked", 0);

		if (ctrl->getBtn(BTN_B) || ctrl->getKey(btn2key(BTN_B)) || ctrl->getMouseBtn(1))
		{
			if (gui->nonNativeWidgetsShown())
			{
				gui->clearNonNativeWidgets();
				timer = 50;
			}
			else
				g_engine2->setExtraInt("removeclicked", 1);
		}
		else
			g_engine2->setExtraInt("removeclicked", 0);

		if (ctrl->getBtn(BTN_GUIDE) || ctrl->getKey(btn2key(BTN_GUIDE)))
		{
			g_engine2->setExtraInt("objbtnclicked", 1);
			timer = 20;
		}
		else
			g_engine2->setExtraInt("objbtnclicked", 0);

		if (ctrl->getBtn(BTN_START) || ctrl->getKey(btn2key(BTN_START)))
		{
			g_engine2->setExtraInt("filebtnclicked", 1);
			timer = 20;
		}
		else
			g_engine2->setExtraInt("filebtnclicked", 0);

		if (ctrl->getBtn(BTN_LEFT_BUMPER) || ctrl->getKey(btn2key(BTN_LEFT_BUMPER)))
		{
			g_engine2->setExtraInt("optionsclicked", 1);
			timer = 20;
		}
		else
			g_engine2->setExtraInt("optionsclicked", 0);

		if (mode == EM_OBJ)
		{
			if (ctrl->getBtn(BTN_RIGHT_BUMPER) || ctrl->getKey(btn2key(BTN_RIGHT_BUMPER)))
			{
				g_engine2->setExtraInt("moveclicked", 1);
				timer = 20;
			}
			else
				g_engine2->setExtraInt("moveclicked", 0);
		}

		if (mode == EM_VOX)
		{
			if (ctrl->getBtn(BTN_LEFT_THUMB) || ctrl->getKey(btn2key(BTN_LEFT_THUMB)))
			{
				g_engine2->setExtraInt("prevbtnclicked", 1);
				timer = 25;
			}
			else
				g_engine2->setExtraInt("prevbtnclicked", 0);

			if (ctrl->getBtn(BTN_RIGHT_THUMB) || ctrl->getKey(btn2key(BTN_RIGHT_THUMB)))
			{
				g_engine2->setExtraInt("nextbtnclicked", 1);
				timer = 25;
			}
			else
				g_engine2->setExtraInt("nextbtnclicked", 0);
		}
	}

	if (ctrl->getBtn(BTN_X) || ctrl->getKey(btn2key(BTN_X)))
		engine->setExtraInt("objmodeclicked", 1);

	if (ctrl->getBtn(BTN_Y) || ctrl->getKey(btn2key(BTN_Y)))
		engine->setExtraInt("voxmodeclicked", 1);

	Object *playerObj = g_engine2->getPlayerObj();

	if (ctrl->getBtn(BTN_UP))
	{
		if (gui->nonNativeWidgetsShown())
			gui->up();
		else
			playerObj->MoveForward(0.1);
	}

	if (ctrl->getBtn(BTN_DOWN))
	{
		if (gui->nonNativeWidgetsShown())
			gui->down();
		else
			playerObj->MoveForward(-0.1);
	}

	if (ctrl->getBtn(BTN_LEFT))
	{
		if (gui->nonNativeWidgetsShown())
			gui->left();
		else
			playerObj->MoveLeft(0.1);
	}

	if (ctrl->getBtn(BTN_RIGHT))
	{
		if (gui->nonNativeWidgetsShown())
			gui->right();
		else
			playerObj->MoveRight(0.1);
	}

	// Triggers and mouse wheel control ray length
	float leftTrigger = ctrl->getAxis(AX_LEFT_TRIGGER);
	float rightTrigger = ctrl->getAxis(AX_RIGHT_TRIGGER);
	float mWheel = ctrl->getMouseScroll() * 2;
	ctrl->setMouseScroll(0.0);

	if (mode == EM_VOX)
	{
		if (rayLength > minRayLength)
			rayLength -= (1.0 + leftTrigger) * rayDeltaMultiplier;
		if (rayLength < maxRayLength)
			rayLength += (1.0 + rightTrigger) * rayDeltaMultiplier;

		rayLength += rayDeltaMultiplier * mWheel;
	}

	if (mode == EM_OBJ)
	{
		if (distToSelected > minRayLength)
			distToSelected -= (1.0 + leftTrigger) * rayDeltaMultiplier;
		if (distToSelected < maxRayLength)
			distToSelected += (1.0 + rightTrigger) * rayDeltaMultiplier;

		distToSelected += rayDeltaMultiplier * mWheel;
	}

	if (placingObj)
	{
		if (objPreviewDist > minRayLength) 
			objPreviewDist -= (1.0 + leftTrigger) * rayDeltaMultiplier;
		if (objPreviewDist < maxRayLength)
 			objPreviewDist += (1.0 + rightTrigger) * rayDeltaMultiplier;

		objPreviewDist += rayDeltaMultiplier * mWheel;
	}

	if (gui->nonNativeWidgetsShown())
		ctrl->setEnabled(false);
	else
		ctrl->setEnabled(true);
}

/*
========================================
btn2key
========================================
*/

int Editor::btn2key(int btn)
{
#ifdef PLATFORM_WINDOWS
	if (btn == BTN_A) return GLFW_KEY_ENTER;
	if (btn == BTN_B) return GLFW_KEY_ESCAPE;
	if (btn == BTN_X) return GLFW_KEY_LEFT_BRACKET;
	if (btn == BTN_Y) return GLFW_KEY_RIGHT_BRACKET;
	if (btn == BTN_GUIDE) return GLFW_KEY_INSERT;
	if (btn == BTN_START) return GLFW_KEY_TAB;
	if (btn == BTN_LEFT_BUMPER) return GLFW_KEY_BACKSPACE;
	if (btn == BTN_RIGHT_BUMPER) return GLFW_KEY_BACKSLASH;
    if (btn == BTN_LEFT_THUMB) return GLFW_KEY_MINUS;
	if (btn == BTN_RIGHT_THUMB) return GLFW_KEY_EQUAL;
#endif

	return 0;
}

/*
========================================
processExtraKeys
========================================
*/

void Editor::processExtraKeys()
{
#ifdef PLATFORM_WINDOWS
	Controls2 *ctrl = g_engine2->getControls();

	// Movement back and forward with keys
	Object *player = g_engine2->getPlayerObj();

	if (ctrl->getKey(GLFW_KEY_APOSTROPHE))
		player->MoveForward(0.4);
	if (ctrl->getKey(GLFW_KEY_SLASH))
		player->MoveBackward(0.4);
#endif
}

/*
========================================
worldToVoxelCoords
========================================
*/

void Editor::worldToVoxelCoords(float wx, float wy, float wz, int &x, int &y, int &z)
{
	if (curVoxels == nullptr)
		return;

	float rx = (wx - (curVoxels->position.x - curVoxels->scale.x / 2.0)) /
		((curVoxels->position.x + curVoxels->scale.x / 2.0) -
		(curVoxels->position.x - curVoxels->scale.x / 2.0));
	float ry = (wy - (curVoxels->position.y - curVoxels->scale.y / 2.0)) /
		((curVoxels->position.y + curVoxels->scale.y / 2.0) -
		(curVoxels->position.y - curVoxels->scale.y / 2.0));
	float rz = (wz - (curVoxels->position.z - curVoxels->scale.z / 2.0)) /
		((curVoxels->position.z + curVoxels->scale.z / 2.0) -
		(curVoxels->position.z - curVoxels->scale.z / 2.0));

	int size = curVoxels->shape->voxels->getSize();

	x = (int)((float)size * rx);
	y = (int)((float)size * ry);
	z = (int)((float)size * rz);
}

/*
========================================
verifySourceDir
========================================
*/

bool Editor::verifySourceDir(std::string filename)
{
	std::string newDir = GetPath(filename);

	if (g_assetsDir != newDir)
	{
		engine->setText("msg2", "must use scene dir");
		msgTimer = 150;
		msg2Timer = 150;

		return false;
	}
	else
		return true;
}
