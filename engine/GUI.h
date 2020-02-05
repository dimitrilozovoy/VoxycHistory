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

#ifndef GUI_h
#define GUI_h

#include "GUI.h"
#include "TextureManager2.h"
#include "TextPrinter.h"
#include "SpriteRenderer2D.hpp"
#include <string>
#include <map>
#include "GLIncludes.h"

typedef enum WidgetType
{
	WG_BTN,
	WG_MENU,
	WG_MENUITEM,
	WG_TEXT,
    WG_SLIDER,
	NUM_WIDGETTYPES
} WidgetType;

typedef struct Widget
{
	std::string name = "";
	WidgetType type;
	std::string text = "";
	glm::vec2 position = glm::vec2(0.0, 0.0);
	glm::vec2 size = glm::vec2(0.1, 0.1);
	std::string texture;
	std::string textureClicked;
	std::string onClickExtra;
	std::string group;
	bool visible = true;
	glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	std::string font = "";
    float sliderValue = 0.5f;
} Widget;

typedef struct DialogPart
{
	std::string caption = "";
	std::string value = "";
	std::string extraKey = "";
} DialogPart;

class GUI
{
public:
	void init(TextureManager2 *texMan, SpriteRenderer2D *renderer, TextPrinter *tp);
	void tick();
    void draw();
	void drawBtn(Widget *item);
    void drawMenu(Widget *item);
	void drawSlider(Widget *item);
	void addWg(std::string name, WidgetType type, std::string texture, std::string text, std::string onClickExtra, std::string group, float x, float y, float sizex, float sizey);
	void remWg(std::string name);
	Widget *findWg(std::string name);
	void setWgVisible(std::string name, bool val);
    void setWgGroupVisible(std::string group, bool val);
	void setWgColor(std::string name, float r, float g, float b, float a);
    std::string getOnClickExtraIfClicked(int action, float x, float y, int finger, int count);
	std::map<std::string, Widget*> getWidgets() { return widgets; }
    void clear();
	void hide();
	void show();

	void setNativeWidgets(bool val) { nativeWidgets = val; };

	void showFileSelector(std::string ext, std::string dir = "");
	void drawFileSelector();

	void clearListMenu();
	void addListMenuOption(std::string title, std::string desc);
	void showListMenuInDialog(std::string title, std::string options);
	void drawListMenu();

	void showText(std::string text);
	void drawText();

	void showLongText(std::string text);
	void drawLongText();

	void clearDialog();
	void addDialogPart(std::string caption, std::string defaultValue, std::string extraKey);
	void showDialog(std::string title, std::string okText, std::string cancelText, std::string okExtra);
	void drawDialog();

	void up();
	void down();
	void left();
	void right();
	void enter();
	void escape();
		
	void charEntered(char c);
	void backspace();

	bool nonNativeWidgetsShown();
	void clearNonNativeWidgets();
	Widget* getTopVisibleWidget();

	Widget* getSelectedWidget() {
		return selectedWidget;
	}

	void setSelectedWidget(Widget *w) {
		selectedWidget = w;
	}

	void setFontKern(std::string font, float kern);
    
    bool isInternallyGeneratedShown();
    
    void setMenuSounds(std::string move, std::string select);
    void playMenuMoveSound();
    void playMenuSelectSound();

private:
    std::map<std::string, Widget*> widgets;
	TextureManager2 *texMan;
	SpriteRenderer2D *renderer;
	TextPrinter *textPrinter;
	bool hide_ = false;
	bool nativeWidgets = true;

	std::vector<std::string> listMenu;
	bool listMenuShown = false;
	int listMenuSelectedItem = 1;
	float listMenuLineHeight = 0;
	float listMenuTypeSize = 0;
    bool scrollingListMenu = false;

	bool fileSelectorShown = false;
	std::string fileSelectorDir = "";
	std::string fileSelectorExt = "";

	std::vector<DialogPart> dialogParts;

	bool dialogShown = false;
	int dialogSelectedItem = 1;
	float dialogLineHeight = 0;
	float dialogTypeSize = 0;
	bool dialogOKSelected = false;
	bool dialogCancelSelected = false;
	int dialogCharIdx = 0;
	std::string dialogOKExtra = "";

	int delayTimer = 0;

	Widget* selectedWidget = nullptr;

	std::map<std::string, float> fontKern;
    
    std::string menuMoveSound = "";
    std::string menuSelectSound = "";
	int soundTimer = 0;
};

#endif
