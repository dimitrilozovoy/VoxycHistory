#include "MainApp.hpp"
#include "Engine.hpp"
#include "Utils.hpp"
#ifndef PLATFORM_GVR
#include "SkyscraperCity.hpp"
#endif

MainApp::MainApp()
{
    m_frameCount = 0;
}

MainApp::~MainApp()
{
}

void MainApp::loadTextures(Engine *engine)
{
    engine->LoadTexture(1, "bkgrass1.png");
    engine->LoadTexture(2, "bkclouds1.png");
    engine->LoadTexture(3, "bkconcrete1.png");
    engine->LoadTexture(4, "ufo.png");
    engine->LoadTexture(5, "ufo.png");
    engine->LoadTexture(6, "bkbrick3.png");
    engine->LoadTexture(7, "bkbrick4.png");
    engine->LoadTexture(8, "bkstone1.png");
    engine->LoadTexture(9, "bkstone2.png");
    engine->LoadTexture(10, "subwaytile.png");
    engine->LoadTexture(11, "fireball.png");
    
    engine->LoadTexture(65, "0.png");
    engine->LoadTexture(66, "1.png");
    engine->LoadTexture(67, "2.png");
    engine->LoadTexture(68, "3.png");
    engine->LoadTexture(69, "4.png");
    engine->LoadTexture(70, "5.png");
    engine->LoadTexture(71, "6.png");
    engine->LoadTexture(72, "7.png");
    engine->LoadTexture(73, "8.png");
    engine->LoadTexture(74, "9.png");
    engine->LoadTexture(75, "a.png");
    engine->LoadTexture(76, "b.png");
    engine->LoadTexture(77, "c.png");
    engine->LoadTexture(78, "d.png");
    engine->LoadTexture(79, "e.png");
    engine->LoadTexture(80, "f.png");
    engine->LoadTexture(81, "g.png");
    engine->LoadTexture(82, "h.png");
    engine->LoadTexture(83, "i.png");
    engine->LoadTexture(84, "j.png");
    engine->LoadTexture(85, "k.png");
    engine->LoadTexture(86, "l.png");
    engine->LoadTexture(87, "m.png");
    engine->LoadTexture(88, "n.png");
    engine->LoadTexture(89, "o.png");
    engine->LoadTexture(90, "p.png");
    engine->LoadTexture(91, "q.png");
    engine->LoadTexture(92, "r.png");
    engine->LoadTexture(93, "s.png");
    engine->LoadTexture(94, "t.png");
    engine->LoadTexture(95, "u.png");
    engine->LoadTexture(96, "v.png");
    engine->LoadTexture(97, "w.png");
    engine->LoadTexture(98, "x.png");
    engine->LoadTexture(99, "y.png");
    engine->LoadTexture(100, "z.png");
}

void MainApp::setUp(Engine *engine)
{
    m_engine = engine;
    
    city.build(engine);
    
    for (int i = 0; i < NUM_UFOS; i++)
        ufoIds[i] = ufos[i].Create(engine);
    
    player.Create(engine);
    physics.Init(engine);
    controls.Create(engine, &player);
    camera.Create(engine, &player, &ufos[0]);
}

void MainApp::tick()
{
    city.tick();
    
    int worldSize = m_engine->GetState()->worldSize;
    
    for (int i = 0; i < NUM_UFOS; i++)
    {
        ufos[i].tick();
    }
    
    player.tick();
    physics.tick();
    camera.tick();
    
    CheckIfWin();
    
    m_frameCount++;
}

void MainApp::CheckIfWin()
{
    EngineState *state = m_engine->GetState();
    
    int enemyCount = 0;
    
    for (int i = 0; i < NUM_UFOS; i++)
    {
        if (ufos[i].IsAlive())
            enemyCount++;
    }
    
    if (enemyCount == 0)
    {
        g_restartGame = true;
    }
}
