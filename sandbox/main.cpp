#include "roguepack/gui/gui.hpp"

int main(int argc, char *argv[]) {
    InitWindow(900, 900, "roguepack");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawPanel(Rectangle{ 100, 100, 600, 300 }, LIGHTGRAY, 2, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}