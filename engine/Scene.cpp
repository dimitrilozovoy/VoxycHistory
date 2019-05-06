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

#include "Scene.h"
#include "DDLUtils.hpp"

void Scene::save(std::string fname, std::map<std::string, Object*> &objects, std::map<std::string, Shape*> &shapes, TextureManager2 *texMan)
{
    if (!openToWrite(fname))
        return;

    write3Char('S', 'C', ' ');
    write3Char('1', '0', '2');

    // Write skybox
    write6Char('S', 'K', 'Y', 'B', 'O', 'X');
    writeStr(skyboxTextureName);

    // Make lists of what to write; system objects and their shapes do not need to be written
    std::map<std::string, Object*> objectsToWrite;
    std::map<std::string, Shape*> shapesToWrite;

    for(const auto &pair: objects)
    {
        Object *obj = pair.second;

        if (obj != nullptr && !obj->system) {
            objectsToWrite[pair.first] = obj;

            if (obj->shape != nullptr) {
                shapesToWrite[obj->shape->name] = obj->shape;
            }
        }
    }

    // Write shapes
    write6Char('S', 'H', 'A', 'P', 'E', 'S');
    writeInt(shapesToWrite.size());

    for(const auto &pair: shapesToWrite)
    {
        Shape *shape = pair.second;

        shape->save(file);
    }

    // Write objects
    write6Char('O', 'B', 'J', 'C', 'T', 'S');
    writeInt(objectsToWrite.size());

    for(const auto &pair: objectsToWrite)
    {
        Object *obj = pair.second;
        obj->save(file);
    }

    // Write texture spans
    std::map<std::string, Texture *> textures = texMan->getTextures();

    write6Char('T', 'E', 'X', 'S', 'P', 'N');
    writeInt(textures.size());

    for(const auto &pair: textures)
    {
        std::string name = pair.first;
        Texture *tex = pair.second;

		if (tex == nullptr)
			continue;

        clearKVSet();
        addKV("name", tex->name);
        addKV("texSpanX", tex->texSpanX);
        addKV("texSpanY", tex->texSpanX);
		addKV("lightEnabled", tex->lightEnabled);
		addKV("lightRadius", tex->lightRadius);
		addKV("lightr", tex->lightr);
		addKV("lightg", tex->lightg);
		addKV("lightb", tex->lightb);
        writeKVSet();
    }

    closeFile();
}

void Scene::load(std::string fname, std::map<std::string, Object*> &objects, std::map<std::string, Shape*> &shapes, TextureManager2 *texMan)
{
	if (!openToRead(fname))
	{
		Log("Cannot load scene", (char *)fname.c_str());
		return;
	}

    char c1, c2, c3;

    read3Char(c1, c2, c3);

    if (c1 != 'S' || c2 != 'C' || c3 != ' ')
        return;

    read3Char(c1, c2, c3);

    if (c1 == '1' && c2 == '0' && (c3 == '0' || c3 == '1' || c3 == '2'))
    {
        g_common.readSCVersionA = c1;
        g_common.readSCVersionB = c2;
        g_common.readSCVersionC = c3;

        if (!read6CharEquals('S', 'K', 'Y', 'B', 'O', 'X'))
            return;

        skyboxTextureName = readStr();

        if (!read6CharEquals('S', 'H', 'A', 'P', 'E', 'S'))
            return;

        int numShapes = readInt();

        for (int s = 0; s < numShapes; s++)
        {
            Shape *shape = new Shape();
            shape->load(file, texMan);
            shapes[shape->name] = shape;
        }

        if (!read6CharEquals('O', 'B', 'J', 'C', 'T', 'S'))
            return;

        int numObjs = readInt();

        for (int o = 0; o < numObjs; o++)
        {
            Object *obj = new Object();
            obj->load(file);
            objects[obj->name] = obj;
        }
    }

    // Read texture spans
    std::map<std::string, Texture *> textures = texMan->getTextures();

    if (!read6CharEquals('T', 'E', 'X', 'S', 'P', 'N'))
        return;

    int numTextures = readInt();

    for(int t = 0; t < numTextures; t++)
    {
        readKVSet();
        std::string name = getKVStr("name");

        Texture *tex = texMan->find(name);

		if (tex == nullptr)
			continue;

		tex->texSpanX = getKVFloat("texSpanX");
        tex->texSpanY = getKVFloat("texSpanY");
        tex->lightEnabled = getKVBool("lightEnabled");
        tex->lightRadius = getKVFloat("lightradius");
        tex->lightr = getKVFloat("lightr");
        tex->lightg = getKVFloat("lightg");
        tex->lightb = getKVFloat("lightb");
    }

    closeFile();
}
