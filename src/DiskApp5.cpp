#include "DiskApp.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>


bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("Collision between two disks");//openWindow();

	//------- set up the position of the coordinate system origin in the window -------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);	

	leftMouseButtonDown = false;

	Body* cueDisk = new Body(Disk(2), -10, 0, 10);
	cueDisk->color = 0xFFFFFFFF;
	bodies.push_back(cueDisk);

	Body* targetDisk = new Body(Disk(2), 10, 0, 10);
	targetDisk->color = 0xFF00FF00;
	bodies.push_back(targetDisk);
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
					pushForce.y = 200;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 200;
				if (event.key.keysym.sym == SDLK_DOWN)
					pushForce.y = -200;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -200;
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
					bodies[0]->velocity = (bodies[0]->position - Graphics::windowToCoordinate(mouseCursor)) * 5;	// simply set up the new velocity
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

	////-------------------------- Apply forces to the disks ----------------------------
	//for (auto body : bodies) {
	//	body->addForce(pushForce);		// added when an arrow key is pressed

	////------------------ Apply a friction force -------------------
	//	Vec2 friction = Force::generateFrictionForce(*body, 100);
	//	if (body->velocity.magnitude() > 0.01)
	//		body->addForce(friction);
	//}

	//------------- Integrate the acceleration and velocity to estimate the new position ---------------
	for (auto body : bodies) {
		body->integrateLinear(deltaTime);
		body->integrateAngular(deltaTime);
	}

	// Check all the rigidbodies with all other rigidbodies for collisions
	for (int i = 0; i < bodies.size() - 1; i++) {
		for (int j = i + 1; j < bodies.size(); j++) {
			Body* a = bodies[i];
			Body* b = bodies[j];

			Contact contact;

			if (CollisionDetection::isColliding(a, b, contact)) {
				// Resolve the collision using the impulse method
				 contact.resolveCollision();
			}
		}
	}

	//------------------ bouncing from the boundaries with reduced speed --------------------------
	static float ratio = 0.75;
	for (auto body : bodies) {
		if (body->shape->getType() == DISK) {
			Disk* disk = (Disk*)body->shape;
			if (Graphics::xPosInWindow(body->position.x - disk->radius) <= 0) {
				body->position.x = Graphics::xPosInCoordinate(0) + disk->radius;
				body->velocity.x *= -1.0 * ratio;
				body->velocity.y *= ratio;
			}
			else if (Graphics::xPosInWindow(body->position.x + disk->radius) >= Graphics::getWindowWidth()) {
				body->position.x = Graphics::xPosInCoordinate(Graphics::getWindowWidth()) - disk->radius;
				body->velocity.x *= -1.0 * ratio;
				body->velocity.y *= ratio;
			}
			if (Graphics::yPosInWindow(body->position.y - disk->radius) >= Graphics::getWindowHeight()) {
				body->position.y = Graphics::yPosInCoordinate(Graphics::getWindowHeight()) + disk->radius;
				body->velocity.x *= ratio;
				body->velocity.y *= -1.0 * ratio;
			}
			else if (Graphics::yPosInWindow(body->position.y + disk->radius) <= 0) {
				body->position.y = Graphics::yPosInCoordinate(0) - disk->radius;
				body->velocity.x *= ratio;
				body->velocity.y *= -1.0 * ratio;
			}
		}
	}
}

//====================================================================================//
// render - render the objects for a frame                                            //
//====================================================================================//
void Application::render() {
	Graphics::clearScreen(0xFF21070F);

	Graphics::coordinateGrid(Graphics::unitLength, 0xFF222222);	// draw the coordinate grid for reference

	int color;
	for (auto body: bodies) {
		if (body->shape->getType() == DISK) {
			color = body->color;
			Disk* disk = (Disk*)body->shape;
			Graphics::drawDisk(body->position.x, body->position.y,
				disk->radius, color);
		}
	}

	//--- draw a line connecting the mouse postion and the cue disk when the mouse if left pressed -----
	if (leftMouseButtonDown) {
		Graphics::drawLine(bodies[0]->position.x, bodies[0]->position.y,
			Graphics::xPosInCoordinate(mouseCursor.x), Graphics::yPosInCoordinate(mouseCursor.y), 0xFFFFFFFF);
	}

    Graphics::renderFrame();
}

//====================================================================================//
// desdroy - release the resources                                                    //
//====================================================================================//
void Application::destroy() {
    for (auto body: bodies) {
        delete body;
    }
    Graphics::CloseWindow();
}
