/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles;
        Vec2 pushForce = Vec2(0, 0);
        Vec2 mouseCursor = Vec2(0, 0);
        bool leftMouseButtonDown = false;

    public:
        Application() = default;
        ~Application() = default;
        bool isRunning();
        void setup();
        void input();
        void update();
        void render();
        void destroy();
};

#endif
