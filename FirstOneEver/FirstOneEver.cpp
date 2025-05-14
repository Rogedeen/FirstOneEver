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
    int health; // D��man seviyesini belirleyen can de�eri eklendi.
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
    const float playerSpeed = 5.0f;

    std::vector<Bullet> bullets;
    std::vector<Enemy> enemies;

    int frameCounter = 0;
    const float bulletSpeed = 7.0f;
    const float enemySpeed = 2.0f;

    while (!WindowShouldClose()) {
        frameCounter++;

        // Oyuncu hareketi
        if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_RIGHT) && playerPosition.x < screenWidth - 50) playerPosition.x += playerSpeed;

        // Mermi olu�turma
        int fps = GetFPS();
        if (fps > 0 && frameCounter % (fps / 4) == 0) {
            bullets.push_back({ {playerPosition.x + 20, playerPosition.y}, bulletSpeed });
        }

        // Mermileri hareket ettir
        for (auto& bullet : bullets) {
            bullet.position.y -= bullet.speed;
        }

        // Ekran d��� mermileri temizle
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet b) { return b.position.y < 0; }), bullets.end());

        // D��man olu�turma (her 60 karede bir)
        if (frameCounter % 60 == 0) {
            float enemyX = rand() % (screenWidth - 40);
            int enemyLevel = rand() % 3 + 1;  // D��mana rastgele 1, 2 veya 3 seviyesinde can verildi.
            enemies.push_back({ {enemyX, -40}, enemySpeed, enemyLevel });
        }

        // D��manlar� hareket ettir
        for (auto& enemy : enemies) {
            enemy.position.y += enemy.speed;
        }

        // �arp��ma kontrol� (mermi & d��man)
        for (int i = bullets.size() - 1; i >= 0; i--) {
            for (int j = enemies.size() - 1; j >= 0; j--) {
                if (CheckCollision(bullets[i].position, { 10, 20 }, enemies[j].position, { 40, 40 })) {
                    bullets.erase(bullets.begin() + i);
                    enemies[j].health--; // D��man�n can�n� bir azalt.

                    // D��man�n can� s�f�ra indi�inde yok et.
                    if (enemies[j].health <= 0) {
                        enemies.erase(enemies.begin() + j);
                    }

                    break;
                }
            }
        }

        // Ekran d��� d��manlar� temizle
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [screenHeight](Enemy e) { return e.position.y > screenHeight; }), enemies.end());

        // �izim
        BeginDrawing();
        ClearBackground(BLACK);

        // Oyuncu
        DrawRectangleV(playerPosition, { 50, 50 }, BLUE);

        // Mermiler
        for (const auto& bullet : bullets) {
            DrawRectangleV(bullet.position, { 10, 20 }, YELLOW);
        }

        // D��manlar
        for (const auto& enemy : enemies) {
            Color enemyColor = (enemy.health == 3) ? RED : (enemy.health == 2) ? ORANGE : GREEN;
            // D��man seviyesine g�re renk belirlendi: 3 = K�rm�z�, 2 = Turuncu, 1 = Ye�il
            DrawRectangleV(enemy.position, { 40, 40 }, enemyColor);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}