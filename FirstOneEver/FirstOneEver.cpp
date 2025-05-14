#include "raylib.h"
#include <iostream>
#include <stdlib.h>

int main()
{
	const int screenWidth = 450;
	const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight, "prototip");
	SetTargetFPS(60);
	
	float backgroundY = 0;
	const float scrollSpeed = 2.0f;


	while (!WindowShouldClose())
	{
		backgroundY += scrollSpeed;
		if (backgroundY >= screenHeight) backgroundY = 0;
		
		BeginDrawing();
		ClearBackground(BLACK);

		DrawRectangle(0, backgroundY, screenWidth, screenHeight, DARKGRAY);
		DrawRectangle(0, backgroundY - screenHeight, screenWidth, screenHeight, DARKGRAY);


		EndDrawing();
		//push 
		//aananananananananananananan
	}

	CloseWindow();
	return 0;	

}
