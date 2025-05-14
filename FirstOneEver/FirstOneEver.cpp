#include <iostream>

#include <string>

#include <cstdlib>

#include <ctime>

#include <vector>

#include <algorithm>

#include "raylib.h"

struct Bullet {

    Vector2 position;

    float speed;

};

struct Enemy {

    Vector2 position;

    float speed;

    int health;

};

struct PowerUp {

    Vector2 position;

    int type;

};

bool CheckCollision(Vector2 aPos, Vector2 aSize, Vector2 bPos, Vector2 bSize) {

    return !(aPos.x > bPos.x + bSize.x ||

        aPos.x + aSize.x < bPos.x ||

        aPos.y > bPos.y + bSize.y ||

        aPos.y + aSize.y < bPos.y);

}

int main() {

    const int screenWidth = 450;

    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "prototip");

    SetTargetFPS(60);

    srand(time(NULL));

    Vector2 playerPosition = { screenWidth / 2 - 25, screenHeight - 100 };

    float playerSpeed = 5.0f;

    std::vector<Bullet> bullets;

    std::vector<Enemy> enemies;

    std::vector<PowerUp> powerUps; // Güçlendirme listesi eklendi.

    int frameCounter = 0;

    float bulletSpeed = 7.0f;

    float enemySpeed = 2.0f;

    bool fastShooting = false;

    int fastShootingTimer = 0;

    while (!WindowShouldClose()) {

        frameCounter++;

        if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) playerPosition.x -= playerSpeed;

        if (IsKeyDown(KEY_RIGHT) && playerPosition.x < screenWidth - 50) playerPosition.x += playerSpeed;

        int fps = GetFPS();

        if (fps > 0 && frameCounter % ((fastShooting ? fps / 8 : fps / 4)) == 0) {

            bullets.push_back({ {playerPosition.x + 20, playerPosition.y}, bulletSpeed });

        }

        for (auto& bullet : bullets) {

            bullet.position.y -= bullet.speed;

        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),

            [](Bullet b) { return b.position.y < 0; }), bullets.end());

        if (frameCounter % 60 == 0) {

            float enemyX = rand() % (screenWidth - 40);

            int enemyLevel = rand() % 3 + 1;

            enemies.push_back({ {enemyX, -40}, enemySpeed, enemyLevel });

        }

        for (auto& enemy : enemies) {

            enemy.position.y += enemy.speed;

        }

        for (int i = bullets.size() - 1; i >= 0; i--) {

            for (int j = enemies.size() - 1; j >= 0; j--) {

                if (CheckCollision(bullets[i].position, { 10, 20 }, enemies[j].position, { 40, 40 })) {

                    bullets.erase(bullets.begin() + i);

                    enemies[j].health--;

                    if (enemies[j].health <= 0) {

                        if (rand() % 100 < 15) {

                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, rand() % 3 });

                        }

                        enemies.erase(enemies.begin() + j);

                    }

                    break;

                }

            }

        }

        for (auto& powerUp : powerUps) {

            powerUp.position.y += enemySpeed;

        }

        for (int i = powerUps.size() - 1; i >= 0; i--) {

            if (CheckCollision(playerPosition, { 50, 50 }, powerUps[i].position, { 30, 30 })) {

                if (powerUps[i].type == 0) fastShooting = true;

                else if (powerUps[i].type == 1) playerSpeed += 2.0f;

                else if (powerUps[i].type == 2) enemySpeed -= 0.5f;

                powerUps.erase(powerUps.begin() + i);

            }

        }

        if (fastShooting) {

            fastShootingTimer++;

            if (fastShootingTimer > 300) {

                fastShooting = false;

                fastShootingTimer = 0;

            }

        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),

            [screenHeight](Enemy e) { return e.position.y > screenHeight; }), enemies.end());

        powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(),

            [screenHeight](PowerUp p) { return p.position.y > screenHeight; }), powerUps.end());

        BeginDrawing();

        ClearBackground(BLACK);

        DrawRectangleV(playerPosition, { 50, 50 }, BLUE);

        for (const auto& bullet : bullets) {

            DrawRectangleV(bullet.position, { 10, 20 }, YELLOW);

        }

        for (const auto& enemy : enemies) {

            Color enemyColor = (enemy.health == 3) ? RED : (enemy.health == 2) ? ORANGE : GREEN;

            DrawRectangleV(enemy.position, { 40, 40 }, enemyColor);

        }

        for (const auto& powerUp : powerUps) {

            Color powerColor = (powerUp.type == 0) ? PURPLE : (powerUp.type == 1) ? BLUE : SKYBLUE;

            DrawRectangleV(powerUp.position, { 30, 30 }, powerColor);

        }

        EndDrawing();

    }

    CloseWindow();

    return 0;

}
