#include "mazegen.h"
#include "types.h"

// Function to shuffle directions to ensure randomness
void shuffle(int arr[][2], int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tempX = arr[i][0], tempY = arr[i][1];
        arr[i][0] = arr[j][0];
        arr[i][1] = arr[j][1];
        arr[j][0] = tempX;
        arr[j][1] = tempY;
    }
}

// Function to check if a cell is within maze bounds
int is_in_bounds(int x, int y, int rows, int cols)
{
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Recursive DFS function to carve paths in the maze
void carve_path(int **maze, int x, int y, int rows, int cols)
{
    // Directions: right, left, down, up
    int dir[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    shuffle(dir, 4); // Shuffle directions to randomize path

    for (int i = 0; i < 4; i++)
    {
        int dx = dir[i][0], dy = dir[i][1];
        int nx = x + 2 * dx, ny = y + 2 * dy; // Move two steps

        if (is_in_bounds(nx, ny, rows, cols) && maze[nx][ny] == 1)
        {
            maze[x + dx][y + dy] = OPEN; // Carve midpoint
            maze[nx][ny] = OPEN;         // Carve destination

            carve_path(maze, nx, ny, rows, cols);
        }
    }
}

// Function to create a 2D maze dynamically
int **generate_maze(int rows, int cols, int startX, int startY, int exitX, int exitY)
{
    // Seed random number generator
    // Allocate memory for the maze
    int **maze = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        maze[i] = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
        {
            maze[i][j] = WALL; // Initialize with walls
        }
    }

    // Set the start and exit points
    maze[startX][startY] = PLAYER; // Start
    maze[exitX][exitY] = GOAL;   // Exit

    // Start carving paths from the starting point
    maze[startX][startY] = OPEN; // Temporarily mark as open for DFS
    carve_path(maze, startX, startY, rows, cols);
    maze[startX][startY] = PLAYER; // Restore start
    maze[exitX][exitY] = GOAL;   // Restore exit

    return maze;
}