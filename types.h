#if !defined(TYPES_H)
#define TYPES_H
//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------

struct Position
{
    int x;
    int y;
};

struct Moeka
{
    Position pos;
    int waiting;
    int resttime;
};

typedef enum CellType
{
    OPEN = 0,
    WALL = 1,
    PLAYER = 2,
    GOAL = 3
} CellType;

typedef enum AppScreen
{
    STARTSCREEN = 0,
    CAPTCHA,
    WIN,
    LOSS
} AppScreen;
#endif // TYPES_H