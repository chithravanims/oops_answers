#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

// ----------------- Mole class -----------------
class Mole {
public:
    Vector2 position;
    bool visible = false;
    float timer = 0.0f;
    Texture2D texture;
    float scale = 0.35f;

    float holeScale = 0.0f;

    Mole(float x, float y, Texture2D tex) : texture(tex) {
        position = { x, y };
    }

    void update(float dt) {
        if (visible) {
            timer -= dt;
            if (timer <= 0.0f) visible = false;
        }

        if (visible) {
            holeScale += 5 * dt;
            if (holeScale > 1.0f) holeScale = 1.0f;
        } else {
            holeScale -= 5 * dt;
            if (holeScale < 0.0f) holeScale = 0.0f;
        }
    }

    void draw() {
        Vector2 shadowPos = { position.x + 10, position.y + 10 };
        DrawCircleV(shadowPos, 55, Fade(BLACK, 0.5f));

        DrawCircleV(position, 60 * holeScale, DARKGRAY);

        if (visible) {
            for (int i = 3; i > 0; i--) {
                DrawCircleLines(
                    position.x,
                    position.y,
                    texture.width * scale / 2.0f + i * 5,
                    Fade(YELLOW, 0.3f / i)
                );
            }
        }

        if (visible) {
            DrawTextureEx(
                texture,
                {
                    position.x - texture.width * scale / 2,
                    position.y - texture.height * scale / 2
                },
                0,
                scale,
                WHITE
            );
        }
    }

    void pop(float duration) {
        visible = true;
        timer = duration;
    }

    bool isHit(Vector2 mouse) {
        if (visible) {
            Rectangle rec = {
                position.x - texture.width * scale / 2,
                position.y - texture.height * scale / 2,
                texture.width * scale,
                texture.height * scale
            };

            if (CheckCollisionPointRec(mouse, rec)) {
                visible = false;
                return true;
            }
        }
        return false;
    }
};

// ----------------- Main -----------------
int main() {
    const int screenW = 800;
    const int screenH = 600;

    InitWindow(screenW, screenH, "Whack-a-Mole Enhanced");
    InitAudioDevice();
    SetTargetFPS(60);

    std::srand(static_cast<unsigned>(time(0)));

    // Load assets
    Texture2D moleTex = LoadTexture("assets/mole.png");
    Texture2D bgTex = LoadTexture("assets/background.png");
    Texture2D hammerTex = LoadTexture("assets/hammer.png");

    Music bgMusic = LoadMusicStream("assets/background.mp3");
    Sound hitSound = LoadSound("assets/hit.wav");

    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, 0.5f);

    // Create moles
    std::vector<Mole> moles;
    float startX = 200, startY = 150;
    float spacingX = 200, spacingY = 150;

    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            moles.push_back(Mole(startX + c * spacingX, startY + r * spacingY, moleTex));
        }
    }

    int score = 0;
    int lives = 3;
    float timeLeft = 60.0f;
    bool gameOver = false;
    bool youWon = false;

    float spawnTimer = 0.0f;
    float spawnInterval = 1.3f;
    float moleVisibleTime = 1.2f;

    HideCursor();

    // Quit button at top-right
    Rectangle quitBtn = { 680, 20, 100, 40 };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        UpdateMusicStream(bgMusic);

        // Timer logic
        if (!gameOver && !youWon) {
            timeLeft -= dt;
            if (timeLeft <= 0) {
                timeLeft = 0;
                if (lives > 0)
                    youWon = true;  // WIN condition
                else
                    gameOver = true;
            }
        }

        // Spawn mole
        if (!gameOver && !youWon) {
            spawnTimer -= dt;
            if (spawnTimer <= 0.0f) {
                spawnTimer = spawnInterval;

                std::vector<int> hiddenMoles;
                for (int i = 0; i < moles.size(); i++)
                    if (!moles[i].visible) hiddenMoles.push_back(i);

                if (!hiddenMoles.empty()) {
                    int idx = hiddenMoles[std::rand() % hiddenMoles.size()];
                    moles[idx].pop(moleVisibleTime);
                }
            }
        }

        // Click
        if (!gameOver && !youWon && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            bool hit = false;

            for (auto &m : moles) {
                if (m.isHit(mouse)) {
                    score += 10;
                    PlaySound(hitSound);
                    hit = true;
                    break;
                }
            }

            if (!hit) {
                lives--;
                if (lives <= 0) gameOver = true;
            }
        }

        // Quit button click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, quitBtn)) {
                CloseWindow();
            }
        }

        // Update moles
        if (!gameOver && !youWon) {
            for (auto &m : moles)
                m.update(dt);
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexture(bgTex, 0, 0, WHITE);

        for (auto &m : moles)
            m.draw();

        // Hammer
        Vector2 mouse = GetMousePosition();
        DrawTextureEx(
            hammerTex,
            { mouse.x - hammerTex.width * 0.75f,
              mouse.y - hammerTex.height * 0.75f },
            0,
            4.0f,
            WHITE
        );

        // Quit button
        Color qColor = CheckCollisionPointRec(mouse, quitBtn) ? LIGHTGRAY : GRAY;
        DrawRectangleRec(quitBtn, qColor);
        DrawText("QUIT", quitBtn.x + 20, quitBtn.y + 10, 20, BLACK);

        // UI
        DrawText(("Score: " + std::to_string(score)).c_str(), 10, 10, 22, DARKGRAY);
        DrawText(("Lives: " + std::to_string(lives)).c_str(), 10, 40, 22, RED);
        DrawText(("Time: " + std::to_string((int)timeLeft)).c_str(), 10, 70, 22, BLUE);

        // GAME OVER SCREEN
        if (gameOver) {
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.6f));
            DrawText("GAME OVER", screenW/2 - 140, screenH/2 - 40, 40, WHITE);
            DrawText(("Final Score: " + std::to_string(score)).c_str(),
                     screenW/2 - 110, screenH/2 + 10, 22, WHITE);
        }

        // YOU WON SCREEN
        if (youWon) {
            DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.6f));
            DrawText("YOU WON!", screenW/2 - 120, screenH/2 - 40, 40, GREEN);
            DrawText(("Final Score: " + std::to_string(score)).c_str(),
                     screenW/2 - 110, screenH/2 + 10, 22, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(moleTex);
    UnloadTexture(bgTex);
    UnloadTexture(hammerTex);
    UnloadSound(hitSound);
    UnloadMusicStream(bgMusic);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
