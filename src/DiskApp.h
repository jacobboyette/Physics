#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Body.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Body*> bodies;
		Vec2 pushForce = Vec2(0, 0);	// a constant force added for a time period
		float tempTorque = 0;			// a constant torque added for a time period

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
