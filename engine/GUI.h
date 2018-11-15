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
	NUM_WIDGETTYPES
};

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
};

class GUI
{
public:
	void init(TextureManager2 *texMan, SpriteRenderer2D *renderer, TextPrinter *tp);
    void draw();
	void drawBtn(Widget *item);
	void drawMenu(Widget *item);
	void addWg(std::string name, WidgetType type, std::string texture, std::string text, std::string onClickExtra, std::string group, float x, float y, float sizex, float sizey);
    Widget *findWg(std::string name);
	void setWgVisible(std::string name, bool val);
	void setWgColor(std::string name, float r, float g, float b, float a);
    std::string getOnClickExtraIfClicked(int action, float x, float y, int finger, int count);
	std::map<std::string, Widget*> getWidgets() { return widgets; }
    void clear();
	void hide();
	void show();
	
private:
    std::map<std::string, Widget*> widgets;
	TextureManager2 *texMan;
	SpriteRenderer2D *renderer;
	TextPrinter *textPrinter;
	bool hide_ = false;
};

#endif
