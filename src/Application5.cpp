#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

#include <iostream>

Particle* bigBall = new Particle(0, 0, 1000, 2);

bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("Gravitational attraction from a big ball");//openWindow();

	//----------- set up the position of the coordinate system origin in the window -----------
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
                break;
            case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_RIGHT) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					Particle* aBall = new Particle(Graphics::xPosInCoordinate(x), Graphics::yPosInCoordinate(y), 1.0, 0.5);
					aBall->velocity = Vec2(20, 0);
					particles.push_back(aBall);
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

	//------ Applying a gravitational force between a ball and the big ball
	float minDistance = bigBall->radius + 0.5;
	for (auto particle : particles) {
		Vec2 attraction = Force::generateGravitationalForce(*particle, *bigBall, 10, minDistance, 1000);
		particle->addForce(attraction);
	}

	//--- Integrate the acceleration and velocity to estimate the new position ----
	for (auto particle : particles) {
		particle->integrate(deltaTime);
	}

	//-------- stop the small balls when they are too close to the big ball -------
	for (auto particle : particles)
	{
		float distance = (particle->position - bigBall->position).magnitude();
			
		if (distance <= minDistance)
		{
			particle->velocity = Vec2(0, 0);
		}
	}

	////------------------ bouncing from the boundaries with reduced speed --------------------------
	//static float ratio = 0.8;
	//for (auto particle : particles) {
	//	if (Graphics::xPosInWindow(particle->position.x - particle->radius) <= 0) {
	//		particle->position.x = Graphics::xPosInCoordinate(0) + particle->radius;
	//		particle->velocity.x *= -ratio;
	//		particle->velocity.y *= ratio;
	//	}
	//	else if (Graphics::xPosInWindow(particle->position.x + particle->radius) >= Graphics::getWindowWidth()) {
	//		particle->position.x = Graphics::xPosInCoordinate(Graphics::getWindowWidth()) - particle->radius;
	//		particle->velocity.x *= -ratio;
	//		particle->velocity.y *= ratio;
	//	}
	//	if (Graphics::yPosInWindow(particle->position.y - particle->radius) >= Graphics::getWindowHeight()) {
	//		particle->position.y = Graphics::yPosInCoordinate(Graphics::getWindowHeight()) + particle->radius;
	//		particle->velocity.x *= ratio;
	//		particle->velocity.y *= -ratio;
	//	}
	//	else if (Graphics::yPosInWindow(particle->position.y + particle->radius) <= 0) {
	//		particle->position.y = Graphics::yPosInCoordinate(0) - particle->radius;
	//		particle->velocity.x *= ratio;
	//		particle->velocity.y *= -ratio;
	//	}
	//}
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

	Graphics::drawDisk(bigBall->position.x, bigBall->position.y, bigBall->radius, 0xFF0000FF);

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
