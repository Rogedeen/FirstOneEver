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
    int level;
    int score;
	Texture2D texture;
};

struct PowerUp {
    Vector2 position;
    int type;
};

struct Explosion {
    Vector2 position;
    int frame;
    int timer;
	Texture2D texture;
};


bool CheckCollision(Vector2 aPos, Vector2 aSize, Vector2 bPos, Vector2 bSize) {
    return !(aPos.x > bPos.x + bSize.x ||
        aPos.x + aSize.x < bPos.x ||
        aPos.y > bPos.y + bSize.y ||
        aPos.y + aSize.y < bPos.y);
}

void ResetGame(Vector2& playerPosition, std::vector<Bullet>& bullets, std::vector<Enemy>& enemies, std::vector<PowerUp>& powerUps, int& playerHealth, bool& gameOver, float& enemySpeed, bool& enemySpeedReduced, int& enemySpeedTimer, unsigned long int& score, float& playerSpeed, float& scoreMultiplier, float& gameTime, float& ultiCharge) {
    playerPosition = { 450 / 2 - 25, 800 - 100 };
    bullets.clear();
    playerSpeed = 5.0f;
    enemies.clear();
    powerUps.clear();
    playerHealth = 3;
    gameOver = false;
    score = 0;
    enemySpeed = 2.0f;
    enemySpeedReduced = false;
    enemySpeedTimer = 0;
	scoreMultiplier = 1.0f;
	gameTime = 0.0f;
	ultiCharge = 0.0f;
}

int main() {
    const int screenWidth = 450;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "prototip");
    InitAudioDevice();
    SetTargetFPS(60);
    srand(time(NULL));

	Texture2D enemyTexture1 = LoadTexture("assets/images/enemylevel1.png");
    Texture2D enemyTexture2 = LoadTexture("assets/images/enemylevel2.png");
    Texture2D enemyTexture3 = LoadTexture("assets/images/enemylevel3.png");
    Texture2D enemyTexture4 = LoadTexture("assets/images/enemylevel4.png");

    Texture2D engineTextures[4];  
    const int ENGINE_FRAME_COUNT = 8;
    const int ENGINE_FRAME_WIDTH = 64; 
    const int ENGINE_FRAME_HEIGHT = 64; 

    int engineFrame = 0;
    int engineTimer = 0;
    const int engineFrameSpeed = 6;

    engineTextures[0] = LoadTexture("assets/images/engine1.png");
    engineTextures[1] = LoadTexture("assets/images/engine2.png");
    engineTextures[2] = LoadTexture("assets/images/engine3.png");
    engineTextures[3] = LoadTexture("assets/images/engine4.png");

    Texture2D explosionTextures[4];
    std::vector<Explosion> explosions;

    explosionTextures[0] = LoadTexture("assets/images/enemydie1.png");
    explosionTextures[1] = LoadTexture("assets/images/enemydie2.png");
    explosionTextures[2] = LoadTexture("assets/images/enemydie3.png");
    explosionTextures[3] = LoadTexture("assets/images/enemydie4.png");


    const int EXPLOSION_FRAME_COUNT = 5;
    const int EXPLOSION_FRAME_WIDTH = 64;
    const int EXPLOSION_FRAME_HEIGHT = 64;
    const int EXPLOSION_FRAME_SPEED = 4; 

    Texture2D playerTextureFull = LoadTexture("assets/images/mainshipfullhealth.png");
    Texture2D playerTextureDamaged = LoadTexture("assets/images/mainshipdamaged.png");
    Texture2D playerTextureCritical = LoadTexture("assets/images/mainshipheavlydamaged.png");

    Texture2D powerUpTextures[5];
    powerUpTextures[0] = LoadTexture("assets/images/fastshooting.png");
    powerUpTextures[1] = LoadTexture("assets/images/movementboost.png");
    powerUpTextures[2] = LoadTexture("assets/images/time.png");
    powerUpTextures[3] = LoadTexture("assets/images/health.png");
    powerUpTextures[4] = LoadTexture("assets/images/clone.png");


    Vector2 playerPosition = { screenWidth / 2 - 25, screenHeight - 100 };
    Vector2 clonePosition;
    bool cloneActive = false;
    int cloneDuration = 0;
    const int cloneMaxDuration = 360;

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

    unsigned long int score = 0;

    float scoreMultiplier = 1.0f;
    float gameTime = 0.0f;

    float enemySpeedIncreaseTime = 0.0f;
    float enemySpawnIncreaseTime = 0.0f;

    float ultiCharge = 0.0f;
    const float ultiChargeRate = 0.025f;
    bool ultiActive = false;
    int ultiDurationTimer = 0;
    const int ultiDurationMax = 300;
    
	const float maxPlayerSpeed = 8.0f;
    const int maxPlayerHealth = 5;

    bool paused = false;

    Sound shootSound = LoadSound("assets/sounds/normal_mermi_sesi.wav");
    Sound gamerOverSound = LoadSound("assets/sounds/gameover.wav");
    Sound cloneSound = LoadSound("assets/sounds/klon.wav");
    Sound ultiCooldownSound = LoadSound("assets/sounds/ultidolumsesi.wav");
    Sound ultiStartSound = LoadSound("assets/sounds/ulti_baslama.wav");
    Sound ultiShootSound = LoadSound("assets/sounds/ultimermi.wav");
    Sound powerupSound = LoadSound("assets/sounds/powerupyakalama.wav");
    Sound healingSound = LoadSound("assets/sounds/canalma.wav");
    Sound explosionSounds[4];
    explosionSounds[0] = LoadSound("assets/sounds/explosion1.wav");
    explosionSounds[1] = LoadSound("assets/sounds/explosion2.wav");
    explosionSounds[2] = LoadSound("assets/sounds/explosion3.wav");
    explosionSounds[3] = LoadSound("assets/sounds/explosion4.wav");



    bool lastFrameGameOver = false; 
    bool ultiWarningPlayed = false;

   
    while (!WindowShouldClose()) {
        frameCounter++;
        float deltaTime = GetFrameTime();
        gameTime += deltaTime;
        enemySpeedIncreaseTime += deltaTime;
        enemySpawnIncreaseTime += deltaTime;



        engineTimer++;
        if (engineTimer >= engineFrameSpeed) {
            engineFrame = (engineFrame + 1) % ENGINE_FRAME_COUNT;
            engineTimer = 0;
        }

        if (enemySpeedIncreaseTime >= 10.0f) {
            enemySpeed += 0.1f;
            enemySpeedIncreaseTime = 0.0f;
        }

        if (!gameOver) {
            if (!ultiActive) {
                ultiCharge += ultiChargeRate * GetFrameTime();
                if (ultiCharge > 1.0f) ultiCharge = 1.0f;
            }
        }

        float ultiWarningThreshold = 1.0f - (9.0f * ultiChargeRate); 
        if (ultiCharge >= ultiWarningThreshold && !ultiWarningPlayed && !ultiActive) {
            PlaySound(ultiCooldownSound);
            ultiWarningPlayed = true;
        }

        if (ultiActive || ultiCharge < ultiWarningThreshold) {
            ultiWarningPlayed = false;
        }

        if (static_cast<int>(gameTime) % 2 == 0 && static_cast<int>(gameTime) != 0 && frameCounter % 60 == 0) {
            scoreMultiplier *= 1.1;
        }
        if (paused) {
            BeginDrawing();
            ClearBackground(DARKGRAY);
            DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 40) / 2, screenHeight / 2 - 100, 40, WHITE);

            DrawRectangle(screenWidth / 2 - 60, screenHeight / 2 - 30, 120, 40, DARKBLUE);
            DrawText("RESTART", screenWidth / 2 - MeasureText("RESTART", 20) / 2, screenHeight / 2 - 20, 20, WHITE);

            DrawRectangle(screenWidth / 2 - 60, screenHeight / 2 + 30, 120, 40, RED);
            DrawText("EXIT", screenWidth / 2 - MeasureText("EXIT", 20) / 2, screenHeight / 2 + 40, 20, WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePos = GetMousePosition();

                if (mousePos.x > screenWidth / 2 - 60 && mousePos.x < screenWidth / 2 + 60 &&
                    mousePos.y > screenHeight / 2 - 30 && mousePos.y < screenHeight / 2 + 10) {
                    ResetGame(playerPosition, bullets, enemies, powerUps, playerHealth, gameOver, enemySpeed, enemySpeedReduced, enemySpeedTimer, score, playerSpeed, scoreMultiplier, gameTime, ultiCharge);
                    paused = false;
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

        if (gameOver) {

            StopSound(shootSound);

            if (!lastFrameGameOver) {
                PlaySound(gamerOverSound);
            }

            lastFrameGameOver = true;

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
                    ResetGame(playerPosition, bullets, enemies, powerUps, playerHealth, gameOver, enemySpeed, enemySpeedReduced, enemySpeedTimer, score, playerSpeed, scoreMultiplier, gameTime, ultiCharge);
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
        else {
            lastFrameGameOver = false;
        }

        if (IsKeyDown(KEY_LEFT) && playerPosition.x > 0) playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_RIGHT) && playerPosition.x < screenWidth - 50) playerPosition.x += playerSpeed;

        if (cloneActive) {
            if (IsKeyDown(KEY_LEFT) && clonePosition.x < screenWidth - 50) clonePosition.x += playerSpeed;
            if (IsKeyDown(KEY_RIGHT) && clonePosition.x > 0) clonePosition.x -= playerSpeed;
        }
        if (cloneActive) {
            cloneDuration++;
            if (cloneDuration >= cloneMaxDuration) {
                cloneActive = false;
                cloneDuration = 0;
            }
        }
        if (IsKeyPressed(KEY_SPACE) && ultiCharge >= 1.0f && !ultiActive) {
            PlaySound(ultiStartSound);
            ultiActive = true;
            ultiCharge = 0.0f;
            fastShooting = true;
            ultiDurationTimer = 0;
        }
        if (IsKeyPressed(KEY_P)) {
            paused = !paused;
        }

        if (ultiActive) {
            ultiDurationTimer++;
            if (ultiDurationTimer >= ultiDurationMax) {
                ultiActive = false;
                fastShooting = false;
                ultiDurationTimer = 0;
            }
        }



        int fps = GetFPS();
        if (fps > 0 && frameCounter % ((fastShooting ? fps / 8 : fps / 4)) == 0) {
            bullets.push_back({ {playerPosition.x + 20, playerPosition.y}, bulletSpeed });
            if (cloneActive) {
                bullets.push_back({ {clonePosition.x + 20, clonePosition.y}, bulletSpeed });
            }

            if (ultiActive) {
                PlaySound(ultiShootSound);
            }
            else {
                PlaySound(shootSound);
            }
        }



        for (auto& bullet : bullets) {
            bullet.position.y -= bullet.speed;
        }

        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet b) { return b.position.y < 0; }), bullets.end());

        if (frameCounter % 60 == 0) {
            float enemyX = rand() % (screenWidth - 40);
            int enemyLevel;

            if (gameTime >= 60.0f) {

                enemyLevel = rand() % 4 + 1;
            }
            else {

                enemyLevel = rand() % 3 + 1;
            }

            float enemySpawnSpeed = enemySpeed;

            if (enemyLevel == 4) {
                enemySpawnSpeed = enemySpeed * 0.7f;
            }

            int enemyHealth = enemyLevel;
            if (enemyLevel == 4) enemyHealth = 6;

            Texture2D selectedTexture;
            if (enemyLevel == 4) {
                selectedTexture = enemyTexture4;
            }
            else if (enemyLevel == 3) {
                selectedTexture = enemyTexture3;
            }
            else if (enemyLevel == 2) {
                selectedTexture = enemyTexture2;
            }
            else {
                selectedTexture = enemyTexture1;
            }
            enemies.push_back({ {enemyX, 0}, enemySpawnSpeed, enemyHealth, enemyLevel, enemyLevel * 10, selectedTexture });
        }


        for (auto& enemy : enemies) {
            enemy.position.y += enemy.speed;
        }

        
        for (int i = explosions.size() - 1; i >= 0; i--) {
            explosions[i].timer++;
            if (explosions[i].timer >= EXPLOSION_FRAME_SPEED) {
                explosions[i].frame++;
                explosions[i].timer = 0;
            }

            if (explosions[i].frame >= EXPLOSION_FRAME_COUNT) {
                explosions.erase(explosions.begin() + i);
            }
        }

        for (int i = bullets.size() - 1; i >= 0; i--) {
            for (int j = enemies.size() - 1; j >= 0; j--) {
                if (CheckCollision(bullets[i].position, { 10, 20 }, enemies[j].position, { 80, 80 })) {
                    bullets.erase(bullets.begin() + i);

                    if (ultiActive) {
                        enemies[j].health -= 100;
                    }
                    else {
                        enemies[j].health--;
                    }

                    if (enemies[j].health <= 0) {

                        PlaySound(explosionSounds[rand() % 3]);
                        score += enemies[j].score * scoreMultiplier;
                        int levelIndex = enemies[j].level - 1;
                        explosions.push_back({ enemies[j].position, 0, 0, explosionTextures[levelIndex] });



                        int rando = rand() % 100;

                        if (rando > 0 && rando < 6) {
                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, 0 });
                        }
                        else if (rando >= 6 && rando < 11) {
                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, 1 });
                        }
                        else if (rando >= 11 && rando < 16) {
                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, 2 });
                        }
                        else if (rando >= 16 && rando < 19) {
                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, 3 });
                        }
                        else if (rando >= 19 && rando < 22) {
                            powerUps.push_back({ {enemies[j].position.x, enemies[j].position.y}, 4 });
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
                    PlaySound(powerupSound);
                    fastShooting = true;
                    fastShootingTimer = 0;
                }
                else if (powerUps[i].type == 1) {
                    PlaySound(powerupSound);
                    playerSpeed += 2.0f;
                    if (playerSpeed > maxPlayerSpeed) {
                        playerSpeed = maxPlayerSpeed;
                    }
                }
                else if (powerUps[i].type == 2) {
                    PlaySound(powerupSound);
                    if (!enemySpeedReduced) {
                        enemySpeed -= 0.5f;
                        enemySpeedReduced = true;
                    }
                    enemySpeedTimer = 0;
                }
                else if (powerUps[i].type == 3 && playerHealth < maxPlayerHealth) {
                    PlaySound(healingSound);
                    playerHealth++;
                }
                if (powerUps[i].type == 4) {
                    PlaySound(cloneSound);
                    cloneActive = true;
                    clonePosition = playerPosition;
                    cloneDuration = 0;
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

        Texture2D currentPlayerTexture;

        if (playerHealth >= 3) {
            currentPlayerTexture = playerTextureFull;
        }
        else if (playerHealth == 2) {
            currentPlayerTexture = playerTextureDamaged;
        }
        else {
            currentPlayerTexture = playerTextureCritical;
        }
        Vector2 adjustedPlayerPos = {
        playerPosition.x - 35,  
        playerPosition.y        
        };
        DrawTextureEx(currentPlayerTexture, adjustedPlayerPos, 0.0f, 2.5f, WHITE);

        if (cloneActive) {
            DrawTextureEx(currentPlayerTexture, clonePosition, 0.0f, 2.5f, SKYBLUE);
        }

        for (const auto& bullet : bullets) {
            DrawRectangleV(bullet.position, { 10, 20 }, YELLOW);
        }

        for (const auto& enemy : enemies) {
            DrawTextureEx(enemy.texture, enemy.position, 0.0f, 2.0f, WHITE);

            int level = enemy.level;
            if (level >= 1 && level <= 4) {
                Texture2D currentEngineTexture = engineTextures[level - 1];

                Rectangle sourceRec = {
                    (float)(engineFrame * ENGINE_FRAME_WIDTH),
                    0,
                    (float)ENGINE_FRAME_WIDTH,
                    (float)ENGINE_FRAME_HEIGHT
                };

                Vector2 enginePos;
                float scale = 4.0f;

                switch (enemy.level) {
                case 1:
                    enginePos = {
                        enemy.position.x,
                        enemy.position.y
                    };
                    scale = 2.0f;
                    break;
                case 2:
                    enginePos = {
                        enemy.position.x, 
                        enemy.position.y 
                    };
                    scale = 2.0f;
                    break;
                case 3:
                    enginePos = {
                        enemy.position.x,
                        enemy.position.y - 15 
                    };
                    scale = 2.0f;
                    break;
                case 4:
                    enginePos = {
                        enemy.position.x,
                        enemy.position.y 
                    };
                    scale = 2.0f;
                    break;
                default:
                    enginePos = enemy.position;
                    break;
                }
                Rectangle destRec = {
                    enginePos.x,
                    enginePos.y,
                    ENGINE_FRAME_WIDTH * scale,
                    ENGINE_FRAME_HEIGHT * scale
                };

                DrawTexturePro(currentEngineTexture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
            }
        }

        for (const auto& explosion : explosions) {
            Rectangle sourceRec = {
                (float)(explosion.frame * EXPLOSION_FRAME_WIDTH),
                0,
                (float)EXPLOSION_FRAME_WIDTH,
                (float)EXPLOSION_FRAME_HEIGHT
            };

            Rectangle destRec = {
                explosion.position.x,
                explosion.position.y,
                EXPLOSION_FRAME_WIDTH*2,
                EXPLOSION_FRAME_HEIGHT*2
            };

            DrawTexturePro(explosion.texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
        }

        DrawRectangle(10, 40, 150, 20, DARKGRAY);
        DrawRectangle(10, 40, static_cast<int>(150 * ultiCharge), 20, GOLD);
        DrawText("ULTI", 170, 40, 20, WHITE);


        for (const auto& powerUp : powerUps) {
            Color powerColor;
                if (powerUp.type == 0) powerColor = PURPLE;
                else if (powerUp.type == 1) powerColor = BLUE;
                else if (powerUp.type == 2) powerColor = BROWN;
                else if (powerUp.type == 3) powerColor = GREEN;
                else if (powerUp.type == 4) powerColor = WHITE;
                if (powerUp.type >= 0 && powerUp.type <= 4) {
                    DrawTextureEx(powerUpTextures[powerUp.type], powerUp.position, 0.0f, 2.0f, WHITE);
                }
        }

        DrawText(TextFormat("HP: %d", playerHealth), 10, 10, 20, WHITE);
        DrawText(TextFormat("Score: %d", score), screenWidth - 175, 10, 20, WHITE);

        EndDrawing();
    }

    UnloadTexture(enemyTexture1);
    UnloadTexture(enemyTexture2);
    UnloadTexture(enemyTexture3);
    UnloadTexture(enemyTexture4);
    for (int i = 0; i < 4; i++) {
        UnloadTexture(engineTextures[i]);
    }
    for (int i = 0; i < 4; i++) {
        UnloadTexture(explosionTextures[i]);
    }
    UnloadSound(shootSound);
    UnloadSound(ultiStartSound);
    CloseAudioDevice();

    UnloadTexture(playerTextureFull);
    UnloadTexture(playerTextureDamaged);
    UnloadTexture(playerTextureCritical);
    for (int i = 0; i < 5; i++) {
        UnloadTexture(powerUpTextures[i]);
    }


    CloseWindow();
    return 0;
}
