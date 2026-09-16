#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/TIM/TIM_int.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../../HAL/IR/IR_int.h"
#include "../SCORE/SCORE_int.h"
#include "TICTACTOE_int.h"

#define TICTACTOE_SIZE       3
#define TICTACTOE_BOARD_SIZE 100
#define TICTACTOE_CELL_SIZE  (TICTACTOE_BOARD_SIZE / TICTACTOE_SIZE)
#define TICTACTOE_BOARD_X    ((128 - TICTACTOE_BOARD_SIZE) / 2)
#define TICTACTOE_BOARD_Y    30

#define TICTACTOE_MOVE_NONE  0
#define TICTACTOE_MOVE_LEFT  1
#define TICTACTOE_MOVE_RIGHT 2
#define TICTACTOE_MOVE_UP    3
#define TICTACTOE_MOVE_DOWN  4

static u8 G_u8Board[TICTACTOE_SIZE][TICTACTOE_SIZE];
static volatile u8 G_u8CursorRow;
static volatile u8 G_u8CursorCol;
static volatile u8 G_u8CursorMove;
static volatile u8 G_u8PlaceFlag;
static volatile u8 G_u8ExitFlag;
static u8 G_u8CurrentPlayer;

static void TICTACTOE_vDrawCell(u8 A_u8Row, u8 A_u8Col);
static void TICTACTOE_vDrawCursor(u8 A_u8Row, u8 A_u8Col, u16 A_u16Color);
static void TICTACTOE_vUpdateCursor(void);
static u8 TICTACTOE_u8HasWinner(u8 A_u8Player);
static u8 TICTACTOE_u8IsDraw(void);

void TICTACTOE_vBtnLeft(void)
{
    if (G_u8CursorCol > 0)
        G_u8CursorMove = TICTACTOE_MOVE_LEFT;
}

void TICTACTOE_vBtnRight(void)
{
    if (G_u8CursorCol < TICTACTOE_SIZE - 1)
        G_u8CursorMove = TICTACTOE_MOVE_RIGHT;
}

void TICTACTOE_vBtnUp(void)
{
    if (G_u8CursorRow > 0)
        G_u8CursorMove = TICTACTOE_MOVE_UP;
}

void TICTACTOE_vBtnDown(void)
{
    if (G_u8CursorRow < TICTACTOE_SIZE - 1)
        G_u8CursorMove = TICTACTOE_MOVE_DOWN;
}

void TICTACTOE_vBtnPlace(void)
{
    G_u8PlaceFlag = 1;
}

void TICTACTOE_vBtnExit(void)
{
    G_u8ExitFlag = 1;
}

void TICTACTOE_vHandleIRCommand(u8 A_u8Command)
{
    switch (A_u8Command)
    {
        case HIR_CMD_POWER:
            TICTACTOE_vBtnExit();
            break;

        case HIR_CMD_4:
            TICTACTOE_vBtnLeft();
            break;

        case HIR_CMD_6:
            TICTACTOE_vBtnRight();
            break;

        case HIR_CMD_2:
            TICTACTOE_vBtnUp();
            break;

        case HIR_CMD_8:
            TICTACTOE_vBtnDown();
            break;

        case HIR_CMD_5:
            TICTACTOE_vBtnPlace();
            break;

        default:
            break;
    }
}

static void TICTACTOE_vDrawCell(u8 A_u8Row, u8 A_u8Col)
{
    u16 L_u16X = TICTACTOE_BOARD_X + A_u8Col * TICTACTOE_CELL_SIZE;
    u16 L_u16Y = TICTACTOE_BOARD_Y + A_u8Row * TICTACTOE_CELL_SIZE;
    const char *L_pcMark = 0;
    u16 L_u16Color = TFT_WHITE;

    HTFT_vSetXPos(L_u16X + 2, L_u16X + TICTACTOE_CELL_SIZE - 2);
    HTFT_vSetYPos(L_u16Y + 2, L_u16Y + TICTACTOE_CELL_SIZE - 2);
    HTFT_vFillRectangle(TFT_BLACK);

    if (G_u8Board[A_u8Row][A_u8Col] == 1)
    {
        L_pcMark = "X";
        L_u16Color = TFT_RED;
    }
    else if (G_u8Board[A_u8Row][A_u8Col] == 2)
    {
        L_pcMark = "O";
        L_u16Color = TFT_YELLOW;
    }

    if (L_pcMark != 0)
    {
        HTFT_vWriteString(
            L_u16X + 11,
            L_u16Y + 8,
            L_pcMark,
            L_u16Color,
            TFT_BLACK,
            2
        );
    }
}

static void TICTACTOE_vDrawCursor(u8 A_u8Row, u8 A_u8Col, u16 A_u16Color)
{
    u16 L_u16X = TICTACTOE_BOARD_X + A_u8Col * TICTACTOE_CELL_SIZE;
    u16 L_u16Y = TICTACTOE_BOARD_Y + A_u8Row * TICTACTOE_CELL_SIZE;

    HTFT_vDrawHorizontalLine(
        L_u16Y + 1,
        L_u16X + 1,
        L_u16X + TICTACTOE_CELL_SIZE - 1,
        A_u16Color
    );
    HTFT_vDrawHorizontalLine(
        L_u16Y + TICTACTOE_CELL_SIZE - 1,
        L_u16X + 1,
        L_u16X + TICTACTOE_CELL_SIZE - 1,
        A_u16Color
    );
    HTFT_vDrawVerticalLine(
        L_u16X + 1,
        L_u16Y + 1,
        L_u16Y + TICTACTOE_CELL_SIZE - 1,
        A_u16Color
    );
    HTFT_vDrawVerticalLine(
        L_u16X + TICTACTOE_CELL_SIZE - 1,
        L_u16Y + 1,
        L_u16Y + TICTACTOE_CELL_SIZE - 1,
        A_u16Color
    );
}

static void TICTACTOE_vUpdateCursor(void)
{
    u8 L_u8Move = G_u8CursorMove;

    if (L_u8Move == TICTACTOE_MOVE_NONE)
        return;

    G_u8CursorMove = TICTACTOE_MOVE_NONE;
    TICTACTOE_vDrawCell(G_u8CursorRow, G_u8CursorCol);
    TICTACTOE_vDrawCursor(G_u8CursorRow, G_u8CursorCol, TFT_WHITE);

    switch (L_u8Move)
    {
        case TICTACTOE_MOVE_LEFT:
            G_u8CursorCol--;
            break;

        case TICTACTOE_MOVE_RIGHT:
            G_u8CursorCol++;
            break;

        case TICTACTOE_MOVE_UP:
            G_u8CursorRow--;
            break;

        case TICTACTOE_MOVE_DOWN:
            G_u8CursorRow++;
            break;

        default:
            break;
    }

    TICTACTOE_vDrawCursor(G_u8CursorRow, G_u8CursorCol, TFT_CYAN);
}

static u8 TICTACTOE_u8HasWinner(u8 A_u8Player)
{
    u8 L_u8Index;

    for (L_u8Index = 0; L_u8Index < TICTACTOE_SIZE; L_u8Index++)
    {
        if (G_u8Board[L_u8Index][0] == A_u8Player &&
            G_u8Board[L_u8Index][1] == A_u8Player &&
            G_u8Board[L_u8Index][2] == A_u8Player)
            return 1;

        if (G_u8Board[0][L_u8Index] == A_u8Player &&
            G_u8Board[1][L_u8Index] == A_u8Player &&
            G_u8Board[2][L_u8Index] == A_u8Player)
            return 1;
    }

    if (G_u8Board[0][0] == A_u8Player &&
        G_u8Board[1][1] == A_u8Player &&
        G_u8Board[2][2] == A_u8Player)
        return 1;

    if (G_u8Board[0][2] == A_u8Player &&
        G_u8Board[1][1] == A_u8Player &&
        G_u8Board[2][0] == A_u8Player)
        return 1;

    return 0;
}

static u8 TICTACTOE_u8IsDraw(void)
{
    u8 L_u8Row;
    u8 L_u8Col;

    for (L_u8Row = 0; L_u8Row < TICTACTOE_SIZE; L_u8Row++)
    {
        for (L_u8Col = 0; L_u8Col < TICTACTOE_SIZE; L_u8Col++)
        {
            if (G_u8Board[L_u8Row][L_u8Col] == 0)
                return 0;
        }
    }

    return 1;
}

void TICTACTOE_vInit(void)
{
    u8 L_u8Row;
    u8 L_u8Col;

    for (L_u8Row = 0; L_u8Row < TICTACTOE_SIZE; L_u8Row++)
    {
        for (L_u8Col = 0; L_u8Col < TICTACTOE_SIZE; L_u8Col++)
            G_u8Board[L_u8Row][L_u8Col] = 0;
    }

    G_u8CursorRow = 1;
    G_u8CursorCol = 1;
    G_u8CursorMove = TICTACTOE_MOVE_NONE;
    G_u8PlaceFlag = 0;
    G_u8ExitFlag = 0;
    G_u8CurrentPlayer = 1;

    HTFT_vFillBackgroundColor(TFT_BLACK);
    HTFT_vWriteString(31, 143, "TIC TAC TOE", TFT_CYAN, TFT_BLACK, 1);
    HTFT_vDrawGrid(
        TICTACTOE_BOARD_X,
        TICTACTOE_BOARD_X + TICTACTOE_BOARD_SIZE,
        TICTACTOE_BOARD_Y,
        TICTACTOE_BOARD_Y + TICTACTOE_BOARD_SIZE,
        TFT_WHITE,
        TFT_BLACK,
        TICTACTOE_SIZE,
        TICTACTOE_SIZE
    );
    TICTACTOE_vDrawCursor(G_u8CursorRow, G_u8CursorCol, TFT_CYAN);
}

void TICTACTOE_vPlay(void)
{
    TICTACTOE_vInit();

    while (G_u8ExitFlag == 0)
    {
        TICTACTOE_vUpdateCursor();

        if (G_u8PlaceFlag)
        {
            G_u8PlaceFlag = 0;

            if (G_u8Board[G_u8CursorRow][G_u8CursorCol] == 0)
            {
                G_u8Board[G_u8CursorRow][G_u8CursorCol] = G_u8CurrentPlayer;
                TICTACTOE_vDrawCell(G_u8CursorRow, G_u8CursorCol);
                TICTACTOE_vDrawCursor(G_u8CursorRow, G_u8CursorCol, TFT_CYAN);

                if (TICTACTOE_u8HasWinner(G_u8CurrentPlayer))
                {
                    SCORE_vIncrement((G_u8CurrentPlayer == 1) ?
                                     SCORE_PLAYER_ONE : SCORE_PLAYER_TWO);

                    if (G_u8CurrentPlayer == 1)
                        HTFT_vWriteString(35, 10, "X WINS", TFT_RED, TFT_BLACK, 1);
                    else
                        HTFT_vWriteString(35, 10, "O WINS", TFT_YELLOW, TFT_BLACK, 1);

                    MTIM_vSetDelayMS(2500);
                    HIR_vResume();

                    if (G_u8ExitFlag == 0)
                        TICTACTOE_vInit();
                }
                else if (TICTACTOE_u8IsDraw())
                {
                    HTFT_vWriteString(35, 10, "DRAW", TFT_CYAN, TFT_BLACK, 1);
                    MTIM_vSetDelayMS(1500);
                    HIR_vResume();

                    if (G_u8ExitFlag == 0)
                        TICTACTOE_vInit();
                }
                else
                {
                    G_u8CurrentPlayer = (G_u8CurrentPlayer == 1) ? 2 : 1;
                }
            }
        }
    }
}
