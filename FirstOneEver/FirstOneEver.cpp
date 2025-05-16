#include <iostream>
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
    int score; 
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

void ResetGame(Vector2& playerPosition, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, std::vector<PowerUp>& powerUps, int& playerHealth, bool& gameOver, float& enemySpeed, bool& enemySpeedReduced, int& enemySpeedTimer, int& score) {
    playerPosition = { 450 / 2 - 25, 800 - 100 };
    bullets.clear();
    enemies.clear();
    powerUps.clear();
    playerHealth = 3;
    gameOver = false;
    score = 0;

    enemySpeed = 2.0f;
    enemySpeedReduced = false;
    enemySpeedTimer = 0;
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
    std::vector<PowerUp> powerUps;

    int frameCounter = 0;
    float bulletSpeed = 7.0f;
    float enemySpeed = 2.0f;

    bool fastShooting = false;
    int fastShootingTimer = 0;

    bool enemySpeedReduced = false;
    int enemySpeedTimer = 0;

    int playerHealth = 3;
    bool gameOver = false;

    int score = 0;

    int scoreMultiplier = 1;
    float gameTime = 0.0f;


    while (!WindowShouldClose()) {
        frameCounter++;
        float deltaTime = GetFrameTime();
        gameTime += deltaTime;
        if (static_cast<int>(gameTime) % 10 == 0 && static_cast<int>(gameTime) != 0 && frameCounter % 60 == 0) {
            scoreMultiplier++;
        }

    

        if (gameOver) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 100, 40, RED);

            DrawRectangle(screenWidth / 2 - 60, screenHeight / 2 - 30, 120, 40, DARKGRAY);
            DrawText("TRY AGAIN", screenWidth / 2 - MeasureText("TRY AGAIN", 20) / 2, screenHeight / 2 - 20, 20, WHITE);

            DrawRectangle(screenWidth / 2 - 60, screenHeight / 2 + 30, 120, 40, DARKGRAY);
            DrawText("QUIT", screenWidth / 2 - MeasureText("QUIT", 20) / 2, screenHeight / 2 + 40, 20, WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();

                if (mousePos.x > screenWidth / 2 - 60 && mousePos.x < screenWidth / 2 + 60 &&
                    mousePos.y > screenHeight / 2 - 30 && mousePos.y < screenHeight / 2 + 10) {
                    ResetGame(playerPosition, bullets, enemies, powerUps, playerHealth, gameOver, enemySpeed, enemySpeedReduced, enemySpeedTimer, score);
                }

                if (mousePos.x > screenWidth / 2 - 60 && mousePos.x < screenWidth / 2 + 60 &&
                    mousePos.y > screenHeight / 2 + 30 && mousePos.y < screenHeight / 2 + 70) {
                    CloseWindow();
                    return 0;
                }
            }

            EndDrawing();
            continue;
        }

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
            enemies.push_back({ {enemyX, -40}, enemySpeed, enemyLevel, enemyLevel * 10});
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
                        score += enemies[j].score * scoreMultiplier;


                        if (rand() % 100 < 20) {
                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, rand() % 3 });
                        }

                        if (rand() % 100 < 2) {
                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, 3 });
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
                if (powerUps[i].type == 0) {
                    fastShooting = true;
                }
                else if (powerUps[i].type == 1) {
                    playerSpeed += 2.0f;
                }
                else if (powerUps[i].type == 2) {
                    if (!enemySpeedReduced) {
                        enemySpeed -= 0.5f;
                        enemySpeedReduced = true;
                    }
                    enemySpeedTimer = 0;
                }
                else if (powerUps[i].type == 3 && playerHealth < 3) {
                    playerHealth++;
                }

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

        if (enemySpeedReduced) {
            enemySpeedTimer++;
            if (enemySpeedTimer > 600) {
                enemySpeed += 0.5f;
                enemySpeedReduced = false;
                enemySpeedTimer = 0;
            }
        }

        for (int i = enemies.size() - 1; i >= 0; i--) {
            if (CheckCollision(playerPosition, { 50, 50 }, enemies[i].position, { 40, 40 })) {
                playerHealth--;
                enemies.erase(enemies.begin() + i);

                if (playerHealth <= 0) {
                    gameOver = true;
                    break;
                }
            }
        }

        for (int i = enemies.size() - 1; i >= 0; i--) {
            if (enemies[i].position.y > screenHeight) {
                playerHealth--;
                enemies.erase(enemies.begin() + i);

                if (playerHealth <= 0) {
                    gameOver = true;
                    break;
                }
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
            Color powerColor =
                (powerUp.type == 0) ? PURPLE :
                (powerUp.type == 1) ? BLUE :
                (powerUp.type == 2) ? SKYBLUE :
                (powerUp.type == 3) ? GREEN : WHITE;
            DrawRectangleV(powerUp.position, { 30, 30 }, powerColor);
        }

        DrawText(TextFormat("HP: %d", playerHealth), 10, 10, 20, WHITE);
        DrawText(TextFormat("Score: %d", score), screenWidth - 100, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
