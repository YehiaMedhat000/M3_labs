#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../HAL/TFT/TFT_int.h"
#include "../../MCAL/NVIC/NVIC_int.h"
#include "../../MCAL/AFIO/AFIO_int.h"
#include "../../MCAL/EXTI/EXTI_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"
#include "../../HAL/IR/IR_int.h"
#include "../CONNECT4/CONNECT4_int.h"
#include "../TICTACTOE/TICTACTOE_int.h"
#include "../SNAKE/SNAKE_int.h"
#include "GAME_int.h"

#define TOTAL_MENU_ITEMS 3

/* Encapsulated State Variables */
static volatile AppState_t G_u8CurrentState      = STATE_MENU;
static volatile u8         G_u8CurrentSelection  = 0;
static volatile u8         G_u8PreviousSelection = 0;
static volatile u8         G_u8FullDrawFlag      = 1;
static volatile u8         G_u8PartialUpdateFlag = 0;
static volatile u8         G_u8ExitGameFlag      = 0;

static const char *G_pcGameList[TOTAL_MENU_ITEMS] = {
    "CONNECT 4",
    "TIC TAC TOE",
    "SNAKE GAME"
};

/* Private Function Prototypes */
static void GAME_vRenderSingleItem(u8 A_u8ItemIndex, u8 A_u8IsHighlighted);
static void GAME_vSetConnect4ISRsCallBack(void);

// ================= Interrupt Service Callbacks =================

void GAME_vInitISRs(void)
{
    MEXTI_vEnableINT(L0);
    MEXTI_vSetTrigger(L0, EXTI_Falling);
    MAFIO_vSetLinePort(L0, AFIO_PORT_A);

    MEXTI_vEnableINT(L1);
    MEXTI_vSetTrigger(L1, EXTI_Falling);
	MAFIO_vSetLinePort(L1, AFIO_PORT_A);

	MEXTI_vEnableINT(L2);
	MEXTI_vSetTrigger(L2, EXTI_Falling);
	MAFIO_vSetLinePort(L2, AFIO_PORT_A);

	MEXTI_vEnableINT(L3);
	MEXTI_vSetTrigger(L3, EXTI_Falling);
	MAFIO_vSetLinePort(L3, AFIO_PORT_A);

    MNVIC_vEnablePeripheralINT(NVIC_EXTI0);
    MNVIC_vEnablePeripheralINT(NVIC_EXTI1);
    MNVIC_vEnablePeripheralINT(NVIC_EXTI2);
    MNVIC_vEnablePeripheralINT(NVIC_EXTI3);
}

void GAME_vNavigateDownISR(void)
{
    if (G_u8CurrentState == STATE_MENU)
    {
        /* Save current position before advancing */
        G_u8PreviousSelection = G_u8CurrentSelection;

        if (G_u8CurrentSelection == 0)
        {
            G_u8CurrentSelection = TOTAL_MENU_ITEMS - 1;
        }
        else
        {
            G_u8CurrentSelection--;
        }

        /* Signal partial update instead of full redraw */
        G_u8PartialUpdateFlag = 1;
    }
}

void GAME_vNavigateUPISR(void)
{
    if (G_u8CurrentState == STATE_MENU)
    {
        /* Save current position before advancing */
        G_u8PreviousSelection = G_u8CurrentSelection;

        if (G_u8CurrentSelection == (TOTAL_MENU_ITEMS - 1))
        {
            G_u8CurrentSelection = 0;
        }
        else
        {
            G_u8CurrentSelection++;
        }

        /* Signal partial update instead of full redraw */
        G_u8PartialUpdateFlag = 1;
    }
}

void GAME_vSelectISR(void)
{
    if (G_u8CurrentState == STATE_MENU)
    {
        G_u8CurrentState = (AppState_t)(G_u8CurrentSelection + 1);

        if (G_u8CurrentState == STATE_SNAKE)
            SNAKE_vSetRandomSeed(MSYSTICK_u32GetElapsedTime());
    }
}

void GAME_vHandleIRCommand(u8 A_u8Command)
{
    if (G_u8CurrentState == STATE_MENU)
    {
        switch (A_u8Command)
        {
            case HIR_CMD_REWIND:
                GAME_vNavigateDownISR();
                break;

            case HIR_CMD_FAST_FORWARD:
                GAME_vNavigateUPISR();
                break;

            case HIR_CMD_EQ:
                GAME_vSelectISR();
                break;

            default:
                break;
        }
    }
    else if (G_u8CurrentState == STATE_CONNECT_FOUR)
    {
        CONNECT4_vHandleIRCommand(A_u8Command);
    }
    else if (G_u8CurrentState == STATE_TIC_TAC_TOE)
    {
        TICTACTOE_vHandleIRCommand(A_u8Command);
    }
    else if (G_u8CurrentState == STATE_SNAKE)
    {
        SNAKE_vHandleIRCommand(A_u8Command);
    }
}

static void GAME_vSetConnect4ISRsCallBack(void)
{
    /*
     * The single EXTI line is reserved for NEC IR edge capture.
     * Connect 4 commands are routed by GAME_vHandleIRCommand()
     * after the IR driver decodes them, so no EXTI callback is
     * reassigned here.
     */
}

// ================= Menu Rendering Logic =================

/**
 * @brief Helper function to render or clear a single menu line item
 */
static void GAME_vRenderSingleItem(u8 A_u8ItemIndex, u8 A_u8IsHighlighted)
{
    u8 L_u8Len = 0;
    u16 L_u16StartX;
    u16 L_u16YPos = 125 - (A_u8ItemIndex * 16);
    const char *L_pcPtr = G_pcGameList[A_u8ItemIndex];

    /* Calculate length for centered alignment */
    while (*L_pcPtr++) L_u8Len++;
    L_u16StartX = (128 - (L_u8Len * 6)) / 2;

    if (A_u8IsHighlighted)
    {
        /* Highlighted: Cyan bounding box background, Black text */
        HTFT_vSetXPos(5, 122);
        HTFT_vSetYPos(L_u16YPos - 3, L_u16YPos + 12);
        HTFT_vFillRectangle(TFT_CYAN);

        HTFT_vWriteString(L_u16StartX, L_u16YPos, G_pcGameList[A_u8ItemIndex], TFT_BLACK, TFT_CYAN, 1);
    }
    else
    {
        /* Normal: Black background box, White text */
        HTFT_vSetXPos(5, 122);
        HTFT_vSetYPos(L_u16YPos - 3, L_u16YPos + 12);
        HTFT_vFillRectangle(TFT_BLACK);

        HTFT_vWriteString(L_u16StartX, L_u16YPos, G_pcGameList[A_u8ItemIndex], TFT_WHITE, TFT_BLACK, 1);
    }
}

/**
 * @brief Full menu redraw (only called once on startup or when returning from a game)
 */
void GAME_vDrawMenu(void)
{
    u8 L_u8Item;
    u8 L_u8Len = 0;
    u16 L_u16StartX;
    const char *L_pcTitle = "GAME CONSOLE";
    const char *L_pcPtr = L_pcTitle;

    /* Clear full screen background */
    HTFT_vFillBackgroundColor(TFT_BLACK);

    /* Render Centered Header Title */
    while (*L_pcPtr++) L_u8Len++;
    L_u16StartX = (128 - (L_u8Len * 6)) / 2;

    HTFT_vWriteString(L_u16StartX, 143, L_pcTitle, TFT_CYAN, TFT_BLACK, 1);
    HTFT_vDrawHorizontalLine(140, 10, 118, TFT_CYAN);

    /* Render all menu items once */
    for (L_u8Item = 0; L_u8Item < TOTAL_MENU_ITEMS; L_u8Item++)
    {
        GAME_vRenderSingleItem(L_u8Item, (L_u8Item == G_u8CurrentSelection));
    }

    G_u8FullDrawFlag = 0;
    G_u8PartialUpdateFlag = 0;
}

/**
 * @brief Fast partial update (only updates old and new selection lines)
 */
void GAME_vUpdateMenuSelection(void)
{
    /* 1. Un-highlight the previously selected line */
    GAME_vRenderSingleItem(G_u8PreviousSelection, 0);

    /* 2. Highlight the newly selected line */
    GAME_vRenderSingleItem(G_u8CurrentSelection, 1);

    G_u8PartialUpdateFlag = 0;
}

void GAME_vTaskHandler(void)
{
    switch (G_u8CurrentState)
    {
        case STATE_MENU:

        	if (G_u8FullDrawFlag)
                GAME_vDrawMenu();

            else if (G_u8PartialUpdateFlag)
                GAME_vUpdateMenuSelection();

            break;

        case STATE_CONNECT_FOUR:
        	GAME_vSetConnect4ISRsCallBack();
        	CONNECT4_vPlay();
        	G_u8CurrentState = STATE_MENU;
        	G_u8FullDrawFlag = 1;
        	GAME_vDrawMenu();
            break;

        case STATE_TIC_TAC_TOE:
            TICTACTOE_vPlay();
            G_u8CurrentState = STATE_MENU;
            G_u8FullDrawFlag = 1;
            GAME_vDrawMenu();
            break;

        case STATE_SNAKE:
            SNAKE_vPlay();
            G_u8CurrentState = STATE_MENU;
            G_u8FullDrawFlag = 1;
            GAME_vDrawMenu();
            break;

        default:
            G_u8CurrentState = STATE_MENU;
            G_u8FullDrawFlag = 1;
            break;
    }
}
