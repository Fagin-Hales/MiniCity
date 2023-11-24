#define PLAY_IMPLEMENTATION
#include "../PlayBuffer/Play.h"

constexpr int DISPLAY_WIDTH = 1024;
constexpr int DISPLAY_HEIGHT = 768;
constexpr int DISPLAY_SCALE = 1;

struct Object
{
	Vector2f pos;
	Vector2f vel;
	float size;
};

Object player{};
constexpr float playerSpeed{10};
constexpr float damping{ 0.9 };
constexpr float gravity{ 3 };
constexpr float jumpForce{ 50 };

// The entry point for a PlayBuffer program
void MainGameEntry(PLAY_IGNORE_COMMAND_LINE)
{
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
	Play::CentreAllSpriteOrigins();

	player.pos = Vector2f{ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 };
	player.vel = Vector2f{ 0,0 };
	player.size = 30;
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate(float elapsedTime)
{
	static float eTime = 0;
	eTime += elapsedTime;

	Play::ClearDrawingBuffer(Play::cCyan);

	player.vel.y -= gravity;
	
	if (Play::KeyDown('D'))
	{
		player.vel.x = playerSpeed;
	}
	else if (Play::KeyDown('A'))
	{
		player.vel.x = -playerSpeed;
	}
	if (Play::KeyPressed(VK_SPACE))
	{
		player.vel.y += jumpForce;
		player.vel.y = player.vel.y < jumpForce ? jumpForce : player.vel.y;
	}

	player.pos += player.vel;

	player.vel *= damping;

	if (player.pos.y - player.size < 0)
	{
		player.pos.y = player.size;
		player.vel.y = 0;
	}

	Play::DrawCircle(player.pos, player.size, Play::cRed);

	Play::PresentDrawingBuffer();

	return Play::KeyDown(VK_ESCAPE);
}

// Gets called once when the player quits the game 
int MainGameExit(void)
{
	Play::DestroyManager();
	return PLAY_OK;
}

