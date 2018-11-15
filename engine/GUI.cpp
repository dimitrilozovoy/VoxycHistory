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

#include "GUI.h"
#include "platform.h"
#include "DDLUtils.hpp"

void GUI::init(TextureManager2 *texMan, SpriteRenderer2D *renderer, TextPrinter *tp)
{
	this->texMan = texMan;
	this->renderer = renderer;
	this->textPrinter = tp;
}

void GUI::draw()
{
	if (hide_)
		return;

	// Menu background layer
	for(const auto &pair: widgets)
    {
        Widget *wg = pair.second;

		if (wg->visible)
		{	
			switch(wg->type)
			{
                case WG_MENU:
				    drawMenu(wg);
					break;
			}
		}
	}
	
	// Text and icon layer
    for(const auto &pair: widgets)
    {
        Widget *wg = pair.second;

		if (wg->visible)
		{	
			switch(wg->type)
			{
				case WG_BTN:
				    drawBtn(wg);
					break;
                case WG_MENUITEM:
				    drawBtn(wg);
					break;
			}
		}
	}
}

void GUI::drawBtn(Widget *item)
{
	float width = PLAT_GetWindowWidth();
    float height = PLAT_GetWindowHeight();
#ifdef PLATFORM_OPENVR
	float aspect = 1.0;
#else
	float aspect = width / height;
#endif

    if (aspect > 1.0)
	    aspect = 1.0;
//	else
//		aspect = aspect * 0.7;
		
	float sizex = item->size.x;
	float sizey = item->size.y;
	
	if (aspect < 1.0)
	{
		sizex = sizex * 0.5;
		sizey = sizey * 0.5;
	}

	if (item->text != "")
	{
		TextItem ti;
		
//		Log("drawing textitem");
		
		ti.text = item->text;
		ti.position.x = item->position.x;
		ti.position.y = item->position.y;
		ti.size = item->size.x;
		
		textPrinter->drawText(&ti);
	}
	else
	{
	    Texture *tex = texMan->find(item->texture);

	    if (tex != nullptr)
	    {
		    renderer->DrawSprite(item->position.x, item->position.y, sizex, sizey, tex->glTexID, item->color.r, item->color.g, item->color.b, item->color.a);
	    }
	}
}

void GUI::drawMenu(Widget *item)
{
	float width = PLAT_GetWindowWidth();
    float height = PLAT_GetWindowHeight();
#ifdef PLATFORM_OPENVR
	float aspect = 1.0;
#else
	float aspect = width / height;
#endif

    if (aspect > 1.0)
	    aspect = 1.0;
//	else
//		aspect = aspect * 0.7;
		
	float sizex = item->size.x;
	float sizey = item->size.y;
	
/*	if (aspect < 1.0)
	{
		sizex = sizex * 0.5;
		sizey = sizey * 0.5;
	}*/

	Texture *tex = texMan->find(item->texture);

	if (tex != nullptr)
	{
		renderer->DrawSprite(item->position.x, item->position.y, sizex, sizey, tex->glTexID, item->color.r, item->color.g, item->color.b, item->color.a);
	}

}

void GUI::addWg(std::string name, WidgetType type, std::string texture, std::string text, std::string onClickExtra, std::string group, float x, float y, float sizex, float sizey)
{
	if (findWg(name) != nullptr)
		return;

    Widget *w = new Widget();
    w->name = name;
	w->type = type;
    w->size = glm::vec2(sizex, sizey);
    w->position = glm::vec2(x, y);
	w->texture = texture;
	w->text = text;
	w->onClickExtra = onClickExtra;
	w->group = group;

    widgets[name] = w;
}

Widget *GUI::findWg(std::string name)
{
    Widget *wg = widgets[name];
	
    return wg;
}

void GUI::setWgVisible(std::string name, bool val)
{
	Widget *wg = findWg(name);
	
	wg->visible = val;
	
	for(const auto &pair: widgets)
    {
        Widget *obj = pair.second;

		if (obj != nullptr)
		{
			if (obj->group == wg->group)
			{
				obj->visible = val;
			}
		}
    }
}

void GUI::setWgColor(std::string name, float r, float g, float b, float a)
{
	widgets[name]->color = glm::vec4(r, g, b, a);
}

std::string GUI::getOnClickExtraIfClicked(int action, float x, float y, int finger, int count)
{
	float yfromtop = (float)PLAT_GetWindowHeight() - y;
//	yfromtop = yfromtop * 0.85;
	y = (float)PLAT_GetWindowHeight() - yfromtop;
	
//	Log("screenx ", x);
//	Log("screeny ", y);
	
	float clickglx = scrToGlX(x);
	float clickgly = scrToGlY(y);
	
	clickgly -= 0.075;
	
	float wwidth = (float)PLAT_GetWindowWidth();
	float q = 2.0f * (float)x / (float)wwidth - 1.0f;
	
    for(const auto &pair: widgets)
    {
		Widget *item = pair.second;
		
		if (item->visible == true && (item->type == WG_BTN || item->type == WG_MENUITEM))
		{
			if (item->text != "")
			{
		        float textglx = item->position.x;
		        float textgly = item->position.y;
		
/*				Log("textglx", (int)(textglx * 1000));
				Log("textgly", (int)(textgly * 1000));
				Log("clickglx", (int)(clickglx * 1000));
				Log("clickgly", (int)(clickgly * 1000));
*/			
		        if (clickgly > textgly - item->size.y * 2.0
		        && clickgly < textgly + item->size.y * 2.0
		        && clickglx > textglx - (item->text.size() * item->size.x)
		        && clickglx < textglx + (item->text.size() * item->size.x)
				&& item->onClickExtra != "")
		        {
					Log(item->onClickExtra);
		            return item->onClickExtra;
		        }
			}
			else 
			{
				if (clickgly > item->position.y - item->size.y / 1.0 
		        && clickgly < item->position.y + item->size.y / 1.0
		        && clickglx > item->position.x - item->size.x / 1.0
		        && clickglx < item->position.x + item->size.x / 1.0
				&& item->onClickExtra != "")
			    {
				    return item->onClickExtra;
			    }
			}
		}
	}
	
	return "";
}

void GUI::clear()
{
	widgets.clear();
}

void GUI::hide()
{
	hide_ = true;
}

void GUI::show()
{
	hide_ = false;
}
