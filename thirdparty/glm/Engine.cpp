//
//  Engine.cpp
//  Drumstr
//
//  Created by Dimitri Lozovoy on 12/28/15.
//
//

#include "Globals.hpp"
#include "Engine.hpp"
#include <pthread.h>
#include "tilemap.h"
#include "entity.h"
#include "drawdata.h"
#include "enginestate.h"
#include "transformer.h"
#include "platform.h"
//#include "io.h"
#include <iostream>

#if defined USE_OPENGL_2 || defined USE_OPENGL_3
#include <GL/glew.h>
#endif
#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#endif
#ifdef USE_OPENGL
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#endif

#include "lodepng.hpp"
#include "Utils.hpp"

#define SKYBOX_NUM_SIDES 6

//pthread_t g_transformerThread;

Engine::Engine()
{
    m_textureIDsGenerated = false;
}

Engine::~Engine()
{
}

void Engine::Start()
{
    Log("engine_start()");
    
    m_state.init();
    m_transformer.init();
    
    if (MULTI_THREADED)
    {
        //        pthread_create(&g_transformerThread, NULL, transform_worker, 0);
    }
        
    for (int i = 0; i < MAX_TILES; i++)
        m_glTexIDsUsed[i] = false;
}

void Engine::RenderFrame()
{
    m_transformer.transform(&m_state);
}

DrawData *Engine::GetDrawData()
{
    return m_transformer.get_draw_data();
}

int Engine::GetGLTexID(int tileIdx)
{
    if (!m_textureIDsGenerated)
        return 1;
    else
        return m_glTexIDs[tileIdx];
}

void Engine::Stop()
{
    if (m_textureIDsGenerated == true)
        ReleaseTextureIDs();
    
    m_transformer.Free();
    m_state.free();
}

void Engine::SetTileColor(int tileId, float r, float g, float b, float a)
{
    m_tileColors[tileId][0] = r;
    m_tileColors[tileId][1] = g;
    m_tileColors[tileId][2] = b;
    m_tileColors[tileId][3] = a;
}

float* Engine::GetTileColor(int tileId)
{
    return (float *)m_tileColors[tileId];
}

float Engine::GetTileColorR(int tileId)
{
    return m_tileColors[tileId][0];
}

float Engine::GetTileColorG(int tileId)
{
    return m_tileColors[tileId][1];
}

float Engine::GetTileColorB(int tileId)
{
    return m_tileColors[tileId][2];
}

float Engine::GetTileColorA(int tileId)
{
    return m_tileColors[tileId][3];
}

void Engine::LoadTexture(int tileId, char *filename)
{
#ifdef PLATFORM_OSX
    char fullFilename[MAX_STR_LEN];
    
    if (m_textureIDsGenerated == false)
        GenerateTextureIDs();
    
    printFullResourceFilename(filename, fullFilename);
    
    // Load file and decode image.
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, fullFilename);
    
    // If there's an error, display it.
    if(error != 0)
    {
        Log("Error loading PNG: ", (char *)lodepng_error_text(error));
    }
    
    m_glTexIDsUsed[tileId] = true;
    
    // Enable the texture for OpenGL.
    checkError();
    glEnable(GL_TEXTURE_2D);
    
//    glActiveTexture(GL_TEXTURE0);
//    checkError();
    
    glBindTexture(GL_TEXTURE_2D, m_glTexIDs[tileId]);
    checkError();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    checkError();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    checkError();
    
    //    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    //    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    checkError();
#endif
}

/*void Engine::SetTileColor(int tileId, float r, float g, float b, float a)
{
    m_tileReds[tileId] = r;
    m_tileGreens[tileId] = g;
    m_tileBlues[tileId] = b;
    m_tileAlphas[tileId] = a;
}*/

void Engine::GenerateTextureIDs()
{
#ifdef USE_OPENGL
    m_glTexIDs = (int *)malloc(sizeof(int) * MAX_TEXTURES);
    glGenTextures(MAX_TEXTURES, (GLuint *)m_glTexIDs);
    m_textureIDsGenerated = true;
#endif
}

void Engine::ReleaseTextureIDs()
{
#ifdef USE_OPENGL
    free(m_glTexIDs);
    glDeleteTextures(MAX_TEXTURES, (GLuint *)m_glTexIDs);
#endif
}

int Engine::LoadTilemap(int entity, char *filename)
{
    return 0;
}

int Engine::NewTilemap(int tilemapSize, int tileSize, int textureSpan, int island)
{
    if (m_mainTileSize == DEFAULT_MAIN_TILE_SIZE)
        m_mainTileSize = tileSize;
    
    int newEntityIdx = m_state.new_entity();
    int newTilemapIdx = m_state.new_tilemap(tilemapSize, tileSize, island, textureSpan);
    m_state.entities[newEntityIdx].tilemapIdx = newTilemapIdx;
    
    return newEntityIdx;
}

int Engine::NewInvisible()
{
    int newEntityIdx = m_state.new_entity();
    
    m_state.entities[newEntityIdx].visualizationType = VIS_NONE;
    
    return newEntityIdx;
}

void Engine::SetMain(int entity)
{
    m_state.entities[entity].SetMain();
    m_state.mainEntityIdx = entity;
}

int Engine::NewShape(Shapes shape)
{
    int newEntityIdx = m_state.new_entity();
    m_state.entities[newEntityIdx].visualizationType = VIS_SHAPE;
    m_state.entities[newEntityIdx].visualizationSubtype = shape;
    return 0;
}

void Engine::SetTile(int entity, int x, int y, int z, int tile)
{
    int tilemap = m_state.entities[entity].tilemapIdx;
    
    m_state.tilemaps[tilemap].set(x, y, z, tile);
}

void Engine::SetTileOffset(int entity, int x, int y, int z, float offsetX, float offsetY, float offsetZ)
{
    int tilemap = m_state.entities[entity].tilemapIdx;
    
    m_state.tilemaps[tilemap].set_offset_x(x, y, z, offsetX);
    m_state.tilemaps[tilemap].set_offset_y(x, y, z, offsetY);
    m_state.tilemaps[tilemap].set_offset_z(x, y, z, offsetZ);
}

void Engine::SetPosition(int entity, float x, float y, float z, float yaw, float pitch, float roll)
{
    m_state.entities[entity].x = x;
    m_state.entities[entity].y = y;
    m_state.entities[entity].z = z;
    m_state.entities[entity].yaw = yaw;
    m_state.entities[entity].pitch = pitch;
    m_state.entities[entity].roll = roll;
}

void Engine::SetSize(int entity, float size)
{
    m_state.entities[entity].size = size;
}

void Engine::SetCamera(float x, float y, float z, float yaw, float pitch, float roll)
{
    m_state.camera.x = x;
    m_state.camera.y = y;
    m_state.camera.z = z;
    m_state.camera.yaw = yaw;
    m_state.camera.pitch = pitch;
    m_state.camera.roll = roll;
}

Entity *Engine::GetCamera()
{
    return &m_state.camera;
}

void checkError()
{
#ifdef USE_OPENGL
    GLenum err = glGetError();
    
    switch (err)
    {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_ENUM:
            Log("GL error GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            Log("GL error GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
            Log("GL error GL_INVALID_OPERATION");
            break;
#ifndef USE_OPENGL_3
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            Log("GL error GL_INVALID_FRAMEBUFFER_OPERATION");
            break;
#endif
        case GL_OUT_OF_MEMORY:
            Log("GL error GL_OUT_OF_MEMORY");
            break;
    }
#endif
}
