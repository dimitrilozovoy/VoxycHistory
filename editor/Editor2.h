#pragma once

#include "../engine/Engine2.h"

class Editor2
{
public:
    void init();
	void load();
	void tick();
	void touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2, int actionIndex, bool ndcCoods = false);
	float scrToGlX(float screenX);
    float scrToGlY(float screenY);
    float glToScrX(float glX);
    float glToScrY(float glY);

private:
    Engine2 *engine;
	
	bool modified = false;
	int screenWidth = 1;
	int screenHeight = 1;
	std::string fileSelectorAction = "";
};
