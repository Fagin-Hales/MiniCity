#define PLAY_IMPLEMENTATION
#include "../PlayBuffer/Play.h"

constexpr int DISPLAY_WIDTH = 1024;
constexpr int DISPLAY_HEIGHT = 768;
constexpr int DISPLAY_SCALE = 1;

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	Play::CentreAllSpriteOrigins();
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate(float elapsedTime)
{
	static float eTime = 0;
	eTime += elapsedTime;

	Play::ClearDrawingBuffer(Play::cBlue);

	Play::DrawCircle(Vector2f{ (sin(eTime / 0.732f + 2.3215f) + 1) * DISPLAY_WIDTH / 2, (cos(eTime / 1.21321f) + 1) * DISPLAY_HEIGHT / 2 }, (sin(eTime * 5) + 1) * 30, Play::cRed);

	Play::PresentDrawingBuffer();

	return Play::KeyDown(VK_ESCAPE);
}

// Gets called once when the player quits the game 
int MainGameExit(void)
{
	Play::DestroyManager();
	return PLAY_OK;
}

