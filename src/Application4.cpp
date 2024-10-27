#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

#include <iostream>

bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("Particles with gravity");//openWindow();

	//----------- set up the position of the coordinate system origin in the window -----------------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);	
}

//====================================================================================//
// input - handle the events                                                          //
//====================================================================================//
void Application::input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
				if (event.key.keysym.sym == SDLK_UP)
					pushForce.y = 50;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50;
				if (event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = -50;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_RIGHT) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					Particle* aDot = new Particle(Graphics::xPosInCoordinate(x), Graphics::yPosInCoordinate(y), 1.0, 0.5);
					float v_x, v_y;
					v_x = (float)(rand() % 50 - 25);
					v_y = (float)(rand() % 50 - 25);
					aDot->velocity = Vec2(v_x, v_y);
					particles.push_back(aDot);
				}
                break;
        }
    }
}

//====================================================================================//
// update - update the properties of the objects                                      //
//====================================================================================//
void Application::update() {
	//--------- Wait some time until reaching the target frame time in milliseconds -----
	static int timePreviousFrame;
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
	if (timeToWait > 0)
		SDL_Delay(timeToWait);

	//---------------Calculate the deltatime in seconds ---------------------------
	float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
	if (deltaTime > 0.016)
		deltaTime = 0.016;

	timePreviousFrame = SDL_GetTicks();	// Set the time of the current frame to be used in the next one

	//-------------------------- Apply forces to the particles ----------------------------
	for (auto particle : particles) {
		particle->addForce(pushForce);	// apply push force
	}

	//------------- Integrate the acceleration and velocity to estimate the new position ---------------
	for (auto particle : particles) {
		particle->integrate(deltaTime, Vec2(0, -10));
	}

	//------------------ bouncing from the boundaries with reduced speed --------------------------
	static float ratio = 0.8;
	for (auto particle : particles) {
		if (Graphics::xPosInWindow(particle->position.x - particle->radius) <= 0) {
			particle->position.x = Graphics::xPosInCoordinate(0) + particle->radius;
			particle->velocity.x *= -ratio;
			particle->velocity.y *= ratio;
		}
		else if (Graphics::xPosInWindow(particle->position.x + particle->radius) >= Graphics::getWindowWidth()) {
			particle->position.x = Graphics::xPosInCoordinate(Graphics::getWindowWidth()) - particle->radius;
			particle->velocity.x *= -ratio;
			particle->velocity.y *= ratio;
		}
		if (Graphics::yPosInWindow(particle->position.y - particle->radius) >= Graphics::getWindowHeight()) {
			particle->position.y = Graphics::yPosInCoordinate(Graphics::getWindowHeight()) + particle->radius;
			particle->velocity.x *= ratio;
			particle->velocity.y *= -ratio;
		}
		else if (Graphics::yPosInWindow(particle->position.y + particle->radius) <= 0) {
			particle->position.y = Graphics::yPosInCoordinate(0) - particle->radius;
			particle->velocity.x *= ratio;
			particle->velocity.y *= -ratio;
		}
	}
}

//====================================================================================//
// render - render the objects for a frame                                            //
//====================================================================================//
void Application::render() {
	Graphics::clearScreen(0xFF21070F);

	Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference

	for (auto particle : particles) {
		Graphics::drawDisk(particle->position.x, particle->position.y, particle->radius, 0xFF00FFFF);
	}

    Graphics::renderFrame();
}

//====================================================================================//
// desdroy - release the resources                                                    //
//====================================================================================//
void Application::destroy() {
    for (auto particle: particles) {
        delete particle;
    }
    Graphics::CloseWindow();
}
