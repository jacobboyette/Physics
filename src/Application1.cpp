/*
Modified based on the source code from Gustavo Pezzi, co-founder of Pikuma.com
----------- Fall, 2022
*/

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
	running = Graphics::openWindow("A single particle moving within a rectangle");//openWindow();

	//------ set up the position of the coordinate system origin in the window ------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);	

	Particle* aDot = new Particle(0, 0, 10, 0.5);
	particles.push_back(aDot);
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
					pushForce.y = 500;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 500;
				if (event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = -500;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -500;
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
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
					particles[0]->velocity = (particles[0]->position - Graphics::windowToCoordinate(mouseCursor)) * 5;	// simply set up the new velocity
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

	timePreviousFrame = SDL_GetTicks(); // Set the time of the current frame to be used in the next one

	//-------------------------- Apply forces to the particles ----------------------------
	for (auto particle : particles) {
		particle->addForce(pushForce);	// Apply a push force

		//------------------ Apply a friction force -------------------
		Vec2 friction = Force::generateFrictionForce(*particle, 40);
		if(particle->velocity.magnitude() > 0.001)
			particle->addForce(friction);
	}

	for (auto particle : particles) {
		particle->integrate(deltaTime);	// Integrate the acceleration and velocity to estimate the new position
	}

	//--------- object bounces from the boundaries with a reduced velocity magnitude ------------
	for (auto particle : particles) {
		if (Graphics::xPosInWindow(particle->position.x - particle->radius) <= 0) {
			particle->position.x = Graphics::xPosInCoordinate(0) + particle->radius;
			particle->velocity.x *= -0.9;
			particle->velocity.y *= 0.9;
		}
		else if (Graphics::xPosInWindow(particle->position.x + particle->radius) >= Graphics::getWindowWidth()) {
			particle->position.x = Graphics::xPosInCoordinate(Graphics::getWindowWidth()) - particle->radius;
			particle->velocity.x *= -0.9;
			particle->velocity.y *= 0.9;
		}
		if (Graphics::yPosInWindow(particle->position.y - particle->radius) >= Graphics::getWindowHeight()) {
			particle->position.y = Graphics::yPosInCoordinate(Graphics::getWindowHeight()) + particle->radius;
			particle->velocity.y *= 0.9;
			particle->velocity.y *= -0.9;
		}
		else if (Graphics::yPosInWindow(particle->position.y + particle->radius) <= 0) {
			particle->position.y = Graphics::yPosInCoordinate(0) - particle->radius;
			particle->velocity.y *= 0.9;
			particle->velocity.y *= -0.9;
		}
	}
}

//====================================================================================//
// render - render the objects for a frame                                            //
//====================================================================================//
void Application::render() {
	Graphics::clearScreen(0xFF21070F);

	Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference

	//--- draw a line connecting the mouse postion and the moving particle when the mouse if left pressed -----
    if (leftMouseButtonDown) {
		Graphics::drawLine(particles[0]->position.x, particles[0]->position.y, 
			Graphics::xPosInCoordinate(mouseCursor.x), Graphics::yPosInCoordinate(mouseCursor.y), 0xFF0000FF);
    }

	//----------------- render the object for the moment ---------------------------
	Graphics::drawDisk(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFAA3300);

    Graphics::renderFrame();	// force to render
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
