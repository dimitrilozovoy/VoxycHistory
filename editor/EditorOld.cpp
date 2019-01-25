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

#include "EditorOld.h"
#include "../engine/Engine2.h"
#include "../engine/DDLUtils.hpp"
#include "../engine/platform.h"

void EditorOld::init()
{
    g_simpleMode = true;

	g_engine2->init();
	
	this->engine = g_engine2;
	
	luaBridge.init(g_engine2);

    engine->useLegacyTextureSpan(true);
	engine->setControlScheme(CTRL_EDITOR);
	engine->setControlsVisible(false);
	engine->setHealthBarsVisible(false);
	
	float voxelsWorldSize = 320.0;
	int voxelsSize = 128;

	// Text
//	engine->addText("testtext", "testtext", 0.0, 0.5, 0.1);
	
	engine->setSkybox("skybox.png");
	engine->setExtraStr("skytexture", "skybox.png");

//	engine->setSun(glm::vec4(10.0, 10.0, 10.0, 0.0), -45.0, -45.0, 0.0);
	
	// Grid
	for (int i = 0; i < 5; i++)
	{
	std::string name = "grid" + i;
    engine->addObject(name);
    engine->setType(name, OBJTYPE_SPRITE);
	engine->setTexture(name, "1.png");
//    engine->setShape(name, "block");
    engine->setPos(name, 0.0 + (float)i, 0.0, -5.0);
//	engine->setOrientation("grid", 0.0, 0.0, 0.0);
    engine->setSize(name, 1.0, 1.0, 1.0);
    engine->setColor(name, 0.3, 0.3, 0.3, 1.0);
	engine->setAlwaysFacePlayer(name, true);
	engine->setVisible(name, true);
	}
	
    // Terrain
    engine->newShape("terrain", SHAPE_TERRAIN, 16.0, 4.0, 4.0);

    engine->addObject("terrain");
    engine->setType("terrain", OBJTYPE_SHAPE);
    engine->setShape("terrain", "terrain");
    engine->setPos("terrain", 0.0, -(voxelsWorldSize / 2) + (voxelsWorldSize / voxelsSize / 4.0), 0.0);
    engine->setSize("terrain", voxelsWorldSize * 4, 25.0, voxelsWorldSize * 4);
    engine->setColor("terrain", 1.0, 1.0, 1.0, 1.0);
    engine->setTexture("terrain", "grass1.png");
    engine->setExtraStr("terraintexture", "grass1.png");
	
	// Voxels
	engine->newShape("voxels", SHAPE_VOXELS, voxelsSize, 0.0);
    engine->addObject("voxels");
    engine->setType("voxels", OBJTYPE_SHAPE);
    engine->setShape("voxels", "voxels");

	engine->setPos("voxels", 0.0, 0.0, 0.0);
    engine->setSize("voxels", voxelsWorldSize, voxelsWorldSize, voxelsWorldSize);
    engine->setColor("voxels", 1.0, 1.0, 1.0, 1.0);
	engine->setTextureSpan("voxels", 8.0, 8.0);

	// Preview
	
	engine->newShape("preview", SHAPE_VOXELS, voxelsSize, 0.0);
    engine->addObject("preview");
    engine->setType("preview", OBJTYPE_SHAPE);
    engine->setShape("preview", "preview");

	engine->setPos("preview", 0.0, 0.0, 0.0);
    engine->setSize("preview", voxelsWorldSize, voxelsWorldSize, voxelsWorldSize);
    engine->setColor("preview", 1.0, 1.0, 1.0, 1.0);
	engine->setTextureSpan("preview", 8.0, 8.0);
	
	engine->setColor("preview", 1.0, 1.0, 1.0, 0.5);

    engine->setVoxelTexture("voxels", 1, "brick3.png");
	engine->setVoxelTexture("voxels", 2, "brick4.png");
	engine->setVoxelTexture("voxels", 3, "concrete1.png");
	engine->setVoxelTexture("voxels", 4, "tile.png");
    engine->setVoxelTexture("voxels", 5, "stone1.png");
	engine->setVoxelTexture("voxels", 6, "stone2.png");
	engine->setVoxelTexture("voxels", 7, "beigestone.png");
	engine->setVoxelTexture("voxels", 8, "asphalt.png");
    engine->setVoxelTexture("voxels", 9, "graychips.png");
	engine->setVoxelTexture("voxels", 10, "leaves.png");

    engine->setVoxelTexture("preview", 1, "brick3.png");
    engine->setVoxelTexture("preview", 2, "brick4.png");
    engine->setVoxelTexture("preview", 3, "concrete1.png");
    engine->setVoxelTexture("preview", 4, "tile.png");
    engine->setVoxelTexture("preview", 5, "stone1.png");
    engine->setVoxelTexture("preview", 6, "stone2.png");
    engine->setVoxelTexture("preview", 7, "beigestone.png");
    engine->setVoxelTexture("preview", 8, "asphalt.png");
    engine->setVoxelTexture("preview", 9, "graychips.png");
    engine->setVoxelTexture("preview", 10, "leaves.png");

    float ts = 2.0;

    engine->setTextureSpan("brick3.png", ts, ts);
    engine->setTextureSpan("brick4.png", ts, ts);
    engine->setTextureSpan("concrete1.png", ts, ts);
    engine->setTextureSpan("tile.png", ts, ts);
    engine->setTextureSpan("stone1.png", ts, ts);
    engine->setTextureSpan("stone2.png", ts, ts);
    engine->setTextureSpan("beigestone.png", ts, ts);
    engine->setTextureSpan("asphalt.png", ts, ts);
    engine->setTextureSpan("graychips.png", ts, ts);
    engine->setTextureSpan("leaves.png", ts, ts);

//	engine->setVoxel("voxels", 0, 0, 0, 0);

	engine->setEditVoxelsObj("voxels");
	
	engine->setPlayerPos(0, -(voxelsWorldSize / 2) + (voxelsWorldSize / voxelsSize * 2), 0);

	int numbtns = 6;
	float bw = 0.3;
	float hbw = 0.15;
	float bs = 0.25;
	float bsv = 0.25;
	float hw = 1.0 / numbtns;
	
//	engine->addWg("filebtn", WG_BTN, "file.png", "", "filebtnclicked", "", -(hbw + bw * 3), 0.6, 0.15, 0.15);
	engine->addWg("filebtn", WG_BTN, "file.png", "", "filebtnclicked", "",  -(hw * 5), 0.8, hw, hw);
	engine->addWg("objbtn", WG_BTN, "shapes.png", "", "objbtnclicked", "",  -(hbw * 3), 0.8, hw, hw);
/*	engine->addWg("orthoview", WG_BTN, "cube.png", "", "", "", -hbw, 0.8, bs, bsv);
    engine->addWg("textureeditor", WG_BTN, "cube.png", "", "", "", hbw, 0.8, bs, bsv);*/
//    engine->addWg("camerabtn", WG_BTN, "camera.png", "", "camerabtnclicked", "", -hw * 1.0, 0.8, hw, hw);
	engine->addWg("prevbtn", WG_BTN, "prevkit.png", "", "prevbtnclicked", "", hw * 3, 0.8, hw, hw);
	engine->addWg("nextbtn", WG_BTN, "nextkit.png", "", "nextbtnclicked", "", hw * 5, 0.8, hw, hw);
//	engine->addWg("help", WG_BTN, "cube.png", "", "", "", hbw + bw * 3, 0.6, 0.15, 0.15);

//	engine->addWg("undo", WG_BTN, "cube.png", "", "", "",  -(hbw + bw * 3), -0.8, bs, bs);
	engine->addWg("moveleft", WG_BTN, "leftarrow.png", "", "moveleftclicked", "", -hw * 5/*-(hbw + bw * 2)*/, -0.8, hw, hw);
	engine->addWg("moveright", WG_BTN, "rightarrow.png", "", "moverightclicked", "", -hw * 3, -0.8, hw, hw);
	engine->addWg("movedown", WG_BTN, "downtriangle.png", "", "movedownclicked", "", -hw * 1, -0.8, hw, hw);
    engine->addWg("moveup", WG_BTN, "uptriangle.png", "", "moveupclicked", "", hw * 1, -0.8, hw, hw);
	engine->addWg("moveforward", WG_BTN, "uparrow.png", "", "moveforwardclicked", "", hw * 3, -0.8, hw, hw);
	engine->addWg("movebackward", WG_BTN, "downarrow.png", "", "movebackwardclicked", "", hw * 5, -0.8, hw, hw);
//	engine->addWg("toolbox", WG_BTN, "cube.png", "", "", "", (hbw + bw * 3), -0.8, bs, bs);

	engine->addWg("add", WG_BTN, "cube.png", "", "addclicked", "", hbw + bw * 2, -0.4, hw, hw);
	engine->addWg("remove", WG_BTN, "redx.png", "", "removeclicked", "", -(hbw + bw * 2), -0.4, hw, hw);
	
	engine->addWg("filemenu", WG_MENU, "bkbrick3.png", "", "filemenuclicked", "filemenu", 0.0, 0.0, 0.6, 0.6);
	engine->addWg("loadfile", WG_MENUITEM, "", "load", "loadclicked", "filemenu", 0.0, 0.2, 0.1, 0.1);
    engine->addWg("savefile", WG_MENUITEM, "", "save", "saveclicked", "filemenu", 0.0, -0.2, 0.1, 0.1);
	engine->setWgVisible("filemenu", false);
}

void EditorOld::load()
{
}

void EditorOld::tick()
{
	static int menuVisible = 0;
	static int timer = 0;
	static bool refreshPreview = false;
	static std::string fileSelectorAction = "";

	Controls2 *ctrl = engine->getControls();
	EditorController *ec = engine->getEditorController();
	
	// Run tick script
    std::string fname;

	if (engine->getExtraStr("tickscript") != "")
	{
		luaBridge.exec(engine->getExtraStr("tickscript"));
	}
//	else
//	{
	
    if (timer == 0)
	{
	    if (engine->getExtraInt("filebtnclicked") == 1
		|| engine->getExtraInt("filebtnclicked") == 2)
	    {
			PLAT_ClearListMenu();
			PLAT_AddListMenuOption("Load...", "");
			PLAT_AddListMenuOption("Save...", "");
		    PLAT_AddListMenuOption("Export OBJ...", "");
			PLAT_AddListMenuOption("Run script...", "");
			PLAT_AddListMenuOption("README", "");
            PLAT_AddListMenuOption("Advanced mode", "");
			PLAT_ShowListMenuInDialog("File", "");
					
		    engine->setExtraInt("filebtnclicked", 0);
			
			timer = 50;
	    }
		
	    if (engine->getExtraInt("objbtnclicked") == 1
		|| engine->getExtraInt("objbtnclicked") == 2)
	    {
			PLAT_ClearListMenu();
		    PLAT_AddListMenuOption("Import voxel texture...", "");
            PLAT_AddListMenuOption("Import terrain texture...", "");
	        PLAT_AddListMenuOption("Import sky texture...", "");
			PLAT_ShowListMenuInDialog("Object", "");
			
		    engine->setExtraInt("objbtnclicked", 0);
			
			timer = 50;
	    }
		
	    if (engine->getExtraStr("listmenuoptionclicked") == "Load...")
	    {
			timer = 50;
			PLAT_ShowFileSelector("vx");
		    engine->setExtraStr("listmenuoptionclicked", "");
			fileSelectorAction = "load";
	    }
		
	    if (engine->getExtraStr("listmenuoptionclicked") == "Save...")
	    {
			timer = 50;
			PLAT_ShowFileSelector("vx");
		    engine->setExtraStr("listmenuoptionclicked", "");
			fileSelectorAction = "save";
	    }
		
/*	    if (engine->getExtraStr("listmenuoptionclicked") == "Save As..")
	    {
			timer = 100;
		    engine->setExtraStr("listmenuoptionclicked", "");
//			engine->save();
			PLAT_ShowFileSelector();
	    }*/

        if (engine->getExtraStr("listmenuoptionclicked") == "Import voxel texture...")
        {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importvoxeltexture";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Import terrain texture...")
        {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importterraintexture";
        }

        if (engine->getExtraStr("listmenuoptionclicked") == "Import sky texture...")
        {
            timer = 50;
            PLAT_ShowFileSelector("png");
            engine->setExtraStr("listmenuoptionclicked", "");
            fileSelectorAction = "importskytexture";
        }
		
		if (engine->getExtraStr("listmenuoptionclicked") == "Import OBJ...")
		{
			timer = 50;
			PLAT_ShowFileSelector("obj");
			engine->setExtraStr("listmenuoptionclicked", "");
			fileSelectorAction = "importobj";
		}

        if (engine->getExtraStr("listmenuoptionclicked") == "Export OBJ...")
		{
			timer = 50;
			PLAT_ShowFileSelector("obj");
			engine->setExtraStr("listmenuoptionclicked", "");
			fileSelectorAction = "writeobj";
		}
		
		if (engine->getExtraStr("listmenuoptionclicked") == "Run script...")
		{
			timer = 50;
			PLAT_ShowFileSelector("lua");
			engine->setExtraStr("listmenuoptionclicked", "");
			fileSelectorAction = "runscript";
		}
		
		if (engine->getExtraStr("listmenuoptionclicked") == "README")
		{
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

        if (engine->getExtraStr("listmenuoptionclicked") == "Advanced mode") {
            engine->setExtraInt("switchmodule", 1);
			engine->setExtraStr("nextmodule", "editor");
		}

	    if (engine->getExtraStr("fileselected") != "")
	    {
			timer = 50;
			
			if (fileSelectorAction == "load")
			{
			    engine->load(engine->getExtraStr("fileselected"));
				engine->setTexture("terrain", engine->getExtraStr("terraintexture"));
				engine->setSkybox(engine->getExtraStr("skytexture"));
			}
			
			if (fileSelectorAction == "save")
			{
			    engine->save(engine->getExtraStr("fileselected"));
			}

            if (fileSelectorAction == "importvoxeltexture")
            {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));

                engine->setVoxelTexture("voxels", ec->getCurTexture(), fname);
                engine->rebuildShape("voxels");
                engine->setVoxelTexture("preview", ec->getCurTexture(), fname);
                engine->rebuildShape("preview");
            }
			
			if (fileSelectorAction == "importterraintexture")
            {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));

				engine->setTexture("terrain", fname);
				engine->setExtraStr("terraintexture", fname);
            }

            if (fileSelectorAction == "importskytexture")
            {
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));

				engine->setSkybox(fname);
				engine->setExtraStr("skytexture", fname);
            }

            if (fileSelectorAction == "writeobj")
            {
                engine->writeObj("voxels", engine->getExtraStr("fileselected"));
            }
			
			if (fileSelectorAction == "runscript")
            {
//				engine->clearGUI();
//				engine->clear();
//				engine->setPlayerPos(0.0, 0.0, 0.0);
                std::string fname = GetFileName(engine->getExtraStr("fileselected"));
				luaBridge.exec(fname);
            }
			
            if (fileSelectorAction == "importobj")
            {
                // engine->getExtraStr("fileselected"));
            }

			engine->setExtraStr("fileselected", "");
	    }
		
		if ((engine->getExtraInt("prevbtnclicked") == 1
        || engine->getExtraInt("prevbtnclicked") == 2))
        {
	        ec->minus();
			refreshPreview = true;
        }
		
		if ((engine->getExtraInt("nextbtnclicked") == 1
        || engine->getExtraInt("nextbtnclicked") == 2))
        {
	        ec->plus();
			refreshPreview = true;
        }
		
		if ((engine->getExtraInt("camerabtnclicked") == 1
        || engine->getExtraInt("camerabtnclicked") == 2))
        {
	        std::string fname = PLAT_GetCameraPic();
			
			engine->setExtraStr("camerapicfilename", fname);
			
//			Log(fname);
            engine->setExtraInt("camerabtnclicked", 0);

            timer = 50;
        }
		
		if (engine->getExtraInt("camerapicready") == 1)
		{
			engine->setVoxelTexture("voxels", ec->getCurTexture(), engine->getExtraStr("camerapicfilename"));
			engine->rebuildShape("voxels");
			engine->setExtraInt("camerapicready", 0);
		}
	}
	else
		timer--;
		
   static bool movingLeft = false;
   static bool movingRight = false;
   static bool movingBackward = false;
   static bool movingForward = false;
   static bool movingDown = false;
   static bool movingUp = false;
   
   // Moving left
		
   if (engine->getExtraInt("moveleftclicked") == 1
       || engine->getExtraInt("moveleftclicked") == 2)
   {
       movingLeft = true;
       movingRight = false;
       movingBackward = false;
       movingForward = false;
       movingDown = false;
       movingUp = false;
   }
   
   // Moving right
   
   else if (engine->getExtraInt("moverightclicked") == 1
   || engine->getExtraInt("moverightclicked") == 2)
   {
       movingLeft = false;
       movingRight = true;
       movingBackward = false;
       movingForward = false;
       movingDown = false;
       movingUp = false;   
   }
   
   // Moving backward
		
   else if (engine->getExtraInt("movebackwardclicked") == 1
   || engine->getExtraInt("movebackwardclicked") == 2)
   {
       movingLeft = false;
       movingRight = false;
       movingBackward = true;
       movingForward = false;
       movingDown = false;
       movingUp = false;
    }
   
   // Moving forward

   else if (engine->getExtraInt("moveforwardclicked") == 1
   || engine->getExtraInt("moveforwardclicked") == 2)
   {
       movingLeft = false;
       movingRight = false;
       movingBackward = false;
       movingForward = true;
       movingDown = false;
       movingUp = false;
   }

   // Moving down
   
   else if (engine->getExtraInt("movedownclicked") == 1
   || engine->getExtraInt("movedownclicked") == 2)
   {
       movingLeft = false;
       movingRight = false;
       movingBackward = false;
       movingForward = false;
       movingDown = true;
       movingUp = false;
   }
   
   // Moving up
		
   else if (engine->getExtraInt("moveupclicked") == 1
   || engine->getExtraInt("moveupclicked") == 2)
   {
       movingLeft = false;
       movingRight = false;
       movingBackward = false;
       movingForward = false;
       movingDown = false;
       movingUp = true;
   }
   	   
   // Stop if finger up
	   
   if (engine->getExtraInt("touchaction1") == 3
   || engine->getExtraInt("touchaction2") == 3)
   {
//	   Log("stop movement");
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

   // Add
      
   static int addTimer = 0;
   
   if (addTimer > 0)
	   addTimer--;
   
   if ((engine->getExtraInt("addclicked") == 1
   || engine->getExtraInt("addclicked") == 2)
   && addTimer == 0)
   {
	   ec->tap(0.0, 0.0);
	   addTimer = 30;
   }

   // Remove
   
   static int rmTimer = 0;
   
   if (rmTimer > 0)
	   rmTimer--;
   
   if ((engine->getExtraInt("removeclicked") == 1
   || engine->getExtraInt("removeclicked") == 2)
   && rmTimer == 0)
   {
	   ec = engine->getEditorController();
	   ec->tap2(0.0, 0.0);
	   rmTimer = 30;
   }
   
   // Update preview
   
   static int refreshPreviewTimer = 0;
	
   if (refreshPreviewTimer > 0)
		refreshPreviewTimer--;
   
   static glm::vec3 lastle = glm::vec3(0.0, 0.0, 0.0);
   glm::vec3 le = ec->getLastEmptyVoxel();
   
   if ((le != lastle || refreshPreview) && refreshPreviewTimer == 0)
   {
       Shape *previewShape = engine->findShape("preview");
	   
	   if (previewShape == nullptr)
		   return;
		   
       previewShape->getVoxels()->clear();
       engine->setVoxel("preview", (int)le.x, (int)le.y, (int)le.z, ec->getCurTexture());
	   refreshPreview = false;
	   refreshPreviewTimer = 0;
//	   Log("prev y", (int)le.y);
   }
   
   lastle = le;
   
//   }
}
