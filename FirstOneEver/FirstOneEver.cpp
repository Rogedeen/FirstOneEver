#include "raylib.h"
#include <iostream>
#include <stdlib.h>

int main()
{
	
	InitWindow(800, 600, "Facebook");
	SetTargetFPS(60);
	
	while (!WindowShouldClose())
	{
		
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("by yunus ahmet", 350, 280, 20, LIGHTGRAY);
		EndDrawing();
	}

	// Close window and OpenGL context
	//denemeutkututku
	
	CloseWindow();
	
}
