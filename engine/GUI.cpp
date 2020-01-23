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
#if defined PLATFORM_WINDOWS
#include "../thirdparty/dirent/dirent.h"
#endif
#if defined PLATFORM_OSX
#include <dirent.h>
#endif
#include "Audio.h"

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

	// Set selected widget if none
	if (selectedWidget == nullptr)
	{
		selectedWidget = getTopVisibleWidget();
	}

	// Sound timer
	if (soundTimer > 0)
		soundTimer--;
}

void GUI::draw()
{
	if (hide_)
		return;

	// Menu background layer
	for(const auto &pair: widgets)
    {
        Widget *wg = pair.second;
        
        if (wg != nullptr)
        {
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
	}
	
	// Text and icon layer
    for(const auto &pair: widgets) 
	{
        Widget *wg = pair.second;

        if (wg != nullptr)
        {
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
                case WG_SLIDER:
                    drawSlider(wg);
                    break;
			}
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
		ti.sizex = item->size.x;
		ti.sizey = item->size.y;
		ti.color = item->color;
		ti.font = item->font;

		textPrinter->drawText(&ti, item == selectedWidget, fontKern[item->font]);
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

void GUI::drawSlider(Widget *item)
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
    //    else
    //        aspect = aspect * 0.7;
    
    float sizex = item->size.x;
    float sizey = item->size.y;
    
/*    if (aspect < 1.0)
    {
        sizex = sizex * 0.5;
        sizey = sizey * 0.5;
    }*/
    
    Texture *tex = texMan->find(item->texture);
        
    if (tex != nullptr)
    {
        float r, g, b, a = 1.0f;
        
        if (item == selectedWidget)
        {
            r = 1.5f;
            g = 1.5f;
            b = 1.5f;
            a = 1.0f;
        }
        else
        {
            r = item->color.r;
            g = item->color.g;
            b = item->color.b;
            a = item->color.a;
        }
        
        Texture *texBg = texMan->find("white.png");

        // Back of slider
        renderer->DrawSprite(item->position.x, item->position.y, sizex, sizey, texBg->glTexID, r, g, b, 0.2);

        // Slider
        float x = item->position.x - sizex * 0.5 + sizex * item->sliderValue;
        
        renderer->DrawSprite(x, item->position.y, sizey, sizey, tex->glTexID, r, g, b, a);
    }
}

void GUI::addWg(std::string name, WidgetType type, std::string texture, std::string text, std::string onClickExtra, std::string group, float x, float y, float sizex, float sizey)
{
	if (findWg(name) != nullptr)
		return;

    Widget *w = new Widget();
    w->name = name;
	w->type = type;
	w->size.x = sizex;
	w->size.y = sizey;
	w->position = glm::vec2(x, y);
	w->texture = texture;
	w->text = text;
	w->onClickExtra = onClickExtra;
	w->group = group;

    widgets[name] = w;
}

void GUI::remWg(std::string name)
{
	widgets.erase(name);
}

Widget *GUI::findWg(std::string name)
{
    Widget *wg = widgets[name];
	
    return wg;
}

void GUI::setWgVisible(std::string name, bool val)
{
	Widget *wg = findWg(name);
	
	if (wg == nullptr)
		return;
	
	wg->visible = val;
}

void GUI::setWgGroupVisible(std::string group, bool val)
{
    for (const auto &pair: widgets)
    {
        Widget *obj = pair.second;
     
        if (obj != nullptr)
        {
            if (obj->group == group)
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
    if (action == 0 || action == 5)
        return "";

	float yfromtop = (float)PLAT_GetWindowHeight() - y;
//	yfromtop = yfromtop * 0.85;
	y = (float)PLAT_GetWindowHeight() - yfromtop;
	
//	Log("screenx ", x);
//	Log("screeny ", y);
	
	float clickglx = scrToGlX(x);
	float clickgly = scrToGlY(y);
	
//	clickgly -= 0.075;
	
	float wwidth = (float)PLAT_GetWindowWidth();
	float q = 2.0f * (float)x / (float)wwidth - 1.0f;
	
    for(const auto &pair: widgets)
    {
		Widget *item = pair.second;
		
		if (item != nullptr && item->visible == true && (item->type == WG_BTN || item->type == WG_MENUITEM))
		{
			if (item->text != "")
			{
		        float textglx = item->position.x;
		        float textgly = item->position.y;

		        if (clickgly > textgly - item->size.y * 2.0
		        && clickgly < textgly + item->size.y * 2.0
		        && clickglx > textglx - (item->text.size() * item->size.x)
		        && clickglx < textglx + (item->text.size() * item->size.x)
				&& item->onClickExtra != "")
		        {
//					Log(item->onClickExtra);
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
/*			        Log("item" + item->onClickExtra);
    				Log("itemglx", (int)(item->position.x * 100));
	    			Log("itemgly", (int)(item->position.y * 100));
                    Log("clickglx", (int)(clickglx * 100));
                    Log("clickgly", (int)(clickgly * 100));*/

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
    if (sdir == "")
        sdir = g_assetsDir;
    
	if (nativeWidgets)
		PLAT_ShowFileSelector(ext, sdir);
	else
	{
#ifdef PLATFORM_WINDOWS
		clearListMenu();
		addListMenuOption("new file", "");

		DIR *dir;
		struct dirent *ent;

		if ((dir = opendir((const char *)sdir.c_str())) == NULL)
		{
			sdir = GetPath(sdir);
		}
			
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
#ifdef PLATFORM_OSX
        clearListMenu();
        addListMenuOption("new file", "");

        DIR *dir;
        struct dirent *ent;
        
        if ((dir = opendir((const char *)sdir.c_str())) == NULL)
        {
            sdir = GetPath(sdir);
        }
        
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
		if (dialogShown)
			clearDialog();

		const float lineHeight = 1.6;
		float typeSize = 0.1;

		float starty = 0;

		if (listMenu.size() > 10)
        {
			starty = 0.0;
            scrollingListMenu = true;
        }
		else
        {
			starty = (listMenu.size() * typeSize * lineHeight * 0.5);
            scrollingListMenu = false;
        }

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
		if (listMenuShown)
			clearListMenu();

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

        if (fileSelectorShown || scrollingListMenu)
        {
            for (int i = 0; i < listMenu.size(); i++)
            {
                std::string name = "listmenuitem" + ToString(i);
                if (widgets.find(name) != widgets.end())
                    widgets[name]->position.y = 0.0 - listMenuTypeSize * listMenuLineHeight * i + listMenuTypeSize * listMenuLineHeight * listMenuSelectedItem;
            }
        }
        
        playMenuMoveSound();
    }
    else if (dialogShown)
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
        playMenuMoveSound();
	}
    else
    {
	// Widget selection; find all widgets directly below the current selected one and find the closest one
	if (selectedWidget != nullptr)
	{
		std::vector<Widget*> widgetsBelow;

		for (const auto& pair : widgets)
		{
			Widget* wg = pair.second;

			if (wg != nullptr)
			{
				if (wg->visible && (wg->type == WG_MENUITEM || wg->type == WG_SLIDER))
				{
					if (wg != selectedWidget && wg->position.y > selectedWidget->position.y /*&& wg->position.x == selectedWidget->position.x*/)
					{
						widgetsBelow.push_back(wg);
					}
				}
			}
		}

		// Traverse all widgets below and find the closest one
		Widget* closest = nullptr;
		float smallestDist = 0;

		for (auto& wg : widgetsBelow)
		{
			if (wg != nullptr)
			{
				if (wg->visible)
				{
					float dist = abs(wg->position.y - selectedWidget->position.y);

					if (smallestDist == 0 || dist < smallestDist)
					{
						smallestDist = dist;
						closest = wg;
					}
				}
			}
		}

		if (closest != nullptr)
			selectedWidget = closest;
        
        playMenuMoveSound();
	}
    }

	delayTimer = 4;
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
        
        if (fileSelectorShown || scrollingListMenu)
        {
        for (int i = 0; i < listMenu.size(); i++)
        {
            std::string name = "listmenuitem" + ToString(i);
            if (widgets.find(name) != widgets.end())
                widgets[name]->position.y = 0.0 - listMenuTypeSize * listMenuLineHeight * i + listMenuTypeSize * listMenuLineHeight * listMenuSelectedItem;
        }
        }
	}
	else if (dialogShown)
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
    else
    {
	// Widget selection; find all widgets directly below the current selected one and find the closest one
	if (selectedWidget != nullptr)
	{
		std::vector<Widget*> widgetsBelow;

		for (const auto& pair : widgets)
		{
			Widget* wg = pair.second;

			if (wg != nullptr)
			{
                if (wg->visible && (wg->type == WG_MENUITEM || wg->type == WG_SLIDER))
				{
					if (wg != selectedWidget && wg->position.y < selectedWidget->position.y /*&& wg->position.x == selectedWidget->position.x*/)
					{
						widgetsBelow.push_back(wg);
					}
				}
			}
		}

		// Traverse all widgets below and find the closets one
		Widget* closest = nullptr;
		float smallestDist = 0;

		for (auto &wg: widgetsBelow)
		{
			if (wg != nullptr)
			{
				if (wg->visible)
				{
					float dist = abs(wg->position.y - selectedWidget->position.y);

					if (smallestDist == 0 || dist < smallestDist)
					{
						smallestDist = dist;
						closest = wg;
					}
				}
			}
		}

		if (closest != nullptr)
			selectedWidget = closest;
        
        playMenuMoveSound();
	}
    }

	delayTimer = 4;
}

void GUI::left()
{
    if (delayTimer > 0)
    {
        delayTimer--;
        return;
    }

    if (dialogCancelSelected)
	{
		dialogCancelSelected = false;
		dialogOKSelected = true;
	}
    // Widget selection; find all widgets directly to the right of the current selected one and find the closest one
    else if (selectedWidget != nullptr)
    {
        if (selectedWidget->type == WG_SLIDER && selectedWidget->sliderValue > 0.0f)
        {
            selectedWidget->sliderValue -= 0.05f;
			if (selectedWidget->sliderValue < 0.1)
				selectedWidget->sliderValue = 0.0;
            g_common.extraInts[selectedWidget->onClickExtra] = 1;
            g_common.extraFloats["slidervalue"] = selectedWidget->sliderValue;
        }
        else
        {
            
        std::vector<Widget*> widgetsBelow;
        
        for (const auto& pair : widgets)
        {
            Widget* wg = pair.second;
            
            if (wg != nullptr)
            {
                if (wg->visible && wg->type == WG_MENUITEM)
                {
                    if (wg != selectedWidget && wg->position.x < selectedWidget->position.x && wg->position.y == selectedWidget->position.y)
                    {
                        widgetsBelow.push_back(wg);
                    }
                }
            }
        }
        
        // Traverse all widgets to the right and find the closest one
        Widget* closest = nullptr;
        float smallestDist = 0;
        
        for (auto& wg : widgetsBelow)
        {
            if (wg != nullptr)
            {
                if (wg->visible)
                {
                    float dist = abs(wg->position.x - selectedWidget->position.x);
                    
                    if (smallestDist == 0 || dist < smallestDist)
                    {
                        smallestDist = dist;
                        closest = wg;
                    }
                }
            }
        }
        
        if (closest != nullptr)
            selectedWidget = closest;
            
        delayTimer = 4;
        
        playMenuMoveSound();
        }
    }
}

void GUI::right()
{
    if (delayTimer > 0)
    {
        delayTimer--;
        return;
    }

    if (dialogOKSelected)
	{
		dialogCancelSelected = true;
		dialogOKSelected = false;
	}
    // Widget selection; find all widgets directly to the right of the current selected one and find the closest one
    else if (selectedWidget != nullptr)
    {
        if (selectedWidget->type == WG_SLIDER && selectedWidget->sliderValue < 1.0f)
        {
            selectedWidget->sliderValue += 0.05f;
			if (selectedWidget->sliderValue > 0.9)
				selectedWidget->sliderValue = 1.0;
			g_common.extraInts[selectedWidget->onClickExtra] = 1;
            g_common.extraFloats["slidervalue"] = selectedWidget->sliderValue;
        }
        else
        {
            
        std::vector<Widget*> widgetsBelow;
        
        for (const auto& pair : widgets)
        {
            Widget* wg = pair.second;
            
            if (wg != nullptr)
            {
                if (wg->visible && wg->type == WG_MENUITEM)
                {
                    if (wg != selectedWidget && wg->position.x > selectedWidget->position.x && wg->position.y == selectedWidget->position.y)
                    {
                        widgetsBelow.push_back(wg);
                    }
                }
            }
        }
        
        // Traverse all widgets to the right and find the closest one
        Widget* closest = nullptr;
        float smallestDist = 0;
        
        for (auto& wg : widgetsBelow)
        {
            if (wg != nullptr)
            {
                if (wg->visible)
                {
                    float dist = abs(wg->position.x - selectedWidget->position.x);
                    
                    if (smallestDist == 0 || dist < smallestDist)
                    {
                        smallestDist = dist;
                        closest = wg;
                    }
                }
            }
        }
        
        if (closest != nullptr)
            selectedWidget = closest;
            
        delayTimer = 4;
        
        playMenuMoveSound();
        }
    }
}

void GUI::enter()
{
	if (fileSelectorShown == true)
	{
#if defined PLATFORM_WINDOWS || defined PLATFORM_OSX
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
    
    playMenuSelectSound();
}

void GUI::escape()
{
	if (fileSelectorShown == true)
	{
		g_common.extraStrings["fileselected"] = "";
		clearListMenu();
		fileSelectorShown = false;
	}
	else if (listMenuShown)
	{
		g_common.extraStrings["listmenuoptionclicked"] = "";
		clearListMenu();
		listMenuShown = false;
	}
	else if (dialogShown)
	{
		clearDialog();
		dialogShown = false;
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

Widget *GUI::getTopVisibleWidget()
{
	float topY = -1.0f;
	Widget *topWidget = nullptr;

	for (const auto& pair : widgets)
	{
		Widget* wg = pair.second;

		if (wg != nullptr)
		{
			if (wg->visible)
			{
				if (wg->position.y > topY)
				{
					topWidget = wg;
					topY = wg->position.y;
				}
			}
		}
	}

	return topWidget;
}

void GUI::setFontKern(std::string font, float kern)
{
	fontKern[font] = kern;
}

bool GUI::isInternallyGeneratedShown()
{
    return listMenuShown || dialogShown;
}

void GUI::setMenuSounds(std::string move, std::string select)
{
    menuMoveSound = move;
    menuSelectSound = select;
}

void GUI::playMenuMoveSound()
{
	if (soundTimer > 0)
	{
		return;
	}

    // Play sound
    
    std::string ext = "";
    
#ifdef PLATFORM_IOS
    ext = "mp3";
#else
    ext = "ogg";
#endif
    
    g_audio.playSound("menumove." + ext, false);

	soundTimer = 10;
}

void GUI::playMenuSelectSound()
{
	if (soundTimer > 0)
	{
		return;
	}
	
	// Play sound
    
    std::string ext = "";
    
#ifdef PLATFORM_IOS
    ext = "mp3";
#else
    ext = "ogg";
#endif
    
    g_audio.playSound("menuselect." + ext, false);

	soundTimer = 10;
}
