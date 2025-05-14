#include <iostream>
#include <string>
#include <cstdlib>
#include "raylib.h"
#include <vector>

struct Bullet {
    Vector2 position;
    float speed;
};

int main() {
    const int screenWidth = 450;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "prototip");
    SetTargetFPS(60);

    Vector2 playerPosition = { screenWidth / 2 - 25, screenHeight - 100 };
    const float playerSpeed = 5.0f;

    std::vector<Bullet> bullets;

    int frameCounter = 0;

    while (!WindowShouldClose()) {
        frameCounter++;

        if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_RIGHT) && playerPosition.x < screenWidth - 50) playerPosition.x += playerSpeed;

        int fps = GetFPS();
        if (fps > 0 && frameCounter % (fps / 4) == 0) {
            bullets.push_back({ {playerPosition.x + 20, playerPosition.y}, 7.0f });
        }

        for (auto& bullet : bullets) {
            bullet.position.y -= bullet.speed;
        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet b) { return b.position.y < 0; }), bullets.end());

        BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleV(playerPosition, { 50, 50 }, BLUE);

        for (const auto& bullet : bullets) {
            DrawRectangleV(bullet.position, { 10, 20 }, YELLOW);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}