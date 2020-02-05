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

#ifndef PLATFORMAUDIO_H
#define PLATFORMAUDIO_H

#include "Globals.hpp"
#include <string>
#include <vector>

typedef struct Sound
{
    std::string name;
    int id;
} Sound;

class PlatformAudio
{
public:
	void init();
	int loadSound(char *filename, bool stereo);
	void unloadSound(int sndidx);
	int playSound(std::string filename, bool stereo);
	int playSound(int idx);
	void stopSound(int stream);
	void playTrack(char *filename, bool stereo);
	void checkError(const char *msg);
	void tick();
    Sound *findSound(std::string name);

private:
    std::vector<Sound> sounds;
};

#endif
