#include "DiskApp.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include <iostream>


//SDL_Renderer* renderer = NULL;
SDL_Surface* image = NULL;
SDL_Texture* texture = NULL;


bool Application::isRunning() {
    return running;
}

//====================================================================================//
// setup - open a window, initialize objects, and set up coordinate system origin     //
//====================================================================================//
void Application::setup() {
	running = Graphics::openWindow("Textures on rotating disks");//openWindow();

	//----------- set up the position of the coordinate system origin in the window -----------------
	Graphics::setOrigin(Graphics::windowWidth / 2, Graphics::windowHeight / 2);	

	image = IMG_Load("./assets/basketball.png");
	if (image) {
		texture = SDL_CreateTextureFromSurface(Graphics::renderer, image);
		std::cout << "basket ball texture created" << std::endl;
		SDL_FreeSurface(image);
	}
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
				if (event.key.keysym.sym == SDLK_KP_PLUS)
					tempTorque = 10;
				if (event.key.keysym.sym == SDLK_KP_MINUS)
					tempTorque = -10;
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
				if (event.key.keysym.sym == SDLK_KP_PLUS)
					tempTorque = 0;
				if (event.key.keysym.sym == SDLK_KP_MINUS)
					tempTorque = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_RIGHT) {
					static int x, y;
					static float radius, mass;
					SDL_GetMouseState(&x, &y);
					radius = 0.5 * (rand() % 4 + 1);
					mass = 3.14159265*radius*radius;
					Body* aBody = new Body(Disk(radius), Graphics::xPosInCoordinate(x), Graphics::yPosInCoordinate(y), mass);
					aBody->angularVelocity = 1;
					bodies.push_back(aBody);
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

	//-------------------------- Apply forces and torques to the disks ----------------------------
	for (auto body : bodies) {
		body->addForce(pushForce);		// added when an arrow key is pressed

		//// Apply the weight force
		//Vec2 weight = Vec2(0.0, body->mass * -9.8 * Graphics::unitLength);//PIXELS_PER_METER);
		//body->addForce(weight);

		////------------------ Apply a drag force -------------------
		//float dragConstant;
		//if (body->shape->getType() == DISK) {
		//	Disk* disk = (Disk*)body->shape;
		//	dragConstant = disk->radius * 0.5;
		//}
		//Vec2 drag = Force::generateDragForce(*body, dragConstant);
		//body->addForce(drag);

		//------------------ add torque ---------------------
		body->addTorque(tempTorque);	// added when keypad +/- key is pressed
		//float torque = 10;
		//body->addTorque(torque);		// added permanently
	}

	//------------- Integrate the acceleration and velocity to estimate the new position ---------------
	for (auto body : bodies) {
		body->integrateLinear(deltaTime);
		body->integrateAngular(deltaTime);
	}

	//------------------ bouncing from the boundaries with reduced speed --------------------------
	static float ratio = 0.75;
	for (auto body : bodies) {
		if (body->shape->getType() == DISK) {
			Disk* disk = (Disk*)body->shape;
			if (Graphics::xPosInWindow(body->position.x - disk->radius) <= 0) {
				body->position.x = Graphics::xPosInCoordinate(0) + disk->radius;
				body->velocity.x *= -ratio;
				body->velocity.y *= ratio;
			}
			else if (Graphics::xPosInWindow(body->position.x + disk->radius) >= Graphics::getWindowWidth()) {
				body->position.x = Graphics::xPosInCoordinate(Graphics::getWindowWidth()) - disk->radius;
				body->velocity.x *= -ratio;
				body->velocity.y *= ratio;
			}
			if (Graphics::yPosInWindow(body->position.y - disk->radius) >= Graphics::getWindowHeight()) {
				body->position.y = Graphics::yPosInCoordinate(Graphics::getWindowHeight()) + disk->radius;
				body->velocity.x *= ratio;
				body->velocity.y *= -ratio;
			}
			else if (Graphics::yPosInWindow(body->position.y + disk->radius) <= 0) {
				body->position.y = Graphics::yPosInCoordinate(0) - disk->radius;
				body->velocity.x *= ratio;
				body->velocity.y *= -ratio;
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

	for (auto body : bodies) {
		if (body->shape->getType() == DISK) {
			Disk* disk = (Disk*)body->shape;
			//Graphics::drawCircle(body->position.x, body->position.y, disk->radius, body->rotation, 0xFF00FFFF);

			SDL_Rect dstrect = { Graphics::xPosInWindow(body->position.x - disk->radius), 
				Graphics::yPosInWindow(body->position.y + disk->radius),
				Graphics::unitLength * disk->radius * 2, Graphics::unitLength * disk->radius *2 };
			SDL_RenderCopyEx(Graphics::renderer, texture, NULL, &dstrect, body->rotation*-57.295779513, NULL, SDL_FLIP_NONE);	// display in a particular rectangle
		}
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

	SDL_DestroyTexture(texture);
    Graphics::CloseWindow();
}
