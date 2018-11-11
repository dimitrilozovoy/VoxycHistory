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

#ifndef TextPrinter_h
#define TextPrinter_h

#include "TextureManager2.h"
#include "SpriteRenderer2D.hpp"
#include <string>
#include "GLIncludes.h"

typedef struct TextItem
{
	std::string name = "";
	std::string text = "";
	glm::vec4 position = glm::vec4(0.0, 0.0, 0.0, 0.0);
	float size = 0.1;
	bool visible = true;
	std::string onClickExtra;
};

class TextPrinter
{
public:
	void init(TextureManager2 *texMan, SpriteRenderer2D *renderer);
	void draw();
	void drawText(TextItem *item);
	void addText(std::string name, std::string text, glm::vec4 position, float size, std::string onClickExtra);
	void setText(std::string name, std::string text);
	void setVisible(std::string name, bool visible);
	TextItem *findItem(std::string name);
	std::string getTextureNameForChar(char symbol);
    std::string getOnClickExtraIfClicked(int action, float x, float y, int finger, int count);
    float scrToGlX(float screenX);
    float scrToGlY(float screenY);
    float glToScrX(float glX);
    float glToScrY(float glY);
	void clear();

private:
	TextureManager2 *texMan;
	SpriteRenderer2D *renderer;
	std::vector<TextItem*> items;
};

#endif
