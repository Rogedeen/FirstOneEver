#include "raylib.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

int main() {
    const int screenWidth = 450;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Dikey Shooter Oyunu");
    SetTargetFPS(60);

    
    Vector2 playerPosition = { screenWidth / 2 - 25, screenHeight - 100 };
    const float playerSpeed = 5.0f; 

    while (!WindowShouldClose()) {
        
        if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_RIGHT) && playerPosition.x < screenWidth - 50) playerPosition.x += playerSpeed;

        BeginDrawing();
        ClearBackground(BLACK);

        
        DrawRectangleV(playerPosition, { 50, 50 }, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
