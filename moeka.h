#include "types.h"
#include <vector>

void moekaMove(int **maze, int rows, int cols, Position *moeka, std::vector<Position> *visited);
Position moekaStartpos(int **maze, int rows, int cols);
bool contains(std::vector<Position> *v, Position p);