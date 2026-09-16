#include "../../LIB/STD_TYPES.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../../HAL/IR/IR_int.h"
#include "../../MCAL/TIM/TIM_int.h"
#include "../SCORE/SCORE_int.h"
#include "SNAKE_int.h"

#define SNAKE_BOARD_X       0
#define SNAKE_BOARD_Y       0
#define SNAKE_BOARD_COLS    16
#define SNAKE_BOARD_ROWS    18
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
static u32 G_u32MoveElapsed;
static u32 G_u32RandomState = 1;

static void SNAKE_vDrawCell(u8 A_u8X, u8 A_u8Y, u16 A_u16Color);
static void SNAKE_vDrawBoard(void);
static void SNAKE_vDrawRound(void);
static void SNAKE_vPlaceFood(void);
static void SNAKE_vResetRound(void);
static void SNAKE_vMove(void);
static u8 SNAKE_u8IsSnakeCell(u8 A_u8X, u8 A_u8Y);
static u32 SNAKE_u32GetRandom(void);

void SNAKE_vSetRandomSeed(u32 A_u32Seed)
{
    if (A_u32Seed == 0)
        A_u32Seed = 1;

    G_u32RandomState = A_u32Seed;
}

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

static u32 SNAKE_u32GetRandom(void)
{
    G_u32RandomState ^= G_u32RandomState << 13;
    G_u32RandomState ^= G_u32RandomState >> 17;
    G_u32RandomState ^= G_u32RandomState << 5;

    return G_u32RandomState;
}

static void SNAKE_vPlaceFood(void)
{
    u16 L_u16FreeCells = 0;
    u16 L_u16SelectedFreeCell;
    u16 L_u16FreeCellIndex = 0;
    u8 L_u8Y;
    u8 L_u8X;

    for (L_u8Y = 0; L_u8Y < SNAKE_BOARD_ROWS; L_u8Y++)
    {
        for (L_u8X = 0; L_u8X < SNAKE_BOARD_COLS; L_u8X++)
        {
            if (!SNAKE_u8IsSnakeCell(L_u8X, L_u8Y))
                L_u16FreeCells++;
        }
    }

    if (L_u16FreeCells == 0)
        return;

    L_u16SelectedFreeCell = (u16)(SNAKE_u32GetRandom() % L_u16FreeCells);

    for (L_u8Y = 0; L_u8Y < SNAKE_BOARD_ROWS; L_u8Y++)
    {
        for (L_u8X = 0; L_u8X < SNAKE_BOARD_COLS; L_u8X++)
        {
            if (!SNAKE_u8IsSnakeCell(L_u8X, L_u8Y))
            {
                if (L_u16FreeCellIndex == L_u16SelectedFreeCell)
                {
                    G_u8FoodX = L_u8X;
                    G_u8FoodY = L_u8Y;
                    return;
                }

                L_u16FreeCellIndex++;
            }
        }
    }
}

static void SNAKE_vDrawBoard(void)
{
    HTFT_vFillBackgroundColor(TFT_BLACK);
    HTFT_vWriteString(49, 148, "SNAKE", TFT_CYAN, TFT_BLACK, 1);

    HTFT_vDrawHorizontalLine(
        145,
        SNAKE_BOARD_X,
        127,
        TFT_WHITE
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
    u8 L_u8Direction = (u8)(SNAKE_u32GetRandom() % 4);

    G_u8SnakeLength = 3;

    if (L_u8Direction == SNAKE_DIR_UP)
    {
        G_u8SnakeX[0] = 6; G_u8SnakeY[0] = 5;
        G_u8SnakeX[1] = 6; G_u8SnakeY[1] = 6;
        G_u8SnakeX[2] = 6; G_u8SnakeY[2] = 7;
    }
    else if (L_u8Direction == SNAKE_DIR_RIGHT)
    {
        G_u8SnakeX[0] = 5; G_u8SnakeY[0] = 5;
        G_u8SnakeX[1] = 4; G_u8SnakeY[1] = 5;
        G_u8SnakeX[2] = 3; G_u8SnakeY[2] = 5;
    }
    else if (L_u8Direction == SNAKE_DIR_DOWN)
    {
        G_u8SnakeX[0] = 6; G_u8SnakeY[0] = 4;
        G_u8SnakeX[1] = 6; G_u8SnakeY[1] = 3;
        G_u8SnakeX[2] = 6; G_u8SnakeY[2] = 2;
    }
    else
    {
        G_u8SnakeX[0] = 7; G_u8SnakeY[0] = 5;
        G_u8SnakeX[1] = 8; G_u8SnakeY[1] = 5;
        G_u8SnakeX[2] = 9; G_u8SnakeY[2] = 5;
    }

    G_u8Direction = L_u8Direction;
    G_u8RequestedDirection = L_u8Direction;
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
        SCORE_vResetSnake();
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
        SCORE_vIncrementSnakeItem();
        SNAKE_vPlaceFood();
        SNAKE_vDrawCell(G_u8FoodX, G_u8FoodY, TFT_RED);
    }
}

void SNAKE_vInit(void)
{
    G_u8ExitFlag = 0;
    SCORE_vResetSnake();
    SNAKE_vResetRound();
    SNAKE_vDrawRound();
    G_u32LastMoveTime = MTIM_u32GetCounter();
    G_u32MoveElapsed = 0;
}

void SNAKE_vPlay(void)
{
    SNAKE_vInit();

    while (G_u8ExitFlag == 0)
    {
        G_u32MoveElapsed += MTIM_u32GetElapsed(G_u32LastMoveTime);
        G_u32LastMoveTime = MTIM_u32GetCounter();

        if (G_u32MoveElapsed >= SNAKE_MOVE_PERIOD_US)
        {
            G_u32MoveElapsed -= SNAKE_MOVE_PERIOD_US;
            SNAKE_vMove();
        }
    }
}
