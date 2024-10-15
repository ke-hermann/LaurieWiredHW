#include "cstdlib"

typedef enum GameScreen
{
    LOGO = 0,
    TITLE,
    GAMEPLAY,
    ENDING
} GameScreen;

void shuffle(int arr[][2], int n);
int is_in_bounds(int x, int y, int rows, int cols);
void carve_path(int **maze, int x, int y, int rows, int cols);
int **generate_maze(int rows, int cols, int startX, int startY, int exitX, int exitY);