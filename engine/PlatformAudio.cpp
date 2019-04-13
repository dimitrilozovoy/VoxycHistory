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

#include "PlatformAudio.h"
#include "DDLUtils.hpp"
#include "platform.h"

void PlatformAudio::init()
{
}

int PlatformAudio::loadSound(char *fname, bool stereo)
{
    Sound sound;

    sound.name = fname;
    sound.id = PLAT_LoadSound(fname, stereo, g_assetsDir);
    sounds.push_back(sound);

	return 0;
}

void PlatformAudio::unloadSound(int sndidx)
{
}

int PlatformAudio::playSound(std::string filename, bool stereo)
{
    Sound *s = findSound(filename);

    if (s == nullptr)
    {
        loadSound((char *)filename.c_str(), stereo);
        s = findSound(filename);
    }

    playSound(s->id);

	return 0;
}

int PlatformAudio::playSound(int bufferIdx)
{
    return PLAT_PlaySound(bufferIdx);
//    return 0;
}

void PlatformAudio::stopSound(int stream)
{
	return;
}

void PlatformAudio::playTrack(char *filename, bool stereo)
{
    PLAT_PlayTrack(filename, stereo, g_assetsDir);

	return;
}

void PlatformAudio::checkError(const char *msg)
{
}

void PlatformAudio::tick()
{
}

Sound *PlatformAudio::findSound(std::string name)
{
    for (int i = 0; i < sounds.size(); i++)
    {
        if (sounds[i].name == name)
        {
            return &sounds[i];
        }
    }

    return nullptr;
}
