#include "raylib.h"
#include <iostream>
#include <stdlib.h>

int main()
{
	
	InitWindow(450, 800, "prototip");
	SetTargetFPS(60);
	
	while (!WindowShouldClose())
	{
		
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText("HAZIR OL", 150, 400, 20,WHITE);
		EndDrawing();
	}

	// Close window and OpenGL context
	
	CloseWindow();
	return 0;	

}
