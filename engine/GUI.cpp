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
#ifdef PLATFORM_WINDOWS
#include "../thirdparty/dirent/dirent.h"
#endif

void GUI::init(TextureManager2 *texMan, SpriteRenderer2D *renderer, TextPrinter *tp)
{
	this->texMan = texMan;
	this->renderer = renderer;
	this->textPrinter = tp;
}

void GUI::tick()
{
	if (listMenuShown)
	{
		int i = 1;

		for (int i = 0; i < listMenu.size(); i++)
		{
			std::string name = "listmenuitem" + ToString(i);

			if (i == listMenuSelectedItem - 1)
				setWgColor(name, 0.5, 0.5, 2.0, 1.0);
			else
				setWgColor(name, 1.0, 1.0, 1.0, 1.0);
		}
	}

	if (dialogShown)
	{
		int i = 1;

		for (int i = 0; i < dialogParts.size(); i++)
		{
			std::string caption = "caption" + ToString(i);
			std::string value = "value" + ToString(i);

			widgets[value]->text = dialogParts[i].value;

			if (i == dialogSelectedItem - 1)
				setWgColor(value, 0.5, 0.5, 2.0, 1.0);
			else
				setWgColor(value, 1.0, 1.0, 1.0, 1.0);
		}

		if (dialogOKSelected)
		{
			setWgColor("ok", 0.5, 0.5, 2.0, 1.0);
		}
		else
			setWgColor("ok", 1.0, 1.0, 1.0, 1.0);

		if (dialogCancelSelected)
		{
			setWgColor("cancel", 0.5, 0.5, 2.0, 1.0);
		}
		else
			setWgColor("cancel", 1.0, 1.0, 1.0, 1.0);
	}

	// New filename dialog; need to add folder
	if (g_common.extraInts["newfilenameselected"] == 1)
	{
		g_common.extraStrings["fileselected"] = fileSelectorDir + "/" + g_common.extraStrings["newfilename"];
		g_common.extraStrings["newfilenameselected"] = "";
		g_common.extraStrings["newfilename"] = "";
	}
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
				case WG_TEXT:
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
		ti.color = item->color;
		
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
	if (widgets.find(name) != widgets.end())
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

void GUI::showFileSelector(std::string ext, std::string sdir)
{
	if (nativeWidgets)
		PLAT_ShowFileSelector(ext, sdir);
	else
	{
#ifdef PLATFORM_WINDOWS
		if (sdir == "")
			sdir = g_assetsDir;

		clearListMenu();

		addListMenuOption("new file", "");

		DIR *dir;
		struct dirent *ent;

		if ((dir = opendir((const char *)sdir.c_str())) != NULL)
		{
			while ((ent = readdir(dir)) != NULL)
			{
				std::string fname = ent->d_name;

				if (fname != ".")
				{
					addListMenuOption(ent->d_name, "");
				}
			}

			closedir(dir);
		}
		else {
			Log("Directory parsing error");
		}

		showListMenuInDialog("", "");
		fileSelectorShown = true;
		fileSelectorDir = sdir;
		fileSelectorExt = ext;
#endif
	}
}

void GUI::drawFileSelector()
{

}

void GUI::clearListMenu()
{
	if (nativeWidgets)
		PLAT_ClearListMenu();
	else
	{
		for (int i = 0; i < listMenu.size(); i++)
		{
			std::string name = "listmenuitem" + ToString(i);
			widgets.erase(name);
		}

		listMenu.clear();
		listMenuShown = false;
		fileSelectorShown = false;
	}
}

void GUI::addListMenuOption(std::string title, std::string desc)
{
	if (nativeWidgets)
		PLAT_AddListMenuOption(title, desc);
	else
	{
		listMenu.push_back(title);
	}
}

void GUI::showListMenuInDialog(std::string title, std::string options)
{
	if (nativeWidgets)
		PLAT_ShowListMenuInDialog(title, options);
	else
	{
		const float lineHeight = 1.6;
		float typeSize = 0.1;

		float starty = 0;

		if (listMenu.size() > 10)
			starty = 0.0;
		else
			starty = (listMenu.size() * typeSize * lineHeight * 0.5);

		for (int i = 0; i < listMenu.size(); i++)
		{
			std::string name = "listmenuitem" + ToString(i);
			std::string title = listMenu[i];

			addWg(name, WG_MENUITEM, "", title, title, "", 0.0, starty - typeSize * lineHeight * i, typeSize * 0.8, typeSize * 0.8);
		}

		listMenuShown = true;
		listMenuSelectedItem = 1;
		listMenuLineHeight = lineHeight;
		listMenuTypeSize = typeSize;
	}
}

void GUI::drawListMenu()
{

}

void GUI::showText(std::string text)
{
	if (nativeWidgets)
		PLAT_ShowText(text);
	else
	{

	}
}

void GUI::drawText()
{

}

void GUI::showLongText(std::string text)
{
	if (nativeWidgets)
		PLAT_ShowLongText(text);
	else
	{

	}
}

void GUI::drawLongText()
{

}

void GUI::clearDialog()
{
	if (nativeWidgets)
		PLAT_ClearDialog();
	else
	{
		for (int i = 0; i < dialogParts.size(); i++)
		{
			std::string captionName = "caption" + ToString(i);
			std::string valueName = "value" + ToString(i);

			widgets.erase(captionName);
			widgets.erase(valueName);
		}

		std::string captionName = "ok";
		std::string valueName = "cancel";

		widgets.erase(captionName);
		widgets.erase(valueName);

		dialogParts.clear();
		dialogShown = false;
	}
}

void GUI::addDialogPart(std::string caption, std::string defaultValue, std::string extraKey)
{
	if (nativeWidgets)
		PLAT_AddDialogPart(caption, defaultValue, extraKey);
	else
	{
		DialogPart part;

		part.caption = caption;
		part.value = defaultValue;
		part.extraKey = extraKey;

		dialogParts.push_back(part);
	}
}

void GUI::showDialog(std::string title, std::string okText, std::string cancelText, std::string okExtra)
{
	if (nativeWidgets)
		PLAT_ShowDialog(title, okText, cancelText, okExtra);
	else
	{
		const float lineHeight = 1.6;
		float typeSize = 0.1;

		float starty = 0;

		if (dialogParts.size() > 10)
			starty = 0.0;
		else
			starty = (dialogParts.size() * typeSize * lineHeight * 0.5);

		for (int i = 0; i < dialogParts.size(); i++)
		{
			DialogPart part = dialogParts[i];

			std::string captionName = "caption" + ToString(i);
			std::string valueName = "value" + ToString(i);

			addWg(captionName, WG_TEXT, "", part.caption, "", "", -0.5, starty - typeSize * lineHeight * i, typeSize * 0.8, typeSize * 0.8);
			addWg(valueName, WG_TEXT, "", part.value, "", "", 0.5, starty - typeSize * lineHeight * i, typeSize * 0.8, typeSize * 0.8);
		}

		addWg("ok", WG_TEXT, "", okText, "", "", -0.5, -starty - lineHeight * typeSize, typeSize * 0.8, typeSize * 0.8);
		addWg("cancel", WG_TEXT, "", cancelText, "", "", 0.5, -starty - lineHeight * typeSize, typeSize * 0.8, typeSize * 0.8);

		dialogShown = true;
		dialogSelectedItem = 1;
		dialogLineHeight = lineHeight;
		dialogTypeSize = typeSize;
		dialogOKSelected = false;
		dialogCancelSelected = false;
		dialogOKExtra = okExtra;
	}
}

void GUI::drawDialog()
{

}

void GUI::up()
{
	if (delayTimer > 0)
	{
		delayTimer--;
		return;
	}

	if (listMenuShown)
	{
		listMenuSelectedItem--;

		if (listMenuSelectedItem <= 0)
			listMenuSelectedItem = listMenu.size();
	}

	if (dialogShown)
	{
		if (dialogCancelSelected)
		{
			dialogCancelSelected = false;
			dialogOKSelected = false;
			dialogSelectedItem = dialogParts.size();
		}
		else if (dialogOKSelected)
		{
			dialogCancelSelected = false;
			dialogOKSelected = false;
			dialogSelectedItem = dialogParts.size();
		}
		else
		{
			dialogSelectedItem--;

			if (dialogSelectedItem <= 0)
				dialogSelectedItem = dialogParts.size();
		}

		dialogCharIdx = 0;
	}

	if (fileSelectorShown)
	{
		for (int i = 0; i < listMenu.size(); i++)
		{
			std::string name = "listmenuitem" + ToString(i);
			if (widgets.find(name) != widgets.end())
				widgets[name]->position.y = 0.0 - listMenuTypeSize * listMenuLineHeight * i + listMenuTypeSize * listMenuLineHeight * listMenuSelectedItem;
		}
	}

//	delayTimer = 5;
	delayTimer = 10 - (listMenu.size() / 20);
	if (delayTimer < 0)
		delayTimer = 0;
}

void GUI::down()
{
	if (delayTimer > 0)
	{
		delayTimer--;
		return;
	}

	if (listMenuShown)
	{
		listMenuSelectedItem++;

		if (listMenuSelectedItem > listMenu.size())
			listMenuSelectedItem = 1;
	}

	if (dialogShown)
	{
		dialogSelectedItem++;

		if (dialogSelectedItem == dialogParts.size() + 1)
		{
			dialogOKSelected = true;
			dialogCancelSelected = false;
		}

		if (dialogSelectedItem == dialogParts.size() + 2)
		{
			dialogOKSelected = false;
			dialogCancelSelected = true;
		}

		dialogCharIdx = 0;
	}

	if (fileSelectorShown)
	{
		for (int i = 0; i < listMenu.size(); i++)
		{
			std::string name = "listmenuitem" + ToString(i);
			if (widgets.find(name) != widgets.end())
				widgets[name]->position.y = 0.0 - listMenuTypeSize * listMenuLineHeight * i + listMenuTypeSize * listMenuLineHeight * listMenuSelectedItem;
		}
	}

//	delayTimer = 5;
	delayTimer = 10 - (listMenu.size() / 20);
	if (delayTimer < 0)
		delayTimer = 0;
}

void GUI::left()
{
	if (dialogCancelSelected)
	{
		dialogCancelSelected = false;
		dialogOKSelected = true;
	}
}

void GUI::right()
{
	if (dialogOKSelected)
	{
		dialogCancelSelected = true;
		dialogOKSelected = false;
	}
}

void GUI::enter()
{
	if (fileSelectorShown == true)
	{
#ifndef PLATFORM_ANDROID
		std::string fname = listMenu[listMenuSelectedItem - 1];

		// Check if this is a dir
		std::string fullFName = fileSelectorDir + "/" + fname;
		DIR *dir = opendir(fullFName.c_str());

		if (dir != nullptr)
		{
			closedir(dir);
			showFileSelector(fileSelectorExt, fullFName);
		}
		else if (fname == "..")
		{
			clearListMenu();
			std::string newDir = "";

			auto pos = fileSelectorDir.rfind("/");

			if (pos != std::string::npos) {
				newDir = fileSelectorDir.substr(0, pos);
			}

			showFileSelector(fileSelectorExt, newDir);
		}
		else if (fname == "new file")
		{
			clearListMenu();
			clearDialog();
			addDialogPart("filename", "", "newfilename");
			showDialog("new file", "ok", "cancel", "newfilenameselected");
		}
		else
		{
			g_common.extraStrings["fileselected"] = fullFName;
			clearListMenu();
		}
#endif
	}
	else if (listMenuShown)
	{
		g_common.extraStrings["listmenuoptionclicked"] = listMenu[listMenuSelectedItem - 1];
		clearListMenu();
	}
	else if (dialogShown)
	{
		if (dialogOKSelected)
		{
			for (int i = 0; i < dialogParts.size(); i++)
			{
				g_common.extraStrings[dialogParts[i].extraKey] = dialogParts[i].value;
			}

			g_common.extraInts[dialogOKExtra] = 1;

			clearDialog();
		}

		if (dialogCancelSelected)
		{
			clearDialog();
		}
	}
}

void GUI::charEntered(char c)
{
	static int timer = 0;

	if (timer > 0)
	{
		timer--;
		return;
	}
	else
	{
		if (dialogShown)
		{
			if (dialogSelectedItem <= dialogParts.size())
			{
				std::string value = dialogParts[dialogSelectedItem - 1].value;

				if (dialogCharIdx > value.size())
					value = value + c;
				else if (dialogCharIdx >= 0)
					value[dialogCharIdx] = c;

				dialogParts[dialogSelectedItem - 1].value = value;

				dialogCharIdx++;
			}
		}

		timer = 10;
	}
}

void GUI::backspace()
{
	static int timer = 0;

	if (timer > 0)
	{
		timer--;
		return;
	}
	else
	{
		if (dialogShown)
		{
			if (dialogSelectedItem <= dialogParts.size())
			{
				std::string value = dialogParts[dialogSelectedItem - 1].value;

				if (dialogCharIdx > 0)
					value = value.substr(0, value.size() - 1);
				else
					value = "";

				dialogParts[dialogSelectedItem - 1].value = value;

				dialogCharIdx--;
			}
		}

		timer = 10;
	}
}

bool GUI::nonNativeWidgetsShown()
{
	if (listMenuShown || dialogShown)
		return true;
	else
		return false;
}

void GUI::clearNonNativeWidgets()
{
	clearListMenu();
	clearDialog();
}
