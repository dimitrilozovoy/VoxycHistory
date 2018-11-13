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

#include <sstream>
#include "../engine/Globals.hpp"
#include "Editor.h"
#include "../engine/Engine2.h"
#include "../engine/DDLUtils.hpp"
#include "../engine/platform.h"

//
// init
//

void Editor::init() {

    g_simpleMode = false;
    g_assetsDirExplicit = false;

    g_engine2->init();
    this->engine = g_engine2;

    luaBridge.init(g_engine2);

    engine->setControlsVisible(false);
    engine->setControlScheme(CTRL_EDITOR);
    engine->setHealthBarsVisible(false);
    engine->setSkybox("");

    g_engine2->draw(0);
}

//
// load
//

void Editor::load() {
    // Reset this in case we are coming back to an old instance
    // or resetting state
    selectedObj = nullptr;
    lastSelectedObj = nullptr;
    lastLitUpGuide = nullptr;
    curVoxels = nullptr;

    // Turn off collision detection for player
    Object *player = engine->getPlayerObj();
    player->ints["ignorecollisions"] = 1;

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
    engine->setColor(name, 1.0, 1.0, 1.0, 1.0);
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

    // Top buttons
    engine->addWg("filebtn", WG_BTN, "file.png", "", "filebtnclicked", "", -hw * 5, 0.8, hw, hw);
    engine->addWg("objbtn", WG_BTN, "shapes.png", "", "objbtnclicked", "", -hw * 3, 0.8, hw, hw);
//    engine->addWg("drawbtn",      WG_BTN, "pixels.png",       "", "drawbtnclicked",      "", -hw * 1, 0.8, hw, hw);
//    engine->addWg("scriptbtn",    WG_BTN, "pixels.png",       "", "drawbtnclicked",      "", hw * 1, 0.8, hw, hw);
    engine->addWg("prevbtn", WG_BTN, "prevkit.png", "", "prevbtnclicked", "", hw * 3, 0.8, hw, hw);
    engine->addWg("nextbtn", WG_BTN, "nextkit.png", "", "nextbtnclicked", "", hw * 5, 0.8, hw, hw);

    // Mid buttons
    engine->addWg("remove", WG_BTN, "redx.png", "", "removeclicked", "", -hw * 5, -0.5, hw, hw);
    engine->addWg("options", WG_BTN, "toolbox.png", "", "optionsclicked", "", -hw * 3, -0.5, hw,
                  hw);
    engine->addWg("objmode", WG_BTN, "shapes.png", "", "objmodeclicked", "", -hw * 1, -0.5, hw, hw);
    engine->addWg("voxmode", WG_BTN, "cube.png", "", "voxmodeclicked", "", hw * 1, -0.5, hw, hw);
    engine->addWg("move", WG_BTN, "dpad.png", "", "moveclicked", "", hw * 3, -0.5, hw, hw);
    engine->addWg("add", WG_BTN, "nextkit.png", "", "addclicked", "", hw * 5, -0.5, hw * 1.4,
                  hw * 1.4);

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

    engine->addText("msg", "", 0.0, 0.6, 0.08);
    engine->addText("msg2", "", 0.0, 0.5, 0.08);
/*	engine->addText("debug3", "debug3", 0.0, 0.4, 0.1);
	engine->addText("debug4", "debug4", 0.0, 0.3, 0.1);*/
}

//
// tick
//

void Editor::tick() {
    static int timer = 0;
    static bool refreshPreview = false;
    static std::string fileSelectorAction = "";
    static int msgTimer = 0;
    static int msg2Timer = 0;
    static const int msgTimerDelay = 50;

    // Show framedump if exists
    if (engine->getFrameDump() != "")
        PLAT_ShowLongText(engine->getFrameDump());

    if (engine->getExtraStr("tickscript") != "") {
        luaBridge.exec(engine->getExtraStr("tickscript"));
    }

    Controls2 *ctrl = engine->getControls();
    EditorController *ec = engine->getEditorController();

    //
    // UI CALLBACKS
    //

    if (timer == 0) {
        // File menu

        if (engine->getExtraInt("filebtnclicked") == 1
            || engine->getExtraInt("filebtnclicked") == 2) {
            PLAT_ClearListMenu();
            PLAT_AddListMenuOption("New Scene...", "");
            PLAT_AddListMenuOption("Load Scene...", "");
            PLAT_AddListMenuOption("Save Scene...", "");
            PLAT_AddListMenuOption("Run script...", "");
            PLAT_AddListMenuOption("Clear and run script...", "");
            PLAT_AddListMenuOption("README", "");
            PLAT_AddListMenuOption("Simple Mode", "");
            PLAT_ShowListMenuInDialog("File", "");

            engine->setExtraInt("filebtnclicked", 0);

            exitScreenShotMode();
            timer = 50;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "New Scene...") {
            timer = 50;
            engine->clear();
            engine->clearGUI();
            load();
            engine->setExtraStr("listmenuoptionclicked", "");
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Load Scene...") {
            timer = 50;
            PLAT_ShowFileSelector("sc");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "loadscene";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Save Scene...") {
            timer = 50;
            PLAT_ShowFileSelector("sc");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "savescene";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Import voxel texture...") {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importvoxeltexture";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Import terrain texture...") {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importterraintexture";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Import sky texture...") {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importskytexture";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Import OBJ...") {
            timer = 50;
            PLAT_ShowFileSelector("obj");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importobj";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Export OBJ...") {
            timer = 50;
            PLAT_ShowFileSelector("obj");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "writeobj";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Run script...") {
            timer = 50;
            PLAT_ShowFileSelector("lua");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "runscript";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Clear and run script...") {
            timer = 50;
            PLAT_ShowFileSelector("lua");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "clearandrunscript";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "README") {
            timer = 50;
            std::string t;
            t = t + "Thank you for downloading Voxyc.\n";
            t = t + "This project is open-source.\n";
            t = t + "If there are certain features you would like,\n";
            t = t + "please feel free to contribute.\n";
            t = t + "\n";
            t = t + "GitHub: https://github.com/dimitrilozovoy/VoxycAndroid\n";
            t = t + "Contact e-mail: dimitrilozovoy@gmail.com";

            PLAT_ShowText(t);
            engine->setExtraStr("listmenuoptionclicked", "");
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Simple Mode") {
            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "editorold");
        }

        // Object menu

        if (engine->getExtraInt("objbtnclicked") == 1
            || engine->getExtraInt("objbtnclicked") == 2) {
            PLAT_ClearListMenu();
            PLAT_AddListMenuOption("Select Only", "");
            PLAT_AddListMenuOption("Add Voxels...", "");
            PLAT_AddListMenuOption("Add Shape...", "");
            PLAT_AddListMenuOption("Add Model...", "");
            PLAT_AddListMenuOption("Add Sprite...", "");
            PLAT_AddListMenuOption("Set Sky...", "");
            PLAT_AddListMenuOption("Hide GUI and Guides", "");
            PLAT_ShowListMenuInDialog("Object", "");

            engine->setExtraInt("objbtnclicked", 0);

            exitScreenShotMode();
            timer = 50;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Select Only") {
            selectOnly = true;
            engine->setExtraStr("listmenuoptionclicked", "");
//            timer = 50;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Shape...") {
            PLAT_ClearListMenu();
            PLAT_AddListMenuOption("Add Terrain...", "");
            PLAT_AddListMenuOption("Add Block...", "");
            PLAT_ShowListMenuInDialog("Add Shape", "");

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Model...") {
            timer = 50;
            PLAT_ShowFileSelector("obj");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "addmodel";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Sprite...") {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "addsprite";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Set Sky...") {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "setskybox";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Hide GUI and Guides") {
            enterScreenShotMode();
            engine->setExtraStr("listmenuoptionclicked", "");
        }

        // File selector actions

        if (engine->getExtraStr("fileselected") != "") {
            timer = 50;

            if (fileSelectorAction == "loadscene") {
                engine->clear();
                engine->clearGUI();
                load();
                engine->setAssetsDir(GetPath(engine->getExtraStr("fileselected")));
                engine->loadScene(GetFileName(engine->getExtraStr("fileselected")));
                Log(engine->objectDump());
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "savescene") {
                engine->saveScene(engine->getExtraStr("fileselected"));
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "importvoxeltexture") {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                engine->setVoxelTexture("voxels", ec->getCurTexture(), fname);
                engine->rebuildShape("voxels");
                engine->setVoxelTexture("preview", ec->getCurTexture(), fname);
                engine->rebuildShape("preview");
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "importterraintexture") {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));

                engine->setTexture("terrain", fname);
                engine->setExtraStr("terraintexture", fname);
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "importskytexture") {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                engine->setSkybox(fname);
                engine->setExtraStr("skytexture", fname);
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "writeobj") {
                engine->writeObj("voxels", engine->getExtraStr("fileselected"));
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "runscript") {
                engine->setAssetsDir(GetPath(engine->getExtraStr("fileselected")));
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                luaBridge.exec(fname);
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "clearandrunscript") {
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
                engine->setExtraStr("fileselected", "");

//                engine->DumpFrame();
            }

            if (fileSelectorAction == "addsprite") {
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
                engine->setExtraStr("fileselected", "");
            }

            if (fileSelectorAction == "setskybox") {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                engine->setSkybox(fname);
                engine->setExtraStr("fileselected", "");
            }
        }

        // Add shape actions

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Terrain...") {
            selectOnly = false;
            mode = EM_OBJ;

            engine->newShape("terrain", SHAPE_TERRAIN, 16.0, 4.0, 4.0);

            std::string name = "objpreview";

            engine->setType(name, OBJTYPE_SHAPE);
            engine->setShape(name, "terrain");
            engine->setSize(name, 1000, 25.0, 1000);
            engine->setTexture(name, "grass1.png");
            engine->setAlwaysFacePlayer(name, false);

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Voxels...") {
            selectOnly = false;
            mode = EM_OBJ;

            PLAT_ClearDialog();
            PLAT_AddDialogPart("Size", "16", "newvoxelssize");
            PLAT_ShowDialog("Voxel Parameters", "OK", "Cancel", "voxelparamsselected");

            engine->newShape("voxels", SHAPE_VOXELS, 10.0, 0);

            std::string name = "objpreview";

            engine->setType(name, OBJTYPE_SHAPE);
            engine->setShape(name, "voxels");
            engine->setSize(name, 10, 10, 10);
            engine->setTexture(name, "bluecube.png");
            engine->setAlwaysFacePlayer(name, false);

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Add Block...") {
            selectOnly = false;
            mode = EM_OBJ;

            engine->newShape("block", SHAPE_BLOCK, 1.0, 1.0, 1.0);

            std::string name = "objpreview";

            engine->setType(name, OBJTYPE_SHAPE);
            engine->setShape(name, "block");
            engine->setSize(name, 1, 1, 1);
            engine->setTexture(name, "bluecube.png");
            engine->setAlwaysFacePlayer(name, false);

            engine->setExtraStr("listmenuoptionclicked", "");
            timer = 50;
        }

        // Other buttons

        static int switchVoxTimer = 0;

        if (switchVoxTimer > 0)
            switchVoxTimer--;

        if ((engine->getExtraInt("prevbtnclicked") == 1
             || engine->getExtraInt("prevbtnclicked") == 2)) {
            if (mode == EM_VOX && curVoxel > 0 && switchVoxTimer == 0) {
                curVoxel--;
                switchVoxTimer = 10;
            } else if (mode == EM_OBJ && selectedObj != nullptr) {
                selectedObj->scale *= 0.99;
            } else if (mode == EM_OBJ && selectedObj == nullptr) {
                Object *objPreview = engine->findObj("objpreview");
                if (objPreview != nullptr)
                    objPreview->scale *= 0.99;
            }

            exitScreenShotMode();
            refreshPreview = true;
        }

        if ((engine->getExtraInt("nextbtnclicked") == 1
             || engine->getExtraInt("nextbtnclicked") == 2)) {
            if (mode == EM_VOX && curVoxel < 255 && switchVoxTimer == 0) {
                curVoxel++;
                switchVoxTimer = 10;
            } else if (mode == EM_OBJ && selectedObj != nullptr) {
                selectedObj->scale *= 1.01;
            } else if (mode == EM_OBJ && selectedObj == nullptr) {
                Object *objPreview = engine->findObj("objpreview");
                if (objPreview != nullptr)
                    objPreview->scale *= 1.01;
            }

            exitScreenShotMode();
            refreshPreview = true;
        }

        if ((engine->getExtraInt("camerabtnclicked") == 1
             || engine->getExtraInt("camerabtnclicked") == 2)) {
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
        newVoxelsSize = PLAT_stoi(engine->getExtraStr("newvoxelssize"));
        if (newVoxelsSize <= 0)
            newVoxelsSize = 16;
        if (newVoxelsSize > 32)
            newVoxelsSize = 32;
        engine->setExtraInt("voxelparamsselected", 0);
    }

    //
    // MOVEMENT
    //

    static bool movingLeft = false;
    static bool movingRight = false;
    static bool movingBackward = false;
    static bool movingForward = false;
    static bool movingDown = false;
    static bool movingUp = false;

    // Moving left

    if (engine->getExtraInt("moveleftclicked") == 1
        || engine->getExtraInt("moveleftclicked") == 2) {
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
             || engine->getExtraInt("moverightclicked") == 2) {
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
             || engine->getExtraInt("movebackwardclicked") == 2) {
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
             || engine->getExtraInt("moveforwardclicked") == 2) {
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
             || engine->getExtraInt("movedownclicked") == 2) {
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
             || engine->getExtraInt("moveupclicked") == 2) {
        movingLeft = false;
        movingRight = false;
        movingBackward = false;
        movingForward = false;
        movingDown = false;
        movingUp = true;
        exitScreenShotMode();
    }

    // Stop if finger up

    if (engine->getExtraInt("touchaction1") == 3
        || engine->getExtraInt("touchaction2") == 3) {
        movingLeft = false;
        movingRight = false;
        movingBackward = false;
        movingForward = false;
        movingDown = false;
        movingUp = false;
    }

    if (engine->getExtraInt("movedownclicked") == 3)
        movingDown = false;
    if (engine->getExtraInt("moveleftclicked") == 3)
        movingLeft = false;
    if (engine->getExtraInt("moverightclicked") == 3)
        movingRight = false;
    if (engine->getExtraInt("movebackwardclicked") == 3)
        movingBackward = false;
    if (engine->getExtraInt("moveforwardclicked") == 3)
        movingForward = false;
    if (engine->getExtraInt("moveupclicked") == 3)
        movingUp = false;

    if (movingDown)
        ctrl->MoveDown();

    if (movingLeft)
        ctrl->setBtn(BTN_LEFT, 1);
	else
		ctrl->setBtn(BTN_LEFT, 0);

	if (movingRight)
		ctrl->setBtn(BTN_RIGHT, 1);
	else
		ctrl->setBtn(BTN_RIGHT, 0);
	
	if (movingBackward)
		ctrl->setBtn(BTN_DOWN, 1);
	else
		ctrl->setBtn(BTN_DOWN, 0);

    if (movingForward)
		ctrl->setBtn(BTN_UP, 1);
	else
		ctrl->setBtn(BTN_UP, 0);

    if (movingUp)
        ctrl->MoveUp();

    //
    // PREVIEW
    //

    int putvoxx = 0;
    int putvoxy = 0;
    int putvoxz = 0;

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

        ray(player, 20.0, wx, wy, wz);

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

            int x = (int) ((float) size * rx);
            int y = (int) ((float) size * ry);
            int z = (int) ((float) size * rz);

            if (x >= 0 && x < size
                && y >= 0 && y < size
                && z >= 0 && z < size) {
                engine->setVoxel("voxpreview", x, y, z, curVoxel);

                putvoxx = x;
                putvoxy = y;
                putvoxz = z;
            }

            previewShape->needsRebuild = true;
            refreshPreview = false;
            refreshPreviewTimer = 0;
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

    static int addTimer = 0;

    if (addTimer > 0)
        addTimer--;

    static int placeObjTimer = 0;

    if (placeObjTimer > 0)
        placeObjTimer--;

    if ((engine->getExtraInt("addclicked") == 1
         || engine->getExtraInt("addclicked") == 2)
        && addTimer == 0 && !selectOnly) {
        if (mode == EM_VOX && curVoxels != nullptr) {

            //
            // Place voxel
            //

            Shape *shape = curVoxels->shape;
            engine->setVoxel(shape->name, putvoxx, putvoxy, putvoxz, curVoxel);

            shape->needsRebuild = true;
        } else if (mode == EM_OBJ) {

            //
            // Place object
            //

            if (placeObjTimer == 0) {
                char newNamec[1024];
                snprintf(newNamec, 1024, "obj%d", RandomInt(0, 10000));
                std::string newName = std::string(newNamec);

                engine->copyObj("objpreview", std::string(newName));
                Object *newObj = engine->findObj(newName);
                newObj->name = newName;
                newObj->system = false;

                Shape *shape = newObj->shape;

                // Added voxels? Create new voxel shape for it
                if (shape != nullptr && shape->voxels != nullptr && !linkCopy) {
                    engine->newShape(newName, SHAPE_VOXELS, newVoxelsSize, 0.0);
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
                if (shape != nullptr && shape->voxels != nullptr && !linkCopy) {
                    // Swtich to voxel mode
                    mode = EM_VOX;
                    engine->setText("msg2", "voxel mode");
                    msg2Timer = msgTimerDelay;
                }

                newObj->visible = true;

                char msg[1024];
                snprintf(msg, 1024, "%s placed", newName.c_str());
                engine->setText("msg", msg);
                msgTimer = msgTimerDelay;
                Log(msg);

                placeObjTimer = 10;
            }

            linkCopy = false;
        }

        exitScreenShotMode();
    }

	//
    // Remove button
	//

    static int rmTimer = 0;

    if (rmTimer > 0)
        rmTimer--;

    if ((engine->getExtraInt("removeclicked") == 1
         || engine->getExtraInt("removeclicked") == 2)
        && rmTimer == 0) {
        if (mode == EM_VOX) {
            // Delete voxel
            if (curVoxels != nullptr) {
                Shape *shape = curVoxels->shape;
                engine->setVoxel(shape->name, putvoxx, putvoxy, putvoxz, 0);

                shape->needsRebuild = true;

                rmTimer = 30;
            }
        } else if (mode == EM_OBJ) {
            // Delete object
            if (selectedObj != nullptr) {
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
            }

            rmTimer = 30;
        }

        exitScreenShotMode();
    }

	//
    // Options button
	//

    static int optTimer = 0;

    if (optTimer > 0)
        optTimer--;

    if ((engine->getExtraInt("optionsclicked") == 1
         || engine->getExtraInt("optionsclicked") == 2)
        && optTimer == 0) {
        if (mode == EM_OBJ) {
            PLAT_ClearListMenu();
            PLAT_AddListMenuOption("Set name...", "");
            PLAT_AddListMenuOption("Set texture...", "");
            PLAT_AddListMenuOption("Set position...", "");
			PLAT_AddListMenuOption("Set orientation...", "");
			PLAT_AddListMenuOption("Set scale...", "");
			PLAT_AddListMenuOption("Link copy", "");
            PLAT_ShowListMenuInDialog("Options", "");

            optTimer = 30;
        } else if (mode == EM_VOX) {
            PLAT_ClearListMenu();
            PLAT_AddListMenuOption("Set texture...", "");
            PLAT_AddListMenuOption("Set texture span...", "");
            PLAT_ShowListMenuInDialog("Options", "");

            optTimer = 30;
        }

        exitScreenShotMode();
    }
	
	// Set name
	
    if (engine->getExtraStr("listmenuoptionclicked") == "Set name...") {
		
		if (selectedObj != nullptr)
		{
            PLAT_ClearDialog();
            PLAT_AddDialogPart("Name", selectedObj->name, "newname");
            PLAT_ShowDialog("Set Name", "OK", "Cancel", "setobjname_entered");
		}
		
        engine->setExtraStr("listmenuoptionclicked", "");
    }
	
	if (engine->getExtraInt("setobjname_entered") == 1)
	{
        if (selectedObj != nullptr)
        {
            std::string oldname = selectedObj->name;
            std::string newname = engine->getExtraStr("newname");

            std::map<std::string, Object *> objs = engine->getObjects();

    		objs[newname] = objs[oldname];
            objs[newname]->name = newname;
            objs.erase(oldname);

            engine->setObjects(objs);

            engine->setExtraInt("setobjname_entered", 0);
        }
    }
	
	// Set texture

    if (engine->getExtraStr("listmenuoptionclicked") == "Set texture...") {
        PLAT_ShowFileSelector("png");
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "settexture";
    }

    if (engine->getExtraStr("fileselected") != "") {
        if (fileSelectorAction == "settexture") {
            if (mode == EM_OBJ) {
                if (selectedObj != nullptr) {
                    std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                    engine->setTexture(selectedObj->name, fname);
                } else if (selectedObj == nullptr) {
                    std::string fname = GetFileName(engine->getExtraStr("fileselected"));
                    engine->setTexture("objpreview", fname);
                }

            } else if (mode == EM_VOX) {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));

                if (curVoxels != nullptr) {
                    Shape *shape = curVoxels->shape;

                    if (shape != nullptr) {
                        engine->setVoxelTexture(shape->name, curVoxel, fname);
                        shape->needsRebuild = true;
                        setVoxPreviewTextures();
                    }
                }
            }

            engine->setExtraStr("fileselected", "");
        }
    }

    // Set position

    if (engine->getExtraStr("listmenuoptionclicked") == "Set position...") {

        if (selectedObj != nullptr)
        {
            PLAT_ClearDialog();

            PLAT_AddDialogPart("x", FloatToStr(selectedObj->position.x), "newx");
            PLAT_AddDialogPart("y", FloatToStr(selectedObj->position.y), "newy");
            PLAT_AddDialogPart("z", FloatToStr(selectedObj->position.z), "newz");

            PLAT_ShowDialog("Set Position", "OK", "Cancel", "setobjposition_entered");
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

    if (engine->getExtraStr("listmenuoptionclicked") == "Set orientation...") {

        if (selectedObj != nullptr)
        {
            PLAT_ClearDialog();

            PLAT_AddDialogPart("pitch", FloatToStr(selectedObj->pitch), "pitch");
            PLAT_AddDialogPart("yaw", FloatToStr(selectedObj->yaw), "yaw");
            PLAT_AddDialogPart("roll", FloatToStr(selectedObj->roll), "roll");

            PLAT_ShowDialog("Set Orientation", "OK", "Cancel", "setobjorientation_entered");
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

            engine->setExtraInt("setobjorientation_entered", 0);
        }
    }

    // Set scale

    if (engine->getExtraStr("listmenuoptionclicked") == "Set scale...") {

        if (selectedObj != nullptr)
        {
            PLAT_ClearDialog();

            PLAT_AddDialogPart("x", FloatToStr(selectedObj->scale.x), "scalex");
            PLAT_AddDialogPart("y", FloatToStr(selectedObj->scale.y), "scaley");
            PLAT_AddDialogPart("z", FloatToStr(selectedObj->scale.z), "scalez");

            PLAT_ShowDialog("Set Scale", "OK", "Cancel", "setobjscale_entered");
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

            engine->setExtraInt("setobjscale_entered", 0);
        }
    }

    // Link copy

    if (engine->getExtraStr("listmenuoptionclicked") == "Link copy") {

        if (selectedObj != nullptr)
        {
            engine->removeObject("objpreview");
            engine->copyObj(selectedObj->name, "objpreview");
            objPreview = engine->findObj("objpreview");
            objPreview->system = true;
            engine->setObjInt("objpreview", "hasguides", 0);
            engine->setColor("objpreview", 1.0f, 1.0f, 1.0f, 1.0f);
            selectedObj = nullptr;
            selectOnly = false;
            linkCopy = true;
        }

        engine->setExtraStr("listmenuoptionclicked", "");
    }

    // Set texture span

    if (engine->getExtraStr("listmenuoptionclicked") == "Set texture span...") {

        if (curVoxels != nullptr && curVoxels->shape != nullptr)
        {
            std::string curVoxelsShapeName = curVoxels->shape->name;

            TextureManager2 *texMan = engine->getTextureManager();
            Texture *tex = texMan->find(engine->getVoxelTexture(curVoxelsShapeName, curVoxel));

            PLAT_ClearDialog();

            PLAT_AddDialogPart("x", FloatToStr(tex->texSpanX), "texturespanx");
            PLAT_AddDialogPart("y", FloatToStr(tex->texSpanY), "texturespany");

            PLAT_ShowDialog("Set Texture Span", "OK", "Cancel", "settexturespan_entered");
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

    //
    // Move button
	//

    static int mvTimer = 0;

    if (mvTimer > 0)
        mvTimer--;

    if ((engine->getExtraInt("moveclicked") == 1
         || engine->getExtraInt("moveclicked") == 2)
        && mvTimer == 0) {

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

    if ((engine->getExtraInt("objmodeclicked") == 1
         || engine->getExtraInt("objmodeclicked") == 2)) {
        mode = EM_OBJ;
        engine->setText("msg2", "object mode");
        msg2Timer = msgTimerDelay;
        engine->setVisible("voxpreview", false);
        engine->setVisible("objpreview", true);
        exitScreenShotMode();
    }

    if ((engine->getExtraInt("voxmodeclicked") == 1
         || engine->getExtraInt("voxmodeclicked") == 2)) {
        selectOnly = false;
        mode = EM_VOX;
        engine->setText("msg2", "voxel mode");
        msg2Timer = msgTimerDelay;
        engine->setVisible("voxpreview", true);
        engine->setVisible("objpreview", false);
        exitScreenShotMode();
    }

    //
    // SELECT OBJECT
    //

    // See if we are pointing at an object; select it if so
    if (!movingSelected && mode == EM_OBJ && !screenshotMode && !linkCopy) {
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

                while (yawToObj > 360.0)
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
                    player->pitch += 360.0;

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

        if (selectedObj == nullptr) {
            engine->setText("msg", "");
        }
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
        engine->setVisible("objpreview", true);
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

//
// tickGuides
//

void Editor::tickGuides() {
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

void Editor::setVoxPreviewTextures() {
    if (curVoxels == nullptr)
        return;
    if (curVoxels->shape == nullptr)
        return;

    std::string curVoxelsShapeName = curVoxels->shape->name;

    for (int i = 0; i < 255; i++) {
        engine->setVoxelTexture("voxpreview", i, engine->getVoxelTexture(curVoxelsShapeName, i));
    }

    Object *voxPreview = engine->findObj("voxpreview");

    if (voxPreview == nullptr)
        return;
    if (voxPreview->shape == nullptr)
        return;

    voxPreview->shape->needsRebuild = true;
}

void Editor::hideSystemObjects() {
    for (const auto &pair: engine->getObjects()) {
        Object *obj = pair.second;

        if (obj->system)
            obj->visible = false;
    }
}

void Editor::showSystemObjects() {
    for (const auto &pair: engine->getObjects()) {
        Object *obj = pair.second;

        if (obj->system)
            obj->visible = true;
    }
}

void Editor::enterScreenShotMode() {
    screenshotMode = true;
    engine->getGUI()->hide();
    hideSystemObjects();
}

void Editor::exitScreenShotMode() {
    if (screenshotMode) {
        screenshotMode = false;
        engine->getGUI()->show();
        showSystemObjects();
    }
}
