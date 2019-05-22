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

//#include <tgmath.h>
#include "DDLUtils.hpp"
#include "VertexLights.h"

void VertexLights::process(Voxels *tm, TextureManager2 *texMan) {

    for (int x = 0; x < tm->getSize(); x++) {
        for (int y = 0; y < tm->getSize(); y++) {
            for (int z = 0; z < tm->getSize(); z++) {

                int tnum = tm->get(x, y, z);
                if (tnum != 0) {
                    std::string texName = tm->getVoxelTexture(tnum);

                    Texture *tex = texMan->find(texName);

                    if (tex->lightEnabled) {
                        float emitr = tex->lightr;
                        float emitg = tex->lightg;
                        float emitb = tex->lightb;

                        int minx = x - (int)tex->lightRadius;
                        int maxx = x + (int)tex->lightRadius;
                        int miny = y - (int)tex->lightRadius;
                        int maxy = y + (int)tex->lightRadius;
                        int minz = z - (int)tex->lightRadius;
                        int maxz = z + (int)tex->lightRadius;

                        for (int xx = minx; xx < maxx; xx++) {
                            for (int yy = miny; yy < maxy; yy++) {
                                for (int zz = minz; zz < maxz; zz++) {
                                    float hyp = abs(hypot(hypot(x - xx, y - yy), z - zz));
                                    float r = emitr + (emitr - 1.0) * (hyp / tex->lightRadius);
                                    float g = emitg + (emitg - 1.0) * (hyp / tex->lightRadius);
                                    float b = emitb + (emitb - 1.0) * (hyp / tex->lightRadius);
                                    tm->setrgba(xx, yy, zz, FloatToUChar(r), FloatToUChar(g), FloatToUChar(b), 127);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

/*	for (int x = 0; x < tm->getSize(); x++)
    {
        for (int y = 0; y < tm->getSize(); y++)
        {
			for (int z = 0; z < tm->getSize(); z++)
            {
//				char r = (char)RandomInt(0, 256);
//				char g = (char)RandomInt(0, 256);
//				char b = (char)RandomInt(0, 256);
				
				char r = (char)(((float)x / (float)(tm->getSize())) * (float)256);
				char g = (char)(((float)y / (float)(tm->getSize())) * (float)256);
				char b = (char)(((float)z / (float)(tm->getSize())) * (float)256);
*/
/*
                char r, g, b;
				
				if (x < tm->getSize() / 2)
				    r = (char)(((float)x / (float)(tm->getSize() / 2)) * (float)256);
				else 
					r = 256 - ((char)(((float)x / (float)(tm->getSize() / 2)) * (float)256));
	
				if (y < tm->getSize() / 2)
				    g = (char)(((float)y / (float)(tm->getSize() / 2)) * (float)256);
				else
				    g = 256 - ((char)(((float)y / (float)(tm->getSize() / 2)) * (float)256));
		
				if (z < tm->getSize() / 2)
				    b = (char)(((float)z / (float)(tm->getSize() / 2)) * (float)256);
				else
				    b = 256 - ((char)(((float)z / (float)(tm->getSize() / 2)) * (float)256));
*/
					
//              char r = 128;
//				char g = 128;
//				char b = 128;
				
/*				tm->setrgb(x, y, z, r, g, b);
		    }
        }
    }*/
}

void VertexLights::processOld(Voxels *tm)
{
	char **insideMap = (char **)malloc((tm->getSize() + 2) * sizeof(char *));

	for (int y = 0; y < tm->getSize(); y++)
	{
		insideMap[y] = (char *)malloc((tm->getSize() + 2) * sizeof(char));
	}

	for (int y = 0; y < tm->getSize(); y++)
		for (int x = 0; x < tm->getSize(); x++)
			insideMap[x][y] = 0;

    for (int x = 0; x < tm->getSize(); x++)
    {
        for (int y = 0; y < tm->getSize(); y++)
        {
            bool foundCeiling = false;
            bool openSpaceUnderCeiling = false;

            for (int z = tm->getSize(); z >= 0; z--)
            {
                if (tm->get(x, y, z) != 0 && !foundCeiling)
                {
                    foundCeiling = true;
                }
                else if (tm->get(x, y, z) == 0 && foundCeiling)
                {
                    //					 openSpaceUnderCeiling = true;
                }
                else if (tm->get(x, y, z) != 0 && foundCeiling)
                {
                    insideMap[x][y] = 1;
                    //					 insideMap[x + 1][y] = 1;
                    //					 insideMap[x + 1][y + 1] = 1;
                    //					 insideMap[x][y + 1] = 1;
                }
            }
        }
    }

    for (int x = tm->getSize() - 1; x > 1; x--)
    {
        for (int y = tm->getSize() - 1; y > 1; y--)
        {
            bool foundCeiling = false;
            bool inside = false;
            bool openSpaceUnderCeiling = false;

            for (int z = 20; z > 0; z--)
            {
                //				 Logv("x", z, "y", y, "z", z);

                int r = 127;
                int g = 127;
                int b = 127;

                if (foundCeiling)
                    inside = true;

                if (tm->get(x, y, z) != 0 && !foundCeiling)
                {
                    foundCeiling = true;
                    inside = true;
                }
                else if (tm->get(x, y, z) == 0 && foundCeiling)
                {
                    openSpaceUnderCeiling = true;
                    tm->setrgba(x, y, z + 1, 77, 77, 77, 127);
                }
                else if (openSpaceUnderCeiling && tm->get(x, y, z) != 0)
                {
                    tm->setrgba(x, y, z + 1, 77, 77, 77, 127);
                }

                // +-----------+
                // |     |     |
                // |     |0,0  |
                // +-----+------
                // |     |     |
                // |     |     |
                // +-----------+

                if (inside && openSpaceUnderCeiling
                    &&
                    (
                            tm->get(x - 1, y, z) != 0
                            ||
                            tm->get(x, y - 1, z) != 0
                    )
                        )
                {
                    r = 88;
                    g = 88;
                    b = 88;
                }

                //				 Logv("*", z);

                tm->setrgba(x, y, z, r, g, b, 127);

                //				 Logv("*", z);

                if (inside && openSpaceUnderCeiling
                    && (tm->get(x, y + 1, z) != 0))
                {
                    tm->setrgba(x, y + 1, z, 77, 77, 77, 127);
                }

                if (inside && openSpaceUnderCeiling
                    && (tm->get(x + 1, y, z) != 0))
                {
                    tm->setrgba(x + 1, y, z, 77, 77, 77, 127);
                }

                if (inside && openSpaceUnderCeiling
                    && (tm->get(x + 1, y + 1, z) != 0))
                {
                    tm->setrgba(x + 1, y + 1, z, 77, 77, 77, 127);
                }

                // if any nearby blocks are empty its a window
/*                if (
                        insideMap[x][y] == 0
                        || insideMap[x - 1][y] == 0
                        || insideMap[x][y - 1] == 0
                        || insideMap[x + 1][y + 1] == 0
                        || insideMap[x + 1][y] == 0
                        || insideMap[x + 1][y - 1] == 0
                        || insideMap[x][y - 1] == 0
                        || insideMap[x - 1][y - 1] == 0)
                {
                    tm->setrgb(x, y, z, 127, 127, 127);
                }*/
            }
        }
    }

    // Do lamp lights

/*    EngineState *state = g_engine.GetState();

    for (int entityIdx = 0; entityIdx < MAX_ENTITIES; entityIdx++)
    {
//        Log("entityIdx", entityIdx);
        if (state->entitiesExist[entityIdx] && state->entitiesActive[entityIdx] && state->entitiesVisible[entityIdx])
        {
            Entity *e = state->get_entity(entityIdx);

            if (e->frameAnimIdx == 60)
            {
                // It's a lamp!

                int tilex = e->x / state->worldSize * 32;
                int tiley = e->y / state->worldSize * 32;
                int tilez = e->z / state->worldSize * 32;

                int minx = e->x - LIGHT_SIZE / 2;
                int miny = e->y - LIGHT_SIZE / 2;
                int minz = e->z - LIGHT_SIZE / 2;

                int maxx = e->x + LIGHT_SIZE / 2;
                int maxy = e->y + LIGHT_SIZE / 2;
                int maxz = e->z + LIGHT_SIZE / 2;

                for (int y = miny; y < maxy; y++)
                {
                    for (int x = minx; x < maxx; x++)
                    {
                        for (int z = minz; z < maxx; z++)
                        {
                            vlm->setr(x, y, z, 127);
                        }
                    }
                }
            }
        }
    }*/

	for (int y = 0; y < tm->getSize(); y++)
	{
		free(insideMap[y]);
	}

	free(insideMap);
}
