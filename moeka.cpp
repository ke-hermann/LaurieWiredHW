#include "moeka.h"
#include "cstdlib"
#include "types.h"

// check if we've already seen the given cell
bool contains(std::vector<Position> *v, Position p)
{
    if (v->size() == 0)
        return false;
    for (std::size_t i = 0; i < v->size(); i++)
    {
        if (v->at(i).x == p.x && v->at(i).y == p.y)
            return true;
    }
    return false;
}

// determine a starting position by looking for open cells
// sufficiently far enough from the start
Position moekaStartpos(int **maze, int rows, int cols)
{
    const int MIN_DISTANCE = 30;
    std::vector<Position> v;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (maze[i][j] == OPEN && (i + j) > MIN_DISTANCE)
                v.push_back(Position{i, j});
        }
    }
    int r = rand() % v.size();
    return v.at(r);
}

// move Moeka into a randomly chosen direction, avoid backtracking until cornered.
void moekaMove(int **maze, int rows, int cols, Position *moeka, std::vector<Position> *visited)
{
    int d[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    std::vector<Position> next;
    for (int i = 0; i < 4; i++)
    {
        int x = moeka->x + d[i][0];
        int y = moeka->y + d[i][1];
        if (x >= 0 && x < rows && y >= 0 && y < cols)
        {
            if (maze[x][y] == OPEN && !contains(visited, Position{x, y}))
                next.push_back(Position{x, y});
        }
    }
    // if there's no candidates we're in a corner.
    if (next.empty())
        visited->clear();
    else
    {
        Position newp = next.at(rand() % next.size());
        moeka->x = newp.x;
        moeka->y = newp.y;
    }
    visited->push_back(Position{moeka->x, moeka->y});
}