#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
    const int windowWidth{1152};
    const int windowHeight{1152};
    InitWindow(windowWidth, windowHeight, "343 Project");

    Texture2D map = LoadTexture("tileset/WorldMap.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character brock{windowWidth, windowHeight};

    // TODO: Add props to map
    // Prop props[]{};

    Enemy virus1{
        Vector2{800.f, 1300.f},
        LoadTexture("characters/virus_idle.png"),
        LoadTexture("characters/virus_run.png")};

    Enemy virus2{
        Vector2{500.f, 1700.f},
        LoadTexture("characters/virus_idle.png"),
        LoadTexture("characters/virus_run.png")};

    Enemy virus3{
        Vector2{200.f, 1900.f},
        LoadTexture("characters/virus_idle.png"),
        LoadTexture("characters/virus_run.png")};

    Enemy virus4{
        Vector2{1000.f, 1950.f},
        LoadTexture("characters/virus_idle.png"),
        LoadTexture("characters/virus_run.png")};

    Enemy *enemies[]{
        &virus1,
        &virus2,
        &virus3,
        &virus4};
    int enemyCount{4};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&brock);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(brock.getWorldPos(), -1.f);

        // draw the map
        DrawTextureEx(map, mapPos, 0.0, 4.0, WHITE);

        if (!brock.getAlive()) // Character is not alive
        {
            DrawText("Game Over!", 55.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else // Character is alive
        {
            std::string brocksHealth = "Health: ";
            brocksHealth.append(std::to_string(brock.getHealth()), 0, 5);
            DrawText(brocksHealth.c_str(), 55.f, 45.f, 40, RED);
            int enemiesLeft{4};
            for (auto enemy : enemies)
            {
                if (!enemy->getAlive())
                {
                    enemiesLeft--;
                }
            }
            if (enemiesLeft <= 0)
            {
                DrawText("You fixed all the bugs, ship the game!", 55.f, 200.f, 40, BLACK);
            }
            else
            {
                DrawText(TextFormat("There are %i viruses left", enemiesLeft), 55.f, 100.f, 40, BLACK);
            }
        }

        brock.tick(GetFrameTime());
        // check map bounds
        if (brock.getWorldPos().x < 0.f ||
            brock.getWorldPos().y < 0.f ||
            brock.getWorldPos().x + windowWidth > map.width * mapScale ||
            brock.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            brock.undoMovement();
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), brock.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    CloseWindow();
}
