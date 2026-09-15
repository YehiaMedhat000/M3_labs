#include "../../LIB/STD_TYPES.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../../HAL/IR/IR_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "SNAKE_int.h"

#define SNAKE_BOARD_X       16
#define SNAKE_BOARD_Y       30
#define SNAKE_BOARD_COLS    12
#define SNAKE_BOARD_ROWS    10
#define SNAKE_CELL_SIZE     8
#define SNAKE_MAX_LENGTH    (SNAKE_BOARD_COLS * SNAKE_BOARD_ROWS)
#define SNAKE_MOVE_PERIOD_US 250000UL

#define SNAKE_DIR_UP        0
#define SNAKE_DIR_RIGHT     1
#define SNAKE_DIR_DOWN      2
#define SNAKE_DIR_LEFT      3

static u8 G_u8SnakeX[SNAKE_MAX_LENGTH];
static u8 G_u8SnakeY[SNAKE_MAX_LENGTH];
static u8 G_u8SnakeLength;
static u8 G_u8FoodX;
static u8 G_u8FoodY;
static volatile u8 G_u8Direction;
static volatile u8 G_u8RequestedDirection;
static volatile u8 G_u8ExitFlag;
static u32 G_u32LastMoveTime;

static void SNAKE_vDrawCell(u8 A_u8X, u8 A_u8Y, u16 A_u16Color);
static void SNAKE_vDrawBoard(void);
static void SNAKE_vDrawRound(void);
static void SNAKE_vPlaceFood(void);
static void SNAKE_vResetRound(void);
static void SNAKE_vMove(void);
static u8 SNAKE_u8IsSnakeCell(u8 A_u8X, u8 A_u8Y);

void SNAKE_vHandleIRCommand(u8 A_u8Command)
{
    switch (A_u8Command)
    {
        case HIR_CMD_POWER:
            G_u8ExitFlag = 1;
            break;

        case HIR_CMD_2:
            if (G_u8Direction != SNAKE_DIR_DOWN)
                G_u8RequestedDirection = SNAKE_DIR_UP;
            break;

        case HIR_CMD_6:
            if (G_u8Direction != SNAKE_DIR_LEFT)
                G_u8RequestedDirection = SNAKE_DIR_RIGHT;
            break;

        case HIR_CMD_8:
            if (G_u8Direction != SNAKE_DIR_UP)
                G_u8RequestedDirection = SNAKE_DIR_DOWN;
            break;

        case HIR_CMD_4:
            if (G_u8Direction != SNAKE_DIR_RIGHT)
                G_u8RequestedDirection = SNAKE_DIR_LEFT;
            break;

        default:
            break;
    }
}

static void SNAKE_vDrawCell(u8 A_u8X, u8 A_u8Y, u16 A_u16Color)
{
    u16 L_u16X = SNAKE_BOARD_X + (A_u8X * SNAKE_CELL_SIZE) + 1;
    u16 L_u16Y = SNAKE_BOARD_Y + (A_u8Y * SNAKE_CELL_SIZE) + 1;

    HTFT_vSetXPos(L_u16X, L_u16X + SNAKE_CELL_SIZE - 2);
    HTFT_vSetYPos(L_u16Y, L_u16Y + SNAKE_CELL_SIZE - 2);
    HTFT_vFillRectangle(A_u16Color);
}

static u8 SNAKE_u8IsSnakeCell(u8 A_u8X, u8 A_u8Y)
{
    u8 L_u8Index;

    for (L_u8Index = 0; L_u8Index < G_u8SnakeLength; L_u8Index++)
    {
        if (G_u8SnakeX[L_u8Index] == A_u8X &&
            G_u8SnakeY[L_u8Index] == A_u8Y)
            return 1;
    }

    return 0;
}

static void SNAKE_vPlaceFood(void)
{
    u8 L_u8Y;
    u8 L_u8X;

    for (L_u8Y = 0; L_u8Y < SNAKE_BOARD_ROWS; L_u8Y++)
    {
        for (L_u8X = 0; L_u8X < SNAKE_BOARD_COLS; L_u8X++)
        {
            if (!SNAKE_u8IsSnakeCell(L_u8X, L_u8Y))
            {
                G_u8FoodX = L_u8X;
                G_u8FoodY = L_u8Y;
                return;
            }
        }
    }
}

static void SNAKE_vDrawBoard(void)
{
    HTFT_vFillBackgroundColor(TFT_BLACK);
    HTFT_vWriteString(49, 143, "SNAKE", TFT_CYAN, TFT_BLACK, 1);
    HTFT_vDrawGrid(
        SNAKE_BOARD_X,
        SNAKE_BOARD_X + (SNAKE_BOARD_COLS * SNAKE_CELL_SIZE),
        SNAKE_BOARD_Y,
        SNAKE_BOARD_Y + (SNAKE_BOARD_ROWS * SNAKE_CELL_SIZE),
        TFT_WHITE,
        TFT_BLACK,
        SNAKE_BOARD_COLS,
        SNAKE_BOARD_ROWS
    );
}

static void SNAKE_vDrawRound(void)
{
    u8 L_u8Index;

    SNAKE_vDrawBoard();
    SNAKE_vDrawCell(G_u8FoodX, G_u8FoodY, TFT_RED);

    for (L_u8Index = 0; L_u8Index < G_u8SnakeLength; L_u8Index++)
    {
        SNAKE_vDrawCell(
            G_u8SnakeX[L_u8Index],
            G_u8SnakeY[L_u8Index],
            TFT_GREEN
        );
    }
}

static void SNAKE_vResetRound(void)
{
    G_u8SnakeLength = 3;
    G_u8SnakeX[0] = 6;
    G_u8SnakeY[0] = 5;
    G_u8SnakeX[1] = 5;
    G_u8SnakeY[1] = 5;
    G_u8SnakeX[2] = 4;
    G_u8SnakeY[2] = 5;
    G_u8Direction = SNAKE_DIR_RIGHT;
    G_u8RequestedDirection = SNAKE_DIR_RIGHT;
    SNAKE_vPlaceFood();
}

static void SNAKE_vMove(void)
{
    s8 L_s8NextX = G_u8SnakeX[0];
    s8 L_s8NextY = G_u8SnakeY[0];
    u8 L_u8Index;
    u8 L_u8AteFood;

    G_u8Direction = G_u8RequestedDirection;

    if (G_u8Direction == SNAKE_DIR_UP)
        L_s8NextY--;
    else if (G_u8Direction == SNAKE_DIR_RIGHT)
        L_s8NextX++;
    else if (G_u8Direction == SNAKE_DIR_DOWN)
        L_s8NextY++;
    else
        L_s8NextX--;

    if (L_s8NextX < 0 || L_s8NextX >= SNAKE_BOARD_COLS ||
        L_s8NextY < 0 || L_s8NextY >= SNAKE_BOARD_ROWS ||
        SNAKE_u8IsSnakeCell((u8)L_s8NextX, (u8)L_s8NextY))
    {
        SNAKE_vResetRound();
        SNAKE_vDrawRound();
        return;
    }

    L_u8AteFood = ((u8)L_s8NextX == G_u8FoodX &&
                   (u8)L_s8NextY == G_u8FoodY);

    if (L_u8AteFood && G_u8SnakeLength < SNAKE_MAX_LENGTH)
        G_u8SnakeLength++;
    else
        SNAKE_vDrawCell(G_u8SnakeX[G_u8SnakeLength - 1],
                        G_u8SnakeY[G_u8SnakeLength - 1], TFT_BLACK);

    for (L_u8Index = G_u8SnakeLength - 1; L_u8Index > 0; L_u8Index--)
    {
        G_u8SnakeX[L_u8Index] = G_u8SnakeX[L_u8Index - 1];
        G_u8SnakeY[L_u8Index] = G_u8SnakeY[L_u8Index - 1];
    }

    G_u8SnakeX[0] = (u8)L_s8NextX;
    G_u8SnakeY[0] = (u8)L_s8NextY;
    SNAKE_vDrawCell(G_u8SnakeX[0], G_u8SnakeY[0], TFT_GREEN);

    if (L_u8AteFood)
    {
        SNAKE_vPlaceFood();
        SNAKE_vDrawCell(G_u8FoodX, G_u8FoodY, TFT_RED);
    }
}

void SNAKE_vInit(void)
{
    G_u8ExitFlag = 0;
    SNAKE_vResetRound();
    SNAKE_vDrawRound();
    G_u32LastMoveTime = MSYSTICK_u32GetElapsedTime();
}

void SNAKE_vPlay(void)
{
    SNAKE_vInit();

    while (G_u8ExitFlag == 0)
    {
        if ((MSYSTICK_u32GetElapsedTime() - G_u32LastMoveTime) >=
            SNAKE_MOVE_PERIOD_US)
        {
            G_u32LastMoveTime = MSYSTICK_u32GetElapsedTime();
            SNAKE_vMove();
        }
    }
}
