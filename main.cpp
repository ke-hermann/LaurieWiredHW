#include "ctime"
#include "mazegen.h"
#include "moeka.h"
#include "types.h"
#include <raylib.h>
#include <string>

//------------------------------------------------------------------------------------------
// Helpers
//------------------------------------------------------------------------------------------

Texture2D loadTextureFromFile(std::string fp, int size, bool resize)
{
    Image img = LoadImage(fp.c_str());
    if (resize)
        ImageResize(&img, size, size);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}

void DrawWalls(int **maze, int rows, int cols, int c)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (maze[i][j] == WALL)
                DrawRectangle(i * c, j * c, c, c, BLACK);
        }
    }
}
//------------------------------------------------------------------------------------------
// Application Entrypoint
//------------------------------------------------------------------------------------------

int main()
{
    srand(static_cast<unsigned>(time(0)));
    // Initialization
    const int CSIZE = 40;
    const int ROWS = 25;
    const int COLS = 20;
    const int SCREENWIDTH = 1000;
    const int SCREENHEIGHT = 800;
    int **maze = generate_maze(ROWS, COLS, 1, 1, ROWS - 1, COLS - 1);

    AppScreen currentScreen = STARTSCREEN;
    Position player = Position{1, 1};
    Moeka moeka = Moeka{moekaStartpos(maze, ROWS, COLS), 0, 2};
    std::vector<Position> moekaVisited;

    Color textColor(LIGHTGRAY);
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Laurie's 2024 Halloween Challenge");

    InitAudioDevice();
    // IMPORTANT: only load textures and fonts after init window!
    Texture2D mayuriTex = loadTextureFromFile("resources/mayuri.png", CSIZE, true);
    Texture2D moekaTex = loadTextureFromFile("resources/moeka.png", CSIZE, true);
    Texture2D okabeTex = loadTextureFromFile("resources/okabe.png", CSIZE, true);
    Texture2D successTex = loadTextureFromFile("resources/success.png", 300, false);
    Texture2D failureTex = loadTextureFromFile("resources/failure.png", 300, false);
    Font departureMono = LoadFontEx("resources/DepartureMono-Regular.otf", 32, 0, 250);
    Sound winSound = LoadSound("resources/tuturu_1.mp3");
    Sound lossSound = LoadSound("resources/stop.mp3");

    SetTextLineSpacing(20);
    SetTargetFPS(30);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        switch (currentScreen)
        {
        case STARTSCREEN: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(GetMousePosition(), Rectangle{250, 200, 500, 200}))
                currentScreen = CAPTCHA;
        }
        break;
        case CAPTCHA: {
            // check winning condition
            if (maze[player.x][player.y] == GOAL)
            {
                currentScreen = WIN;
                PlaySound(winSound);
            };
            // process keyboard inputs
            if (IsKeyDown(KEY_RIGHT) && player.x < ROWS - 1 && maze[player.x + 1][player.y] != 1)
                player.x += 1;
            if (IsKeyDown(KEY_LEFT) && player.x > 0 && maze[player.x - 1][player.y] != 1)
                player.x -= 1;
            if (IsKeyDown(KEY_UP) && player.y > 0 && maze[player.x][player.y - 1] != 1)
                player.y -= 1;
            if (IsKeyDown(KEY_DOWN) && player.y < COLS - 1 && maze[player.x][player.y + 1] != 1)
                player.y += 1;

            // move moeka
            if (moeka.waiting > moeka.resttime)
            {
                moekaMove(maze, ROWS, COLS, &moeka.pos, &moekaVisited);
                moeka.waiting = 0;
            }
            else
                moeka.waiting++;
            // check for collisions
            Rectangle rec1 = Rectangle{float(player.x) * CSIZE, float(player.y) * CSIZE, CSIZE, CSIZE};
            Rectangle rec2 = Rectangle{float(moeka.pos.x) * CSIZE, float(moeka.pos.y) * CSIZE, CSIZE, CSIZE};

            if (CheckCollisionRecs(rec1, rec2))
            {
                currentScreen = LOSS;
                PlaySound(lossSound);
            }
        }
        break;
        default:
            break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        switch (currentScreen)
        {
        case STARTSCREEN: {
            DrawTextEx(departureMono, "To prove you're human, get Mayuri to the exit!", Vector2{50.0f, 125.0f}, 24.0f,
                       8, DARKGRAY);
            DrawTextEx(departureMono, "(and avoid Moeka)", Vector2{300.0f, 150.0f}, 24.0f, 8, DARKGRAY);
            DrawRectangle(250, 200, 500, 200, DARKGRAY);
            DrawTextEx(departureMono, "START", Vector2{425.0f, 275.0f}, 48.0f, 8, RAYWHITE);
        }
        break;
        case CAPTCHA: {
            DrawWalls(maze, ROWS, COLS, CSIZE);
            // draw the character sprites
            DrawTexture(mayuriTex, player.x * CSIZE, player.y * CSIZE, WHITE);
            DrawTexture(moekaTex, moeka.pos.x * CSIZE, moeka.pos.y * CSIZE, WHITE);
            DrawTexture(okabeTex, (ROWS - 1) * CSIZE, (COLS - 1) * CSIZE, RAYWHITE);
        }
        break;
        case WIN: {
            DrawTexture(successTex, 200, 200, RAYWHITE);
        }
        break;
        case LOSS: {
            DrawTexture(failureTex, 200, 200, RAYWHITE);
        }
        break;
        default:
            break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFont(departureMono);
    UnloadSound(winSound);
    UnloadSound(lossSound);
    UnloadTexture(mayuriTex);
    UnloadTexture(okabeTex);
    UnloadTexture(moekaTex);
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}