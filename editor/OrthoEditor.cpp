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

#include "OrthoEditor.h"
#include "../engine/SoftCanvas.h"
#include "../thirdparty/lodepng/lodepng.hpp"

void OrthoEditor::init()
{
	g_engine2->init();
    this->engine = g_engine2;
	engine->clear();
}

void OrthoEditor::load()
{
    // Button dimensions
    int numbtns = 6;
    float bw = 0.3;
    float hbw = 0.15;
    float bs = 0.25;
    float bsv = 0.25;
    float hw = 1.0 / numbtns;

    // Top buttons
	engine->addWg("filebtn", WG_BTN, "file.png", "", "filebtnclicked", "", -hw * 5, 0.8, hw, hw);
	engine->addWg("modebtn", WG_BTN, "m.png", "", "modebtnclicked", "", -hw * 3, 0.8, hw, hw);
	engine->addWg("colorbtn", WG_BTN, "palette.png", "", "colorbtnclicked", "", hw * 3, 0.8, hw, hw);
    engine->addWg("backbtn", WG_BTN, "redx.png", "", "backbtnclicked", "", hw * 5, 0.8, hw, hw);

    // Bottom buttons
//    engine->addWg("xzbtn", WG_BTN, "xz.png", "", "xzbtnclicked", "", -hw * 5.25, -0.8, hw, hw);
//    engine->addWg("xybtn", WG_BTN, "xy.png", "", "xybtnclicked", "", -hw * 3.50, -0.8, hw, hw);
//    engine->addWg("yzbtn", WG_BTN, "yz.png", "", "yzbtnclicked", "", hw * -1.75, -0.8, hw, hw);
    engine->addWg("down", WG_BTN, "downarrow.png", "", "downbtnclicked", "", hw * 0.0, -0.8, hw, hw);
    engine->addWg("up", WG_BTN, "uparrow.png", "", "upbtnclicked", "", hw * 1.75, -0.8, hw, hw);
    engine->addWg("prev", WG_BTN, "prevkit.png", "", "prevbtnclicked", "", hw * 3.50, -0.8, hw, hw);
    engine->addWg("next", WG_BTN, "nextkit.png", "", "nextbtnclicked", "", hw * 5.25, -0.8, hw, hw);

	engine->setPhysicsEnabled(false);
	engine->setControlScheme(CTRL_CUSTOM);
	
	// Default voxels
	clear();
		
	engine->setPlayerPos(0.0, 0.0, 0.0);
	engine->setPlayerOrientation(0.0, 0.0, 0.0);
		
	voxels.init(16, nullptr);
	create();
	setDefaultTextures();
	refresh();

    engine->addText("msg", "", 0.0, 0.6, 0.08);
    engine->addText("msg2", "", 0.0, 0.5, 0.08);
}

void OrthoEditor::tick()
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
            gui->addListMenuOption("Save and Exit", "");
            gui->addListMenuOption("Discard and Exit", "");
            gui->showListMenuInDialog("Voxels not saved!", "");
	    }
				
        engine->setExtraInt("backbtnclicked", 0);
		btnTimer = 30;
    }

    if (engine->getExtraStr("listmenuoptionclicked") == "Save and Exit")
    {
        if (PLAT_LoadPref("main", "voxels") == "")
        {
            engine->setText("msg", "filename not set");
            msgTimer = msgTimerDelay;
        }
        else
        {
            PLAT_SavePref("main", "voxels", engine->getExtraStr("fileselected"));
            saveVoxels(engine->getExtraStr("fileselected"));
            engine->setExtraInt("switchmodule", 1);
            engine->setExtraStr("nextmodule", "editor");
        }

        engine->setExtraStr("listmenuoptionclicked", "");
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
		gui->addListMenuOption("New Voxels", "");
		gui->addListMenuOption("Load Voxels", "");
		gui->addListMenuOption("Save Voxels", "");
        gui->addListMenuOption("Export PNG", "");
		gui->showListMenuInDialog("File", "");
		
		engine->setExtraInt("filebtnclicked", 0);
		btnTimer = 30;
    }
	
	if (btnTimer == 0 && (engine->getExtraInt("modebtnclicked") == 1
        || engine->getExtraInt("modebtnclicked") == 1)) {
			
		if (mode == MODE_VOXELS)
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
		}
		
		engine->setExtraInt("modebtnclicked", 0);
		btnTimer = 30;
    }
	
	if (engine->getExtraStr("listmenuoptionclicked") == "New Voxels")
    {
		gui->clearDialog();
		gui->addDialogPart("Resolution", "16", "newvoxelsresolution");
		gui->showDialog("Voxel Parameters", "OK", "Cancel", "voxelparamsselected");

        engine->setExtraStr("voxelsfilename", "");

        engine->setExtraStr("listmenuoptionclicked", "");
    }
	
    if (engine->getExtraInt("voxelparamsselected") == 1)
    {
        // Resolution
        int newVoxelResolution = PLAT_stoi(engine->getExtraStr("newvoxelsresolution"));

        if (newVoxelResolution <= 0)
            newVoxelResolution = 16;
        if (newVoxelResolution > 64)
            newVoxelResolution = 64;
			
		clear();
		
		engine->setPlayerPos(0.0, 0.0, 0.0);
	    engine->setPlayerOrientation(0.0, 0.0, 0.0);
		
		voxels.init(newVoxelResolution, nullptr);
		create();
		setDefaultTextures();
		refresh();

        engine->setExtraInt("voxelparamsselected", 0);
    }
	
	if (engine->getExtraStr("listmenuoptionclicked") == "Load Voxels")
    {
		std::string fname = PLAT_LoadPref("main", "voxels", "");
		if (fname == "")
            fname = g_assetsDir;
		
        gui->showFileSelector("sc", fname);
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "loadvoxels";
    }
	
	if (engine->getExtraStr("listmenuoptionclicked") == "Save Voxels")
    {
		std::string fname = PLAT_LoadPref("main", "voxels", "");
		if (fname == "")
            fname = g_assetsDir;
		
        gui->showFileSelector("sc", fname);
        engine->setExtraStr("listmenuoptionclicked", "");
        fileSelectorAction = "savevoxels";
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

    // File selector actions

    if (engine->getExtraStr("fileselected") != "") {
 
        if (fileSelectorAction == "loadvoxels") {
            PLAT_SavePref("main", "voxels", engine->getExtraStr("fileselected"));
			loadVoxels(engine->getExtraStr("fileselected"));

            char msg[1024];
            snprintf(msg, 1024, "loaded %s", GetFileName(engine->getExtraStr("fileselected")).c_str());
            engine->setText("msg", msg);
            msgTimer = msgTimerDelay;
		}
		
		if (fileSelectorAction == "savevoxels") {
            PLAT_SavePref("main", "voxels", engine->getExtraStr("fileselected"));
			saveVoxels(engine->getExtraStr("fileselected"));

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
	
    if (btnTimer == 0 && (engine->getExtraInt("downbtnclicked") == 1
        || engine->getExtraInt("downbtnclicked") == 1)) {

        if (level > 0)
			level--;

        char msg[1024];
        snprintf(msg, 1024, "layer %d", level);
        engine->setText("msg", msg);
        msgTimer = msgTimerDelay;
			
		refresh();
			
        engine->setExtraInt("backbtnclicked", 0);
		btnTimer = 5;
    }
	
    if (btnTimer == 0 && (engine->getExtraInt("upbtnclicked") == 1
        || engine->getExtraInt("upbtnclicked") == 1)) {

        if (level < voxels.getSize() - 1)
			level++;
			
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

void OrthoEditor::loadVoxels(std::string filename)
{
	clear();
	
	engine->setPlayerPos(0.0, 0.0, 0.0);
	engine->setPlayerOrientation(0.0, 0.0, 0.0);
	
	voxels.load(filename, nullptr, engine->getTextureManager());
	create();
	refresh();
}

void OrthoEditor::saveVoxels(std::string filename)
{
	voxels.save(filename, nullptr);
	modified = false;
}

void OrthoEditor::clear()
{
    for (int z = 0; z < voxels.getSize(); z++)
	{
		for (int x = 0; x < voxels.getSize(); x++)
        {		
			char newNamec[1024];
            snprintf(newNamec, 1024, "vox%d-%d", z, x);
            std::string name = std::string(newNamec);
			
			engine->removeObject(name);
		}
	}
	
	voxels.clear();

    modified = false;
}

void OrthoEditor::create()
{
	for (int z = 0; z < voxels.getSize(); z++)
	{
		for (int x = 0; x < voxels.getSize(); x++)
        {		
			char newNamec[1024];
            snprintf(newNamec, 1024, "vox%d-%d", z, x);
            std::string name = std::string(newNamec);
			
			engine->addObject(name);
			engine->setType(name, OBJTYPE_SPRITE);
		}
	}
}

void OrthoEditor::setDefaultTextures()
{
    voxels.setVoxelTexture(1, "brick3.png");
    voxels.setVoxelTexture(2, "brick4.png");
    voxels.setVoxelTexture(3, "concrete1.png");
    voxels.setVoxelTexture(4, "tile.png");
    voxels.setVoxelTexture(5, "stone1.png");
    voxels.setVoxelTexture(6, "stone2.png");
    voxels.setVoxelTexture(7, "beigestone.png");
    voxels.setVoxelTexture(8, "asphalt.png");
    voxels.setVoxelTexture(9, "graychips.png");
    voxels.setVoxelTexture(10, "leaves.png");
    voxels.setVoxelTexture(11, "bluecube.png");
}

void OrthoEditor::refresh()
{
	for (int z = 0; z < voxels.getSize(); z++)
	{
		for (int x = 0; x < voxels.getSize(); x++)
        {
			int y = 0;	
			int lastTexture = 0;
			int lastLevelWithTexture = 0;
			float bgr = 0.0f, bgg = 0.0f, bgb = 0.0f, bga = 0.0f;
			
			if (voxels.get(x, y, z))
            {
			    if (mode == MODE_PIXELS)
			    {
			        unsigned char ur, ug, ub, ua;
					
			        voxels.getrgba(x, y, z, ur, ug, ub, ua);
				
				    bgr = UCharToFloat255(ur);
			        bgg = UCharToFloat255(ug);
			        bgb = UCharToFloat255(ub);
			        bga = UCharToFloat255(ua);
                }
				
			    lastTexture = voxels.get(x, y, z);
			    lastLevelWithTexture = 0;
			}
			
			while (y < voxels.getSize() && y < level)
			{
				y++;
				
				if (voxels.get(x, y, z))
				{
					if (mode == MODE_PIXELS)
			        {
					    unsigned char ur, ug, ub, ua;
					
			            voxels.getrgba(x, y, z, ur, ug, ub, ua);
				    
					    float fgr = UCharToFloat(ur);
					    float fgg = UCharToFloat(ug);
					    float fgb = UCharToFloat(ub);
					    float fga = UCharToFloat(ua);
					
                        float ra = 1 - (1 - fga) * (1 - bga);
                        float rr = fgr * fga / ra + bgr * bga * (1 - fga) / ra;
                        float rg = fgg * fga / ra + bgg * bga * (1 - fga) / ra;
                        float rb = fgb * fga / ra + bgb * bga * (1 - fga) / ra;

                        bga = ra;
                        bgr = rr;
                        bgg = rg;
                        bgb = rb;
					}
					
				    lastTexture = voxels.get(x, y, z);
                    lastLevelWithTexture = y;
				}
			}
					
			char newNamec[1024];
            snprintf(newNamec, 1024, "vox%d-%d", z, x);
            std::string name = std::string(newNamec);
			
			float sx = 0, sy = 0, sz = -5;
			float size = (float)voxels.getSize();
			float voxssize = 2.0 / size;
			
			sx = -1.0 + (float)x * voxssize + voxssize / 2.0;
			sy = -1.0 + (float)z * voxssize + voxssize / 2.0;
			
			if (mode == MODE_VOXELS)
			{
			    engine->setTexture(name, voxels.getVoxelTexture(lastTexture));
			}
			else
			{
			    engine->setTexture(name, "white.png");
			}
			
			if (mode == MODE_VOXELS)
			{
			    int levelDiff = level - lastLevelWithTexture;
			    float darkening = ((float)voxels.getSize() - (float)(voxels.getSize() - levelDiff)) / (float)voxels.getSize();

				engine->setColor(name, 1.0 - darkening, 1.0 - darkening, 1.0 - darkening, 1.0f);
			}
			else if (mode == MODE_PIXELS)
			{
				engine->setColor(name, bgr, bgg, bgb, bga);
			}
			
			engine->setSize(name, voxssize, voxssize, voxssize);
			engine->setPos(name, sx, sy, sz);
		}
	}
}

void OrthoEditor::touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2, int actionIndex)
{
	float aspect = (float)screenWidth / (float)screenHeight;
	
	float glx = scrToGlX(x1);
	float gly = scrToGlY(y1);
	
	float glFromX = -1.0;
	float glToX = 1.0;
	float glFromY = -1.0 * aspect;
	float glToY = 1.0 * aspect;
	
	float leftX = glx - glFromX;
	float rightX = glToX - glx;
	float ratioX = leftX / (leftX + rightX);
	
	float leftY = gly - glFromY;
	float rightY = glToY - gly;
	float ratioY = leftY / (leftY + rightY);
	
	int x = (int)(ratioX * (float)voxels.getSize());
	int z = (int)(ratioY * (float)voxels.getSize());

	if (x >= 0 && x < voxels.getSize() && z >= 0 && z < voxels.getSize()) {
        voxels.set(x, level, z, texture);

        if (colora != 0.0) {
            voxels.setrgba(x, level, z, FloatToUChar255(colorr), FloatToUChar255(colorg),
                           FloatToUChar255(colorb), FloatToUChar255(colora));
        } else {
            voxels.set(x, level, z, 0);
            voxels.setrgba(x, level, z, 127, 127, 127, 127);
        }

        modified = true;
        needsRefresh = true;
    }

	/*
	
	    if (mTileMap == null) {
            return true;
        }

        final int pointerCount = ev.getPointerCount();

        if (ui.getOnScreenTouch() != null) {
            ui.getOnScreenTouch().respondToNode();
        }

        if (mZoomAndPanLock) {
            if (mModifySkipTimer > 0) {
                mModifySkipTimer--;
                return true;
            }

            float vertexGlX = 0;
            float vertexGlY = 0;

            if (mTool == OffsetEditTools.SHAPETILES) {
                if (ev.getAction() == MotionEvent.ACTION_DOWN) {

                    if (mDragging) {
                        return true;
                    }
					
					if (mCurLevel == 0) {
						resetLevel();
					}

                    if (mPlane == OffsetPlanes.XY) {
                        for (int y = 0; y < mTileMap.mSizeY; y++) {
                            for (int x = 0; x < mTileMap.mSizeX; x++) {

                                vertexGlX = x * mGLTileSizeX + mTileMap.getOffsetX(x, y, mCurLevel) * mGLTileSizeX;
                                vertexGlY = y * mGLTileSizeY + mTileMap.getOffsetY(x, y, mCurLevel) * mGLTileSizeY;

                                // See if we hit near glX, glY

                                float tapScreenX = ev.getX();
                                float tapScreenY = ev.getY();

                                float tapRelX = tapScreenX / (float) ui.getScreenWidth();
                                float tapRelYFromTop = tapScreenY / (float) ui.getContentHeight();
                                float tapRelYFromBottom = 1f - tapRelYFromTop;

                                float tapGlX = mCameraMinX + (mCameraMaxX - mCameraMinX) * tapRelX;
                                float tapGlY = mCameraMinY + (mCameraMaxY - mCameraMinY) * tapRelYFromBottom;

                                if (Math.abs(tapGlX - vertexGlX) < VERTEX_DRAG_THRESHOLD
									&& Math.abs(tapGlY - vertexGlY) < VERTEX_DRAG_THRESHOLD) {
                                    mDragging = true;
                                    mDraggingTileX = x;
                                    mDraggingTileY = y;
                                    mDraggingTileZ = mCurLevel;
                                    mLastDragGlX = tapGlX;
                                    mLastDragGlY = tapGlY;
                                }
                            }
                        }
                    }
                    if (mPlane == OffsetPlanes.XZ) {
                        for (int z = 0; z < mTileMap.mSizeZ; z++) {
                            for (int x = 0; x < mTileMap.mSizeX; x++) {

                                vertexGlX = x * mGLTileSizeX + mTileMap.getOffsetX(x, mTileMap.mSizeY - mCurLevel, z) * mGLTileSizeX;
                                vertexGlY = z * mGLTileSizeZ + mTileMap.getOffsetZ(x, mTileMap.mSizeY - mCurLevel, z) * mGLTileSizeZ;

                                // See if we hit near glX, glY

                                float tapScreenX = ev.getX();
                                float tapScreenY = ev.getY();

                                float tapRelX = tapScreenX / (float) ui.getScreenWidth();
                                float tapRelYFromTop = tapScreenY / (float) ui.getContentHeight();
                                float tapRelYFromBottom = 1f - tapRelYFromTop;

                                float tapGlX = mCameraMinX + (mCameraMaxX - mCameraMinX) * tapRelX;
                                float tapGlY = mCameraMinY + (mCameraMaxY - mCameraMinY) * tapRelYFromBottom;

                                if (Math.abs(tapGlX - vertexGlX) < 30
                                        && Math.abs(tapGlY - vertexGlY) < 30) {
                                    mDragging = true;
                                    mDraggingTileX = x;
//                                if (mCurLevel == 0)
//                                    mCurLevel = 1;
//                                mDraggingTileY = mCurLevel - 1;
                                    mDraggingTileY = mTileMap.mSizeY - mCurLevel;
                                    mDraggingTileZ = z;
                                    mLastDragGlX = tapGlX;
                                    mLastDragGlY = tapGlY;
                                }
                            }
                        }
                    }
                    if (mPlane == OffsetPlanes.YZ) {
                        for (int z = 0; z < mTileMap.mSizeZ; z++) {
                            for (int y = 0; y < mTileMap.mSizeY; y++) {

                                vertexGlX = y * mGLTileSizeY + mTileMap.getOffsetY(mCurLevel - 1, y, z) * mGLTileSizeY;
                                vertexGlY = z * mGLTileSizeZ + mTileMap.getOffsetZ(mCurLevel - 1, y, z) * mGLTileSizeZ;

                                // See if we hit near glX, glY

                                float tapScreenX = ev.getX();
                                float tapScreenY = ev.getY();

                                float tapRelX = tapScreenX / (float) ui.getScreenWidth();
                                float tapRelYFromTop = tapScreenY / (float) ui.getContentHeight();
                                float tapRelYFromBottom = 1f - tapRelYFromTop;

                                float tapGlX = mCameraMinX + (mCameraMaxX - mCameraMinX) * tapRelX;
                                float tapGlY = mCameraMinY + (mCameraMaxY - mCameraMinY) * tapRelYFromBottom;

                                if (Math.abs(tapGlX - vertexGlX) < 30
                                        && Math.abs(tapGlY - vertexGlY) < 30) {
                                    mDragging = true;
                                    mDraggingTileX = mCurLevel - 1;
                                    mDraggingTileY = y;
                                    mDraggingTileZ = z;
                                    mLastDragGlX = tapGlX;
                                    mLastDragGlY = tapGlY;
                                }
                            }
                        }
                    }
                }
                if (ev.getAction() == MotionEvent.ACTION_MOVE) {

                    // Move is locked

                    if (mDragging) {
                        float dragScreenX = ev.getX();
                        float dragScreenY = ev.getY();

                        float dragRelX = dragScreenX / (float) ui.getScreenWidth();
                        float dragRelYFromTop = dragScreenY / (float) ui.getContentHeight();
                        float dragRelYFromBottom = 1f - dragRelYFromTop;

                        float dragGlX = mCameraMinX + (mCameraMaxX - mCameraMinX) * dragRelX;
                        float dragGlY = mCameraMinY + (mCameraMaxY - mCameraMinY) * dragRelYFromBottom;

                        if (mTileMap == null) {
                            return true;
                        }


                        if (mPlane == OffsetPlanes.XY) {
                            float newOffsetX = (dragGlX - (float) mDraggingTileX * mGLTileSizeX) / mGLTileSizeX;
                            float newOffsetY = (dragGlY - (float) mDraggingTileY * mGLTileSizeY) / mGLTileSizeY;

                            mTileMap.setOffsetX(mDraggingTileX, mDraggingTileY, mDraggingTileZ, newOffsetX);
                            mTileMap.setOffsetY(mDraggingTileX, mDraggingTileY, mDraggingTileZ, newOffsetY);
							
							mTileMap.setOffsetX(mDraggingTileX, mDraggingTileY, mDraggingTileZ + 1, newOffsetX);
                            mTileMap.setOffsetY(mDraggingTileX, mDraggingTileY, mDraggingTileZ + 1, newOffsetY);
                        }
                        if (mPlane == OffsetPlanes.XZ) {
                            float newOffsetX = (dragGlX - (float) mDraggingTileX * mGLTileSizeX) / mGLTileSizeX;
                            float newOffsetY = (dragGlY - (float) mDraggingTileZ * mGLTileSizeZ) / mGLTileSizeZ;

                            mTileMap.setOffsetX(mDraggingTileX, mDraggingTileY, mDraggingTileZ, newOffsetX);
                            mTileMap.setOffsetZ(mDraggingTileX, mDraggingTileY, mDraggingTileZ, newOffsetY);
							
							mTileMap.setOffsetX(mDraggingTileX, mDraggingTileY + 1, mDraggingTileZ, newOffsetX);
                            mTileMap.setOffsetZ(mDraggingTileX, mDraggingTileY + 1, mDraggingTileZ, newOffsetY);
                        }
                        if (mPlane == OffsetPlanes.YZ) {
                            float newOffsetX = (dragGlX - (float) mDraggingTileY * mGLTileSizeY) / mGLTileSizeY;
                            float newOffsetY = (dragGlY - (float) mDraggingTileZ * mGLTileSizeZ) / mGLTileSizeZ;

                            mTileMap.setOffsetY(mDraggingTileX, mDraggingTileY, mDraggingTileZ, newOffsetX);
                            mTileMap.setOffsetZ(mDraggingTileX, mDraggingTileY, mDraggingTileZ, newOffsetY);
							
							mTileMap.setOffsetY(mDraggingTileX + 1, mDraggingTileY, mDraggingTileZ, newOffsetX);
                            mTileMap.setOffsetZ(mDraggingTileX + 1, mDraggingTileY, mDraggingTileZ, newOffsetY);
                        }

                        mLastDragGlX = dragGlX;
                        mLastDragGlY = dragGlY;
                    }
                }
                if (ev.getAction() == MotionEvent.ACTION_UP) {
                    mDragging = false;
                }
            } else if (mTool == OffsetEditTools.DRAWTILES) {
                if (ev.getAction() == MotionEvent.ACTION_DOWN || ev.getAction() == MotionEvent.ACTION_MOVE) {

                    if (mPlane == OffsetPlanes.XY) {
                        for (int y = 0; y < mTileMap.mSizeY; y++) {
                            for (int x = 0; x < mTileMap.mSizeX; x++) {

                                int viewWidth = ui.getScreenWidth();
                                int viewHeight = ui.getContentHeight();
                                int viewX = (int) ev.getX();
                                int viewY = (int) ev.getY();

                                viewY = ui.getContentHeight() - viewY;

                                float cameraWidth = mCameraMaxX - mCameraMinX;
                                float cameraHeight = mCameraMaxY - mCameraMinY;

                                float glX = mCameraMinX + ((float) viewX / (float) viewWidth) * cameraWidth;
                                float glY = mCameraMinY + ((float) viewY / (float) viewHeight) * cameraHeight;

                                int tileX = (int) (mTileMap.mSizeX * (double) (glX / mMaxX));
                                int tileY = (int) (mTileMap.mSizeY * (double) (glY / mMaxY));

                                if (tileX < mTileMap.mSizeX && tileY < mTileMap.mSizeY) {
//                                  ui.consoleInUIThread(tileX + "," + tileY + "," + mTileEdit.mCurLevel + "=" + mTileEdit.mCurTile);
                                    mTileMap.set(tileX, tileY, mCurLevel, edit3d.getSelectedTileId());
                                }
                            }
                        }
                    }
                    if (mPlane == OffsetPlanes.XZ) {
						for (int y = 0; y < mTileMap.mSizeY; y++) {
                            for (int x = 0; x < mTileMap.mSizeX; x++) {

                                int viewWidth = ui.getScreenWidth();
                                int viewHeight = ui.getContentHeight();
                                int viewX = (int) ev.getX();
                                int viewY = (int) ev.getY();

                                viewY = ui.getContentHeight() - viewY;

                                float cameraWidth = mCameraMaxX - mCameraMinX;
                                float cameraHeight = mCameraMaxY - mCameraMinY;

                                float glX = mCameraMinX + ((float) viewX / (float) viewWidth) * cameraWidth;
                                float glY = mCameraMinY + ((float) viewY / (float) viewHeight) * cameraHeight;

                                int tileX = (int) (mTileMap.mSizeX * (double) (glX / mMaxX));
                                int tileZ = (int) (mTileMap.mSizeZ * (double) (glY / mMaxY));

                                if (tileX < mTileMap.mSizeX && tileZ < mTileMap.mSizeZ) {
//                                  ui.consoleInUIThread(tileX + "," + tileY + "," + mTileEdit.mCurLevel + "=" + mTileEdit.mCurTile);
                                    mTileMap.set(tileX, mTileMap.mSizeY - mCurLevel, tileZ, edit3d.getSelectedTileId());
                                }
                            }
                        }
						
                    }
                    if (mPlane == OffsetPlanes.YZ) {
						for (int y = 0; y < mTileMap.mSizeY; y++) {
                            for (int x = 0; x < mTileMap.mSizeX; x++) {

                                int viewWidth = ui.getScreenWidth();
                                int viewHeight = ui.getContentHeight();
                                int viewX = (int) ev.getX();
                                int viewY = (int) ev.getY();

                                viewY = ui.getContentHeight() - viewY;

                                float cameraWidth = mCameraMaxX - mCameraMinX;
                                float cameraHeight = mCameraMaxY - mCameraMinY;

                                float glX = mCameraMinX + ((float) viewX / (float) viewWidth) * cameraWidth;
                                float glY = mCameraMinY + ((float) viewY / (float) viewHeight) * cameraHeight;

                                int tileY = (int) (mTileMap.mSizeY * (double) (glX / mMaxX));
                                int tileZ = (int) (mTileMap.mSizeZ * (double) (glY / mMaxY));

                                if (tileY < mTileMap.mSizeY && tileZ < mTileMap.mSizeZ) {
//                                  ui.consoleInUIThread(tileX + "," + tileY + "," + mTileEdit.mCurLevel + "=" + mTileEdit.mCurTile);
                                    mTileMap.set(mCurLevel - 1, tileY, tileZ, edit3d.getSelectedTileId());
                                }
                            }
                        }
						
                    }
                }
            }

        } else {
            // Move is unlocked; we are panning / zooming
            if (ev.getAction() == MotionEvent.ACTION_DOWN) {
                if (pointerCount == 1) {
                    m_pxLastPanX = (int) ev.getX();
                    m_pxLastPanY = (int) ev.getY();

                    int touchX = m_pxLastPanX;
                    int touchY = m_pxLastPanY;
                }
            }
            if (ev.getAction() == MotionEvent.ACTION_MOVE) {
                if (pointerCount == 1) {

                    if (m_afterZoomPanSkipTimer > 0) {
                        m_afterZoomPanSkipTimer--;
                    } else {

                        // Panning

                        if (m_pxLastPanX == 0) {
                            m_pxLastPanX = (int) ev.getX();
                        }
                        if (m_pxLastPanY == 0) {
                            m_pxLastPanY = (int) ev.getY();
                        }

                        int pxNewPanX = (int) ev.getX();
                        int pxNewPanY = (int) ev.getY();

                        int pxDeltaPanX = pxNewPanX - m_pxLastPanX;
                        int pxDeltaPanY = pxNewPanY - m_pxLastPanY;

                        // Perform pan calculations

                        float deltaPanX = pxDeltaPanX * (mCameraMaxX - mCameraMinX) / (mMaxX - mMinX);
                        float deltaPanY = pxDeltaPanY * (mCameraMaxY - mCameraMinY) / (mMaxY - mMinY);

                        deltaPanY = -deltaPanY;

                        mTargetX -= deltaPanX;
                        mTargetY -= deltaPanY;

                        // Call back main process
                        if (m_onPan != null) {
                            m_onPan.respondToNode();
                        }

                        // Prepare for next touch motion event
                        m_pxLastPanX = pxNewPanX;
                        m_pxLastPanY = pxNewPanY;
                    }

                } else if (pointerCount > 1) {
                    // Zooming

                    int pxNewZoomX1 = (int) ev.getX(0);
                    int pxNewZoomY1 = (int) ev.getY(0);
                    int pxNewZoomX2 = (int) ev.getX(1);
                    int pxNewZoomY2 = (int) ev.getY(1);

                    int deltaZoomX = pxNewZoomX1 - m_pxLastZoomX2;
                    int deltaZoomY = pxNewZoomY1 - m_pxLastZoomY2;

                    double zoomH = 0;

                    zoomH = Math.sqrt((deltaZoomX * deltaZoomX) + (deltaZoomY * deltaZoomY));

                    double deltaZoomH = zoomH - m_lastZoomH;

                    ui.getScreenDiagonalSize();

                    m_zoomDelay++;

                    double targetZoom = mZoom + deltaZoomH / 10;
                    if (targetZoom < 10) {
                        targetZoom = 10;
                    }
                    if (targetZoom > 90) {
                        targetZoom = 90;
                    }
                    double zoom = mZoom;

                    double zoomDelta = 2.0f;
                    if (targetZoom > 70) {
                        zoomDelta = 1.0f;
                    }
                    if (targetZoom > 80) {
                        zoomDelta = 0.6f;
                    }

                    if (zoom > targetZoom) {
                        mZoom -= zoomDelta;
                    }
                    if (zoom < targetZoom) {
                        mZoom += zoomDelta;
                    }

                    m_afterZoomPanSkipTimer = 10;

                    // Ready for next loop

                    m_lastZoomH = zoomH;

                    m_pxLastDeltaZoomX = deltaZoomX;
                    m_pxLastDeltaZoomY = deltaZoomY;

                    m_pxLastZoomX1 = pxNewZoomX1;
                    m_pxLastZoomY1 = pxNewZoomY1;
                    m_pxLastZoomX2 = pxNewZoomX2;
                    m_pxLastZoomY2 = pxNewZoomY2;
                }
            }
        }


	
	*/
}

/*
    public void drawFrame(GL10 gl) {

        LozowareAndroidUI ui = (LozowareAndroidUI) Lozoware.getUI();
        GL3DAndroid gl3d = (GL3DAndroid) ui.getGL3D();
        ByteBuffer bb = null;

        // Calculate camera position

        float screenAspectRatio = (float) ui.getScreenWidth() / (float) ui.getContentHeight();

        float mViewWidth = 1200;
        float mViewHeight = 1200;

        if (mZoom > MAX_ZOOM) {
            double a = mZoom - MAX_ZOOM;
            mViewWidth -= a * MAX_ZOOM;
            mViewHeight -= a * MAX_ZOOM;
        }
		
		if (mCurLevel == 0) {
			resetLevel();
		}

        mViewWidth = mViewWidth * screenAspectRatio;
        mViewHeight = mViewHeight;

        mCameraMinY = mTargetY - (mViewHeight / 2);
        mCameraMaxY = mTargetY + (mViewHeight / 2);
        mCameraMinX = mTargetX - (mViewWidth / 2);
        mCameraMaxX = mTargetX + (mViewWidth / 2);

        // Set OpenGL settings

        gl.glDisable(GL10.GL_DEPTH_TEST);
        gl.glEnable(GL10.GL_TEXTURE_2D);
        gl.glBlendFunc(GL10.GL_SRC_ALPHA, GL10.GL_ONE_MINUS_SRC_ALPHA);
        gl.glEnable(GL10.GL_BLEND);

        // Set OpenGL view

        gl.glLoadIdentity();

        gl.glOrthof(mCameraMinX, mCameraMaxX, mCameraMinY, mCameraMaxY, -1000, 1000);

        gl.glClearColor(0.0f, 0.0f, 0.0f, 1f);

        gl.glClear(GL10.GL_COLOR_BUFFER_BIT);

        // Do not continue if no tilemap

        TileMap3D tm = mTileMap;

        if (tm == null) {
            return;
        }

        // Define tile id and texture id

        int tileId = 0;
        int glTexId = 0;

        // Do not continue if no tileset

        TilesetAndroid ts = (TilesetAndroid) gl3d.getActiveTileset();

        if (ts == null) {
            return;
        }

//        mCurLevel = tm.mSizeZ;

        // Figure out standard tile size

        mGLTileSizeX = mMaxX / tm.mSizeX;
        mGLTileSizeY = mMaxX / tm.mSizeY;
        mGLTileSizeZ = mMaxX / tm.mSizeY;

        // Draw tiles
        if (mPlane == OffsetPlanes.XY) {

            for (int z = 0; z <= mCurLevel; z++) {
                for (int y = 0; y < tm.mSizeY; y++) {
                    for (int x = 0; x < tm.mSizeX; x++) {

                        int b = tm.get(x, y, z);

                        if (b != 0) {

                            // Set texture
                            glTexId = ts.getGlTexId((int) b, gl);
							
							// Calculate alpha
							float alpha = 1.0f;
							if (z < mCurLevel)
							    alpha = 0.5f;

                            // Texture
                            gl.glColor4f(1.0f, 1.0f, 1.0f, alpha);

                            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(textureCoords.length * 4);
                            byteBuffer.order(ByteOrder.nativeOrder());
                            textureCoordsBuffer = byteBuffer.asFloatBuffer();
                            textureCoordsBuffer.put(textureCoords);
                            textureCoordsBuffer.position(0);

                            gl.glBindTexture(GL10.GL_TEXTURE_2D, glTexId);

                            // Vertex buffer
                            bb = ByteBuffer.allocateDirect((6 * 3) * 3 * 4);
                            bb.order(ByteOrder.nativeOrder());
                            FloatBuffer buf = bb.asFloatBuffer();

                            float bottomLeftX = x * mGLTileSizeX;
                            float bottomLeftY = y * mGLTileSizeY;
                            float topLeftX = x * mGLTileSizeX;
                            float topLeftY = y * mGLTileSizeY + mGLTileSizeY;
                            float bottomRightX = x * mGLTileSizeX + mGLTileSizeX;
                            float bottomRightY = y * mGLTileSizeY;
                            float topRightX = x * mGLTileSizeX + mGLTileSizeX;
                            float topRightY = y * mGLTileSizeY + mGLTileSizeY;

                            // Apply offset
                            bottomLeftX += mGLTileSizeX * tm.getOffsetX(x, y, z);
                            bottomLeftY += mGLTileSizeY * tm.getOffsetY(x, y, z);
                            topLeftX += mGLTileSizeX * tm.getOffsetX(x, y + 1, z);
                            topLeftY += mGLTileSizeY * tm.getOffsetY(x, y + 1, z);
                            bottomRightX += mGLTileSizeX * tm.getOffsetX(x + 1, y, z);
                            bottomRightY += mGLTileSizeY * tm.getOffsetY(x + 1, y, z);
                            topRightX += mGLTileSizeX * tm.getOffsetX(x + 1, y + 1, z);
                            topRightY += mGLTileSizeY * tm.getOffsetY(x + 1, y + 1, z);

                            // Fill vertex buffer

                            buf.position(0);

                            buf.put(bottomLeftX);
                            buf.put(bottomLeftY);
                            buf.put(0);

                            buf.put(topLeftX);
                            buf.put(topLeftY);
                            buf.put(0);

                            buf.put(bottomRightX);
                            buf.put(bottomRightY);
                            buf.put(0);

                            buf.put(topLeftX);
                            buf.put(topLeftY);
                            buf.put(0);

                            buf.put(topRightX);
                            buf.put(topRightY);
                            buf.put(0);

                            buf.put(bottomRightX);
                            buf.put(bottomRightY);
                            buf.put(0);

                            buf.position(0);

                            // Draw
                            gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
                            if (glTexId != 0) {
                                gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
                            }

                            gl.glVertexPointer(3, GL10.GL_FLOAT, 0, buf);
                            if (glTexId != 0) {
                                gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, textureCoordsBuffer);
                            }

                            gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 6 * 3);
                            gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
                            if (glTexId != 0) {
                                gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
                            }
                        }
                    }
                }
            }
        }
        if (mPlane == OffsetPlanes.XZ) {

//            for (int y = mCurLevel; y >= 0; y--) { // z -> y
            for (int y = tm.mSizeY; y >= (tm.mSizeY - mCurLevel); y--) { // z -> y
                for (int z = 0; z < tm.mSizeZ; z++) { // y -> z
                    for (int x = 0; x < tm.mSizeX; x++) {

                        int b = tm.get(x, y, z);

                        if (b != 0) {

                            // Set texture
                            glTexId = ts.getGlTexId((int) b, gl);
							
				            // Calculate alpha
							float alpha = 1.0f;
							if ((tm.mSizeY - y) < mCurLevel)
							    alpha = 0.5f;

                            // Texture
                            gl.glColor4f(1.0f, 1.0f, 1.0f, alpha);

                            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(textureCoords.length * 4);
                            byteBuffer.order(ByteOrder.nativeOrder());
                            textureCoordsBuffer = byteBuffer.asFloatBuffer();
                            textureCoordsBuffer.put(textureCoords);
                            textureCoordsBuffer.position(0);

                            gl.glBindTexture(GL10.GL_TEXTURE_2D, glTexId);

                            // Vertex buffer
                            bb = ByteBuffer.allocateDirect((6 * 3) * 3 * 4);
                            bb.order(ByteOrder.nativeOrder());
                            FloatBuffer buf = bb.asFloatBuffer();

                            float bottomLeftX = x * mGLTileSizeX;
                            float bottomLeftY = z * mGLTileSizeZ;
                            float topLeftX = x * mGLTileSizeX;
                            float topLeftY = z * mGLTileSizeZ + mGLTileSizeZ;
                            float bottomRightX = x * mGLTileSizeX + mGLTileSizeX;
                            float bottomRightY = z * mGLTileSizeZ;
                            float topRightX = x * mGLTileSizeX + mGLTileSizeX;
                            float topRightY = z * mGLTileSizeZ + mGLTileSizeZ;

                            // Apply offset
                            bottomLeftX += mGLTileSizeX * tm.getOffsetX(x, y, z);
                            bottomLeftY += mGLTileSizeZ * tm.getOffsetZ(x, y, z);
                            topLeftX += mGLTileSizeX * tm.getOffsetX(x, y, z + 1);
                            topLeftY += mGLTileSizeZ * tm.getOffsetZ(x, y, z + 1);
                            bottomRightX += mGLTileSizeX * tm.getOffsetX(x + 1, y, z);
                            bottomRightY += mGLTileSizeZ * tm.getOffsetZ(x + 1, y, z);
                            topRightX += mGLTileSizeX * tm.getOffsetX(x + 1, y, z + 1);
                            topRightY += mGLTileSizeZ * tm.getOffsetZ(x + 1, y, z + 1);

                            // Fill vertex buffer

                            buf.position(0);

                            buf.put(bottomLeftX);
                            buf.put(bottomLeftY);
                            buf.put(0);

                            buf.put(topLeftX);
                            buf.put(topLeftY);
                            buf.put(0);

                            buf.put(bottomRightX);
                            buf.put(bottomRightY);
                            buf.put(0);

                            buf.put(topLeftX);
                            buf.put(topLeftY);
                            buf.put(0);

                            buf.put(topRightX);
                            buf.put(topRightY);
                            buf.put(0);

                            buf.put(bottomRightX);
                            buf.put(bottomRightY);
                            buf.put(0);

                            buf.position(0);

                            // Draw
                            gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
                            if (glTexId != 0) {
                                gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
                            }

                            gl.glVertexPointer(3, GL10.GL_FLOAT, 0, buf);
                            if (glTexId != 0) {
                                gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, textureCoordsBuffer);
                            }

                            gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 6 * 3);
                            gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
                            if (glTexId != 0) {
                                gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
                            }
                        }
                    }
                }
            }
        }
        if (mPlane == OffsetPlanes.YZ) {

            for (int x = 0; x < mCurLevel; x++) { // y -> x
                for (int z = 0; z < tm.mSizeZ; z++) {
                    for (int y = 0; y < tm.mSizeY; y++) {

                        int b = tm.get(x, y, z);

                        if (b != 0) {

                            // Set texture
                            glTexId = ts.getGlTexId((int) b, gl);
							
			                // Calculate alpha
							float alpha = 1.0f;
							if (x < mCurLevel - 1)
							    alpha = 0.5f;

                            // Texture
                            gl.glColor4f(1.0f, 1.0f, 1.0f, alpha);

                            ByteBuffer byteBuffer = ByteBuffer.allocateDirect(textureCoords.length * 4);
                            byteBuffer.order(ByteOrder.nativeOrder());
                            textureCoordsBuffer = byteBuffer.asFloatBuffer();
                            textureCoordsBuffer.put(textureCoords);
                            textureCoordsBuffer.position(0);

                            gl.glBindTexture(GL10.GL_TEXTURE_2D, glTexId);

                            // Vertex buffer
                            bb = ByteBuffer.allocateDirect((6 * 3) * 3 * 4);
                            bb.order(ByteOrder.nativeOrder());
                            FloatBuffer buf = bb.asFloatBuffer();

                            float bottomLeftX = y * mGLTileSizeY;
                            float bottomLeftY = z * mGLTileSizeZ;
                            float topLeftX = y * mGLTileSizeY;
                            float topLeftY = z * mGLTileSizeZ + mGLTileSizeZ;
                            float bottomRightX = y * mGLTileSizeY + mGLTileSizeY;
                            float bottomRightY = z * mGLTileSizeZ;
                            float topRightX = y * mGLTileSizeY + mGLTileSizeY;
                            float topRightY = z * mGLTileSizeZ + mGLTileSizeZ;

                            // Apply offset
                            bottomLeftX += mGLTileSizeY * tm.getOffsetY(x, y, z);
                            bottomLeftY += mGLTileSizeZ * tm.getOffsetZ(x, y, z);
                            topLeftX += mGLTileSizeY * tm.getOffsetY(x, y, z + 1);
                            topLeftY += mGLTileSizeZ * tm.getOffsetZ(x, y, z + 1);
                            bottomRightX += mGLTileSizeY * tm.getOffsetY(x, y + 1, z);
                            bottomRightY += mGLTileSizeZ * tm.getOffsetZ(x, y + 1, z);
                            topRightX += mGLTileSizeY * tm.getOffsetY(x, y + 1, z + 1);
                            topRightY += mGLTileSizeZ * tm.getOffsetZ(x, y + 1, z + 1);

                            // Fill vertex buffer

                            buf.position(0);

                            buf.put(bottomLeftX);
                            buf.put(bottomLeftY);
                            buf.put(0);

                            buf.put(topLeftX);
                            buf.put(topLeftY);
                            buf.put(0);

                            buf.put(bottomRightX);
                            buf.put(bottomRightY);
                            buf.put(0);

                            buf.put(topLeftX);
                            buf.put(topLeftY);
                            buf.put(0);

                            buf.put(topRightX);
                            buf.put(topRightY);
                            buf.put(0);

                            buf.put(bottomRightX);
                            buf.put(bottomRightY);
                            buf.put(0);

                            buf.position(0);

                            // Draw
                            gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
                            if (glTexId != 0) {
                                gl.glEnableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
                            }

                            gl.glVertexPointer(3, GL10.GL_FLOAT, 0, buf);
                            if (glTexId != 0) {
                                gl.glTexCoordPointer(2, GL10.GL_FLOAT, 0, textureCoordsBuffer);
                            }

                            gl.glDrawArrays(GL10.GL_TRIANGLES, 0, 6 * 3);
                            gl.glDisableClientState(GL10.GL_VERTEX_ARRAY);
                            if (glTexId != 0) {
                                gl.glDisableClientState(GL10.GL_TEXTURE_COORD_ARRAY);
                            }
                        }
                    }
                }
            }
        }

        gl.glFlush();
    }

*/

float OrthoEditor::scrToGlX(float screenX) {
    return 2.0f * screenX / screenWidth - 1.0f;
}

float OrthoEditor::scrToGlY(float screenY) {
    return -(2.0f * screenY / screenHeight - 1.0f);
}

float OrthoEditor::glToScrX(float glX) {
    return ((glX + 1.0f) / 2.0f) * screenWidth;
}

float OrthoEditor::glToScrY(float glY) {
    return screenHeight - abs(((glY + 1.0f) / 2.0f) * screenHeight);
}

void OrthoEditor::exportPNG(std::string filename)
{
    SoftCanvas sc;
    sc.init(voxels.getSize(), voxels.getSize());

    for (int z = 0; z < voxels.getSize(); z++) {
        for (int x = 0; x < voxels.getSize(); x++) {
            int y = 0;

            if (voxels.get(x, y, z))
            {
                unsigned char ur, ug, ub, ua;
                voxels.getrgba(x, y, z, ur, ug, ub, ua);
                sc.setPx(x, voxels.getSize() - z, ur, ug, ub, ua);
            }
        }
    }

    std::vector<unsigned char> image(sc.getData(), sc.getData() + sc.getWidth() * sc.getHeight() * 4);
    std::vector<unsigned char> png;

    unsigned error = lodepng::encode(png, image, sc.getWidth(), sc.getHeight());
    if(!error) lodepng::save_file(png, filename);
}
