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
	running = Graphics::openWindow("Two particles attracting each other");//openWindow();

	//----------- set up the position of the coordinate system origin in the window -----------------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);	

	Particle* dot1 = new Particle(-10, 0, 100, 1);
	dot1->velocity = { 0, 5 };
	particles.push_back(dot1);
	Particle* dot2 = new Particle(10, 0, 100, 1);
	dot2->velocity = { 0, -5 };
	particles.push_back(dot2);
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
					Vec2 impulseDirection = (particles[0]->position - Graphics::windowToCoordinate(mouseCursor)).unitVector();
					float impulseMagnitude = (particles[0]->position - Graphics::windowToCoordinate(mouseCursor)).magnitude() * 5.0;
                    particles[0]->velocity += impulseDirection * impulseMagnitude;	//
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

	//------ Applying a gravitational force between the two particles
	float minDistance = particles[0]->radius + particles[1]->radius;
	Vec2 attraction = Force::generateGravitationalForce(*particles[0], *particles[1], 10, minDistance, 1000);
	particles[0]->addForce(attraction);
	particles[1]->addForce(-attraction);

	//----------Integrate the acceleration and velocity to estimate the new position -------------
	for (auto particle : particles) {
		particle->integrate(deltaTime);
	}

	//--------------------- stop the objects when they touch each other (post effects can be added later) -----------------------
	float distance = (particles[0]->position - particles[1]->position).magnitude();
	if (distance <= minDistance)
	{
		for (auto particle : particles)
		{
			particle->acceleration = { 0, 0 };
			particle->velocity = { 0, 0 };
			particle->sumForces = { 0, 0 };
		}
	}

	//------------------ bouncing from the boundaries with reduced speed --------------------------
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
    //Graphics::clearScreen(0xFF0F0721);
	Graphics::clearScreen(0xFF21070F);

	Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference

    if (leftMouseButtonDown) {
		Graphics::drawLine(particles[0]->position.x, particles[0]->position.y, 
			Graphics::xPosInCoordinate(mouseCursor.x), Graphics::yPosInCoordinate(mouseCursor.y), 0xFF0000FF);
    }

	Graphics::drawDisk(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFAA3300);
	Graphics::drawDisk(particles[1]->position.x, particles[1]->position.y, particles[1]->radius, 0xFF00FFFF);

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
