
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>
#include "../lib//SDL2_gfx/SDL2_gfxPrimitives.h"
#include "Physics/Vec2.h"
#include <vector>

struct Graphics {
    static int windowWidth;
    static int windowHeight;
	static int xOrigin;
	static int yOrigin;
	static int unitLength;
    static SDL_Window* window;
    static SDL_Renderer* renderer;

    static int getWindowWidth();
    static int getWindowHeight();
    static bool openWindow();
	static bool openWindow(const char * title);
	static bool openWindow(const char * title, int windowWidth, int windowHeight);
    static void CloseWindow();
    static void clearScreen(Uint32 color);
	static void clearScreen(Uint8 red, Uint8 green, Uint8 blue);
    static void renderFrame();
	static int xPosInWindow(float x);
	static int yPosInWindow(float y);
	static float xPosInCoordinate(int x);
	static float yPosInCoordinate(int y);
	static Vec2 windowToCoordinate(Vec2 pix);
	static Vec2 coordinateToWindow(Vec2 vector);
	static void setOrigin();
	static void setOrigin(int x, int y);
	static void coordinateGrid(int space, Uint32 color);

	static void drawLine(float x1, float y1, float x2, float y2, Uint32 color);
	static void drawCircle(float x, float y, float radius, Uint32 color);
	static void drawCircle(float x, float y, float radius, float angle, Uint32 color);	// rotation considered
	static void drawDisk(float x, float y, float radius, Uint32 color);
	static void drawDisk(float x, float y, float radius, float angle, Uint32 color);	// rotation considered
	static void drawRect(float x, float y, float width, float height, Uint32 color);	
	static void drawBox(float x, float y, float width, float height, Uint32 color);
	static void drawPolygon(const std::vector<Vec2>& vertices, Uint32 color);
	static void drawFilledPolygon(const std::vector<Vec2>& vertices, Uint32 color);
	static void drawTexture(float x, float y, float width, float height, float rotation, SDL_Texture* texture);

    static void DrawLine(int x0, int y0, int x1, int y1, Uint32 color);
    static void DrawCircle(int x, int y, int radius, float angle, Uint32 color);
    static void DrawFillCircle(int x, int y, int radius, Uint32 color);
    static void DrawRect(int x, int y, int width, int height, Uint32 color);
    static void DrawFillRect(int x, int y, int width, int height, Uint32 color);
    static void DrawPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color);
    static void DrawFillPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color);
    static void DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture* texture);
};

#endif