#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/TIM/TIM_int.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../../HAL/IR/IR_int.h"
#include "CONNECT4_int.h"

/* Display & Board Layout Constants for 128x160 TFT */
#define CELL_WIDTH   14
#define CELL_HEIGHT  18

#define TOTAL_WIDTH  (CONNECT4_COLS * CELL_WIDTH)   /* 98 pixels wide */
#define TOTAL_HEIGHT (CONNECT4_ROWS * CELL_HEIGHT)  /* 108 pixels high */

#define BOARD_X_OFFSET  ((128 - TOTAL_WIDTH) / 2)   /* 15 pixels padding */
#define BOARD_Y_OFFSET  ((160 - TOTAL_HEIGHT) / 2)  /* 26 pixels padding */

/* Internal Encapsulated Game Variables */
static u8          G_u8Board[CONNECT4_ROWS][CONNECT4_COLS];
static volatile u8 G_u8SelectedCol  = 3; // Center column default
static volatile u8 G_u8CursorMoveFlag = 0;
static volatile u8 G_u8DropCoinFlag = 0;
static volatile u8 G_u8ExitGameFlag = 0;
static u8          G_u8CurrentPlayer = 1; // 1: Red, 2: Yellow

/* Private Helper Prototypes */
static void ConnectFour_vDrawColumnCursor(u8 A_u8Col, u16 A_u16Color);
static void ConnectFour_vUpdateCursor(void);
static u8   ConnectFour_u8CheckWin(u8 A_u8Player);

// ================= Direct EXTI Input Callbacks =================

void CONNECT4_vBtnLeft_ISR(void)
{
    if (G_u8SelectedCol > 0)
        G_u8CursorMoveFlag = 1;
}

void CONNECT4_vBtnDrop_ISR(void)
{
    G_u8DropCoinFlag = 1; /* Request piece placement in game task context */
}

void CONNECT4_vBtnRight_ISR(void)
{
    if (G_u8SelectedCol < (CONNECT4_COLS - 1))
        G_u8CursorMoveFlag = 2;
}

void CONNECT4_vBtnExit_ISR(void)
{
    G_u8ExitGameFlag = 1;
}

void CONNECT4_vHandleIRCommand(u8 A_u8Command)
{
    switch (A_u8Command)
    {
        case HIR_CMD_POWER:
            CONNECT4_vBtnExit_ISR();
            break;

        case HIR_CMD_4:
            CONNECT4_vBtnLeft_ISR();
            break;

        case HIR_CMD_6:
            CONNECT4_vBtnRight_ISR();
            break;

        case HIR_CMD_5:
            CONNECT4_vBtnDrop_ISR();
            break;

        default:
            break;
    }
}

// ================= Internal Logic & Rendering =================

static void ConnectFour_vDrawColumnCursor(u8 A_u8Col, u16 A_u16Color)
{
    u16 L_u16Px = BOARD_X_OFFSET + (A_u8Col * CELL_WIDTH) + 2;
    u16 L_u16Py = BOARD_Y_OFFSET - 8; /* Positioned right above column header */

    HTFT_vSetXPos(L_u16Px, L_u16Px + CELL_WIDTH - 6);
    HTFT_vSetYPos(L_u16Py, L_u16Py + 4);
    HTFT_vFillRectangle(A_u16Color);
}

static void ConnectFour_vUpdateCursor(void)
{
    u8 L_u8Move = G_u8CursorMoveFlag;

    if (L_u8Move == 0)
        return;

    G_u8CursorMoveFlag = 0;
    ConnectFour_vDrawColumnCursor(
        G_u8SelectedCol,
        TFT_BLACK
    );

    if (L_u8Move == 1)
        G_u8SelectedCol--;
    else if (L_u8Move == 2)
        G_u8SelectedCol++;

    ConnectFour_vDrawColumnCursor(
        G_u8SelectedCol,
        (G_u8CurrentPlayer == 1) ? TFT_RED : TFT_YELLOW
    );
}

static u8 ConnectFour_u8CheckWin(u8 A_u8Player)
{
    u8 r, c;

    /* 1. Horizontal Check */
    for (r = 0; r < CONNECT4_ROWS; r++)
    {
        for (c = 0; c < CONNECT4_COLS - 3; c++)
        {
            if (G_u8Board[r][c] == A_u8Player && G_u8Board[r][c+1] == A_u8Player &&
                G_u8Board[r][c+2] == A_u8Player && G_u8Board[r][c+3] == A_u8Player)
                return 1;
        }
    }

    /* 2. Vertical Check */
    for (r = 0; r < CONNECT4_ROWS - 3; r++)
    {
        for (c = 0; c < CONNECT4_COLS; c++)
        {
            if (G_u8Board[r][c] == A_u8Player && G_u8Board[r+1][c] == A_u8Player &&
                G_u8Board[r+2][c] == A_u8Player && G_u8Board[r+3][c] == A_u8Player)
                return 1;
        }
    }

    /* 3. Positive Diagonal Check */
    for (r = 0; r < CONNECT4_ROWS - 3; r++)
    {
        for (c = 0; c < CONNECT4_COLS - 3; c++)
        {
            if (G_u8Board[r][c] == A_u8Player && G_u8Board[r+1][c+1] == A_u8Player &&
                G_u8Board[r+2][c+2] == A_u8Player && G_u8Board[r+3][c+3] == A_u8Player)
                return 1;
        }
    }

    /* 4. Negative Diagonal Check */
    for (r = 3; r < CONNECT4_ROWS; r++)
    {
        for (c = 0; c < CONNECT4_COLS - 3; c++)
        {
            if (G_u8Board[r][c] == A_u8Player && G_u8Board[r-1][c+1] == A_u8Player &&
                G_u8Board[r-2][c+2] == A_u8Player && G_u8Board[r-3][c+3] == A_u8Player)
                return 1;
        }
    }

    return 0;
}

void CONNECT4_vInit(void)
{
    for (u8 r = 0; r < CONNECT4_ROWS; r++)
    {
        for (u8 c = 0; c < CONNECT4_COLS; c++)
        {
            G_u8Board[r][c] = 0;
        }
    }
    G_u8SelectedCol   = 3;
    G_u8CursorMoveFlag = 0;
    G_u8CurrentPlayer = 1;
    G_u8DropCoinFlag  = 0;
    G_u8ExitGameFlag  = 0;

    /* Render Screen Framework */
    HTFT_vFillBackgroundColor(TFT_BLACK);
    HTFT_vDrawGrid(BOARD_X_OFFSET, BOARD_X_OFFSET + TOTAL_WIDTH,
                   BOARD_Y_OFFSET, BOARD_Y_OFFSET + TOTAL_HEIGHT,
                   TFT_WHITE, TFT_BLACK, CONNECT4_COLS, CONNECT4_ROWS);

    HTFT_vWriteString(37, 5, "CONNECT 4", TFT_CYAN, TFT_BLACK, 1);
    ConnectFour_vDrawColumnCursor(G_u8SelectedCol, TFT_RED);
}

void CONNECT4_vPlay(void)
{
    CONNECT4_vInit();

    while (G_u8ExitGameFlag == 0)
    {
        ConnectFour_vUpdateCursor();

        if (G_u8DropCoinFlag)
        {
            G_u8DropCoinFlag = 0;

            /* Locate bottom-most open slot in chosen column */
            s8 L_s8AvailableRow = -1;
            for (s8 r = CONNECT4_ROWS - 1; r >= 0; r--)
            {
                if (G_u8Board[r][G_u8SelectedCol] == 0)
                {
                    L_s8AvailableRow = r;
                    break;
                }
            }

            if (L_s8AvailableRow != -1)
            {
                G_u8Board[L_s8AvailableRow][G_u8SelectedCol] = G_u8CurrentPlayer;

                u16 L_u16Color = (G_u8CurrentPlayer == 1) ? TFT_RED : TFT_YELLOW;
                u16 L_u16Px = BOARD_X_OFFSET + (G_u8SelectedCol * CELL_WIDTH) + 2;
                u16 L_u16Py = BOARD_Y_OFFSET + (L_s8AvailableRow * CELL_HEIGHT) + 2;

                /* Fill Slot Window */
                HTFT_vSetXPos(L_u16Px, L_u16Px + CELL_WIDTH - 5);
                HTFT_vSetYPos(L_u16Py, L_u16Py + CELL_HEIGHT - 5);
                HTFT_vFillRectangle(L_u16Color);

                /* Win Check */
                if (ConnectFour_u8CheckWin(G_u8CurrentPlayer))
                {
                    if (G_u8CurrentPlayer == 1)
                        HTFT_vWriteString(28, 142, "RED WINS!", TFT_RED, TFT_BLACK, 1);
                    else
                        HTFT_vWriteString(19, 142, "YELLOW WINS!", TFT_YELLOW, TFT_BLACK, 1);

                    MTIM_vSetDelayMS(2500);
                    HIR_vResume();

                    /*
                     * Start a new match after showing the result. Keep an
                     * exit request received during the delay intact.
                     */
                    if (G_u8ExitGameFlag == 0)
                    {
                        CONNECT4_vInit();
                    }
                }
                else
                {
                    /* Toggle Active Turn */
                    G_u8CurrentPlayer = (G_u8CurrentPlayer == 1) ? 2 : 1;
                    ConnectFour_vDrawColumnCursor(G_u8SelectedCol, (G_u8CurrentPlayer == 1) ? TFT_RED : TFT_YELLOW);
                }
            }
        }
    }
}
