#include "raylib.h"

int main() {
    // 1️⃣ Initialize window
    InitWindow(800, 600, "Test");

    // 2️⃣ Load texture inside main
    Texture2D bg = LoadTexture("../assets/background.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 3️⃣ Draw texture inside main loop
        DrawTexture(bg, 0, 0, WHITE);

        EndDrawing();
    }

    // 4️⃣ Unload texture
    UnloadTexture(bg);

    CloseWindow();
    return 0;
}

