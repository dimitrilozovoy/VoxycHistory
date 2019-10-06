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

#ifndef FATELESS_TEXTUREMANAGER_H
#define FATELESS_TEXTUREMANAGER_H

#include <string>
#include <vector>
#include "../thirdparty/glm/glm.hpp"
#include <map>

typedef struct Texture
{
    std::string name;
    int glTexID;
	float texSpanX = 1.0;
	float texSpanY = 1.0;
	bool lightEnabled = false;
	float lightRadius = 0.0;
	float lightr = 1.0;
	float lightg = 1.0;
	float lightb = 1.0;
} Texture;

class TextureManager2
{
public:
    Texture *find(std::string name);
    void load(std::string name, bool external = false);
	void add(std::string name, int glTexId);
    void release();
	void checkGLError(char *tag);
    std::map<std::string, Texture *> getTextures() { return textures; };

private:
	std::map<std::string, Texture *> textures;
};

#endif //FATELESS_TEXTUREMANAGER_H
