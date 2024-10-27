#include "Graphics.h"
#include <iostream>

SDL_Window* Graphics::window = NULL;
SDL_Renderer* Graphics::renderer = NULL;
int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;
int Graphics::xOrigin = 0;
int Graphics::yOrigin = 0;
int Graphics::unitLength = 20;

int Graphics::getWindowWidth() {
    return windowWidth;
}

int Graphics::getWindowHeight() {
    return windowHeight;
}

//---------------- open the full screen window ---------------------------
bool Graphics::openWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = display_mode.w;
    windowHeight = display_mode.h;
    window = SDL_CreateWindow(NULL, 0, 0, windowWidth, windowHeight, SDL_WINDOW_FULLSCREEN);

    if (!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return false;
    }
    return true;
}

//--------------------- open a maximized window with a title ------------------------------------
bool Graphics::openWindow(const char *windowTitle) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL" << std::endl;
		return false;
	}

	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1,1, SDL_WINDOW_MAXIMIZED);

	if (!window) {
		std::cerr << "Error creating SDL window" << std::endl;
		return false;
	}

	SDL_GetWindowSize(window, &windowWidth, &windowHeight);	// get the display window size
	Graphics::windowWidth = windowWidth;	// set the window width
	Graphics::windowHeight = windowHeight;	// set the window height
	//std::cout << "w = " << windowWidth << ", h = " << windowHeight << std::endl;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer" << std::endl;
		return false;
	}
	return true;
}

//--------------------- open a centered window with a title and fixed size -----------------------------
bool Graphics::openWindow(const char *windowTitle, int windowWidth, int windowHeight) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL" << std::endl;
		return false;
	}

	window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

	if (!window) {
		std::cerr << "Error creating SDL window" << std::endl;
		return false;
	}

	Graphics::windowWidth = windowWidth;	// set the window width
	Graphics::windowHeight = windowHeight;	// set the window height

	//std::cout << "w = " << windowWidth << ", h = " << windowHeight << std::endl;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer" << std::endl;
		return false;
	}
	return true;
}


void Graphics::clearScreen(Uint32 color) {
	//SDL_SetRenderDrawColor(renderer, color >> 16, color >> 8, color, 255);		
	SDL_SetRenderDrawColor(renderer, color, color >> 8, color >> 16, 255);
    SDL_RenderClear(renderer);
}

void Graphics::clearScreen(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
	SDL_RenderClear(renderer);
}

void Graphics::renderFrame() {
    SDL_RenderPresent(renderer);
}

int Graphics::xPosInWindow(float x)
{
	return xOrigin + (int)(x * unitLength);
}

int Graphics::yPosInWindow(float y)
{
	return yOrigin - (int)(y * unitLength);
}

float Graphics::xPosInCoordinate(int x)
{
	return 1.0* (x - xOrigin) / unitLength;
}

float Graphics::yPosInCoordinate(int y)
{
	return 1.0* (yOrigin - y) / unitLength;
}

//--------------------- set the origin at the lower left corner of the window ----------------------
void Graphics::setOrigin()
{
	xOrigin = 0;
	yOrigin = windowHeight;
}

//---------------------- set the origin at the specified position ---------------------
void Graphics::setOrigin(int x, int y)
{
	xOrigin = x;
	yOrigin = y;
}

Vec2 Graphics::windowToCoordinate(Vec2 pix)
{
	float x, y;

	x = (pix.x - xOrigin) / unitLength;
	y = (yOrigin - pix.y) / unitLength;

	Vec2 vector = Vec2(x, y);
	return vector;
}

Vec2 Graphics::coordinateToWindow(Vec2 vector)
{
	int x, y;

	x = xPosInWindow(vector.x);
	y = yPosInWindow(vector.y);

	Vec2 pix = Vec2(x, y);
	return pix;
}

void Graphics::coordinateGrid(int space, Uint32 color)
{
	thickLineColor(renderer, 0, yOrigin, windowWidth, yOrigin, 3, color);
	thickLineColor(renderer, xOrigin, 0, xOrigin, windowHeight, 3, color);

	int x = 1;
	while (x*space <= windowWidth - xOrigin)
	{
		lineColor(renderer, xOrigin + x * space, 0, xOrigin + x * space, windowHeight, color);
		x++;
	}

	x = 1;
	while (x*space <= xOrigin)
	{
		lineColor(renderer, xOrigin - x * space, 0, xOrigin - x * space, windowHeight, color);
		x++;
	}

	int y = 1;
	while (y*space <= windowHeight - yOrigin)
	{
		lineColor(renderer, 0, yOrigin + y * space, windowWidth, yOrigin + y * space, color);
		y++;
	}

	y = 1;
	while (y*space <= yOrigin)
	{
		lineColor(renderer, 0, yOrigin - y * space, windowWidth, yOrigin - y * space, color);
		y++;
	}
}

void Graphics::drawLine(float x1, float y1, float x2, float y2, Uint32 color)
{
	lineColor(renderer, xPosInWindow(x1), yPosInWindow(y1), xPosInWindow(x2), yPosInWindow(y2), color);
}

void Graphics::drawCircle(float x, float y, float radius, Uint32 color)
{
	circleColor(renderer, xPosInWindow(x), yPosInWindow(y), static_cast<short>(radius*unitLength), color);
}

//-------- rotation considered ----------
void Graphics::drawCircle(float x, float y, float radius, float angle, Uint32 color) {
	circleColor(renderer, xPosInWindow(x), yPosInWindow(y), 
		static_cast<short>(radius*unitLength), color);

	lineColor(renderer, xPosInWindow(x), yPosInWindow(y),
		xPosInWindow(x + cos(angle) * radius), 
		yPosInWindow(y + sin(angle) * radius), color);
}

void Graphics::drawDisk(float x, float y, float radius, Uint32 color)
{
	filledCircleColor(renderer, xPosInWindow(x), yPosInWindow(y), (short)(radius*unitLength), color);
}

//-------- rotation considered ----------
void Graphics::drawDisk(float x, float y, float radius, float angle, Uint32 color) {
	filledCircleColor(renderer, xPosInWindow(x), yPosInWindow(y), 
		(short)(radius*unitLength), color);

	lineColor(renderer, xPosInWindow(x), yPosInWindow(y),
		xPosInWindow(x + cos(angle) * radius), 
		yPosInWindow(y + sin(angle) * radius), 0xff000000);
}

void Graphics::drawRect(float x, float y, float width, float height, Uint32 color)
{
	float x1, y1, x2, y2, halfW, halfH;

	halfW = width / 2;
	halfH = height / 2;

	x1 = x - halfW;
	y1 = y - halfH;
	x2 = x + halfW;
	y2 = y + halfH;

	rectangleColor(renderer, xPosInWindow(x1), yPosInWindow(y1), xPosInWindow(x2), yPosInWindow(y2), color);
}

void Graphics::drawBox(float x, float y, float width, float height, Uint32 color)
{
	float x1, y1, x2, y2, halfW, halfH;

	halfW = width / 2;
	halfH = height / 2;

	x1 = x - halfW;
	y1 = y - halfH;
	x2 = x + halfW;
	y2 = y + halfH;

	boxColor(renderer, xPosInWindow(x1), yPosInWindow(y1), xPosInWindow(x2), yPosInWindow(y2), color);
}

void Graphics::drawPolygon(const std::vector<Vec2>& vertices, Uint32 color)
{
	std::vector<short> vX;
	std::vector<short> vY;

	for (int i = 0; i < vertices.size(); i++) {
		vX.push_back(xPosInWindow(vertices[i].x));
	}
	for (int i = 0; i < vertices.size(); i++) {
		vY.push_back(yPosInWindow(vertices[i].y));
	}

	polygonColor(renderer, &vX[0], &vY[0], vertices.size(), color);
}

void Graphics::drawFilledPolygon(const std::vector<Vec2>& vertices, Uint32 color)
{
	std::vector<short> vX;
	std::vector<short> vY;

	for (int i = 0; i < vertices.size(); i++) {
		vX.push_back(xPosInWindow(vertices[i].x));
	}
	for (int i = 0; i < vertices.size(); i++) {
		vY.push_back(yPosInWindow(vertices[i].y));
	}

	filledPolygonColor(renderer, &vX[0], &vY[0], vertices.size(), color);
}

void Graphics::drawTexture(float x, float y, float width, float height, float rotation, SDL_Texture* texture)
{
	SDL_Rect dstRect = { xPosInWindow(x - (width / 2)), yPosInWindow(y - (height / 2)), 
		static_cast<int>(width*unitLength), static_cast<int>(height*unitLength) };

	SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, rotation, NULL, SDL_FLIP_NONE);
}	// rotation is in degree, clockwise


//-------------------- source code from Gustavo Pezzi, co-founder of Pikuma.com ---------------------------
void Graphics::DrawLine(int x0, int y0, int x1, int y1, Uint32 color) {
    lineColor(renderer, x0, y0, x1, y1, color);
}

void Graphics::DrawCircle(int x, int y, int radius, float angle, Uint32 color) {
    circleColor(renderer, x, y, radius, color);
    lineColor(renderer, x, y, x + cos(angle) * radius, y + sin(angle) * radius, color);
}

void Graphics::DrawFillCircle(int x, int y, int radius, Uint32 color) {
    filledCircleColor(renderer, x, y, radius, color);
}

void Graphics::DrawRect(int x, int y, int width, int height, Uint32 color) {
    lineColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y - height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y + height / 2.0, x - width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x - width / 2.0, y + height / 2.0, x - width / 2.0, y - height / 2.0, color);
}

void Graphics::DrawFillRect(int x, int y, int width, int height, Uint32 color) {
    boxColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
}

void Graphics::DrawPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    for (int i = 0; i < vertices.size(); i++) {
        int currIndex = i;
        int nextIndex = (i + 1) % vertices.size();
        lineColor(renderer, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
    }
    filledCircleColor(renderer, x, y, 1, color);
}

void Graphics::DrawFillPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    std::vector<short> vx;
    std::vector<short> vy;
    for (int i = 0; i < vertices.size(); i++) {
        vx.push_back(static_cast<int>(vertices[i].x));
    }
    for (int i = 0; i < vertices.size(); i++) {
        vy.push_back(static_cast<int>(vertices[i].y));
    }
    filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), color);
    filledCircleColor(renderer, x, y, 1, 0xFF000000);
}

void Graphics::DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture* texture) {
    SDL_Rect dstRect = {x - (width / 2), y - (height / 2), width, height};
    float rotationDeg = rotation * 57.2958;
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, rotationDeg, NULL, SDL_FLIP_NONE);
}

void Graphics::CloseWindow(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}