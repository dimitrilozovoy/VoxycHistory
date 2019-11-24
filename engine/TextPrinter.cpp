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

#include "TextPrinter.h"
#include <cstdlib>
#include "platform.h"
#include "DDLUtils.hpp"

void TextPrinter::init(TextureManager2 *texMan, SpriteRenderer2D *renderer)
{
	this->texMan = texMan;
	this->renderer = renderer;
}
 
void TextPrinter::draw()
{
	for (int i = 0; i < items.size(); i++)
	{
		TextItem *item = items[i];

		if (item->visible)
		{
			drawText(item);
		}
	}
}

void TextPrinter::drawText(TextItem *item, bool lightUp, float kern)
{
	if (item->text == "")
		return;

	// Is the sprite visible on the screen?
	if (item->position.x < (-1.0 - item->sizex / 2.0) || item->position.x > (1.0 + item->sizex / 2.0)
		|| item->position.y < (-1.0 - item->sizey / 2.0) || item->position.y >(1.0 + item->sizey / 2.0))
		return;

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
		
	float sizex = item->sizex;
	float sizey = item->sizey;
	
	if (aspect < 1.0)
	{
		sizex = sizex * 0.5;
		sizey = sizey * 0.5;
	}

	for (int i = 0; i < item->text.size(); i++)
	{
		char symbol = item->text.at(i);

		if (item->visible && item->text.at(i) != ' ')
		{
			Texture *tex = texMan->find(item->font + getTextureNameForChar(item->text.at(i)));

			// Light up?
			glm::vec4 color2 = glm::vec4(1.0, 1.0, 1.0, 1.0);
			if (lightUp)
				color2 = glm::vec4(1.5, 1.5, 1.5, 1.0);

//			float kern = 1.2;
			if (kern == 0)
				kern = 1.2;

			if (tex != nullptr && item->text.at(i) != ' ')
			{
				renderer->DrawSprite(item->position.x - (((item->text.size() - 1) * kern) * item->sizex) / 2.0 + (float)i * item->sizex * kern, item->position.y, sizex, sizey, tex->glTexID, item->color.r * color2.r, item->color.g * color2.g, item->color.b * color2.b, item->color.a * color2.a);
			}
		}
	}
}

void TextPrinter::addText(std::string name, std::string text, glm::vec4 position, float size, std::string onClickExtra)
{
	TextItem *t = new TextItem();
	t->name = name;
	t->text = text;
	t->position = position;
	t->sizex = size;
	t->sizey = size;
	t->onClickExtra = onClickExtra;
	t->color = glm::vec4(1.0, 1.0, 1.0, 1.0);

	items.push_back(t);
}

void TextPrinter::setText(std::string name, std::string text)
{
	TextItem *t = findItem(name);

	if (t == nullptr)
		return;

	t->text = text;
}

void TextPrinter::setVisible(std::string name, bool visible)
{
	TextItem *t = findItem(name);

	if (t == nullptr)
		return;

	t->visible = visible;
}

TextItem *TextPrinter::findItem(std::string name)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->name == name)
		{
			return items[i];
		}
	}

	return nullptr;
}

std::string TextPrinter::getTextureNameForChar(char symbol)
{
	std::string result = "";

	switch (symbol)
	{
	case ' ':
		result = " ";
		break;
	case '0':
		result = "0.png";
		break;
	case '1':
		result = "1.png";
		break;
	case '2':
		result = "2.png";
		break;
	case '3':
		result = "3.png";
		break;
	case '4':
		result = "4.png";
		break;
	case '5':
		result = "5.png";
		break;
	case '6':
		result = "6.png";
		break;
	case '7':
		result = "7.png";
		break;
	case '8':
		result = "8.png";
		break;
	case '9':
		result = "9.png";
		break;
	case 'a':
	case 'A':
		result = "a.png";
		break;
	case 'b':
	case 'B':
		result = "b.png";
		break;
	case 'c':
	case 'C':
		result = "c.png";
		break;
	case 'd':
	case 'D':
		result = "d.png";
		break;
	case 'e':
	case 'E':
		result = "e.png";
		break;
	case 'f':
	case 'F':
		result = "f.png";
		break;
	case 'G':
	case 'g':
		result = "g.png";
		break;
	case 'h':
	case 'H':
		result = "h.png";
		break;
	case 'i':
	case 'I':
		result = "i.png";
		break;
	case 'j':
	case 'J':
		result = "j.png";
		break;
	case 'k':
	case 'K':
		result = "k.png";
		break;
	case 'l':
	case 'L':
		result = "l.png";
		break;
	case 'm':
	case 'M':
		result = "m.png";
		break;
	case 'n':
	case 'N':
		result = "n.png";
		break;
	case 'o':
	case 'O':
		result = "o.png";
		break;
	case 'p':
	case 'P':
		result = "p.png";
		break;
	case 'q':
	case 'Q':
		result = "q.png";
		break;
	case 'r':
	case 'R':
		result = "r.png";
		break;
	case 's':
	case 'S':
		result = "s.png";
		break;
	case 't':
	case 'T':
		result = "t.png";
		break;
	case 'u':
	case 'U':
		result = "u.png";
		break;
	case 'v':
	case 'V':
		result = "v.png";
		break;
	case 'w':
	case 'W':
		result = "w.png";
		break;
	case 'x':
	case 'X':
		result = "x.png";
		break;
	case 'y':
	case 'Y':
		result = "y.png";
		break;
	case 'z':
	case 'Z':
		result = "z.png";
		break;
	case '_':
		result = "underscore.png";
		break;
	case '.':
		result = "period.png";
		break;
	case '-':
		result = "dash.png";
		break;
	default:
		result = "empty.png";
	}

	return result;
}

std::string TextPrinter::getOnClickExtraIfClicked(int action, float x, float y, int finger, int count)
{
	float yfromtop = (float)PLAT_GetWindowHeight() - y;
	yfromtop = yfromtop * 0.90;
	y = (float)PLAT_GetWindowHeight() - yfromtop;
	
	float clickglx = scrToGlX(x);
	float clickgly = scrToGlY(y);
	
	float wwidth = (float)PLAT_GetWindowWidth();
	float q = 2.0f * (float)x / (float)wwidth - 1.0f;
		
	for (int i = 0; i < items.size(); i++)
	{
		TextItem *item = items[i];
		
		if (item->visible == true)
		{
		float textglx = items[i]->position.x;
		float textgly = items[i]->position.y;
		
		if (clickgly > textgly - item->sizey / 0.5 
		&& clickgly < textgly + item->sizey / 0.5
		&& clickglx > item->position.x - (item->text.size() * item->sizex) / 2.0
		&& clickglx < item->position.x + (item->text.size() * item->sizey) / 2.0)
		{
		    return item->onClickExtra;
		}
		}
	}
	
	return "";
}

float TextPrinter::scrToGlX(float screenX)
{
	return 2.0f * screenX / (float)PLAT_GetWindowWidth() - 1.0f;
}

float TextPrinter::scrToGlY(float screenY)
{
	return -(2.0f * screenY / (float)PLAT_GetWindowHeight() - 1.0f);
}

float TextPrinter::glToScrX(float glX)
{
	return ((glX + 1.0f) / 2.0f) * (float)PLAT_GetWindowWidth();
}

float TextPrinter::glToScrY(float glY)
{
	return (float)PLAT_GetWindowHeight() - abs(((glY + 1.0f) / 2.0f) * (float)PLAT_GetWindowHeight());
}

void TextPrinter::clear()
{
	items.clear();
}
