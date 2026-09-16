#include "../../LIB/STD_TYPES.h"
#include "../../HAL/SSD_S2P/SSD_S2P_int.h"
#include "SCORE_int.h"

static S2P_PinConfig_t *G_xS2P;
static u8 G_au8Score[2];
static u8 G_u8SnakeScore;

void SCORE_vInit(S2P_PinConfig_t *A_xS2P)
{
    G_xS2P = A_xS2P;
    G_au8Score[SCORE_PLAYER_ONE] = 0;
    G_au8Score[SCORE_PLAYER_TWO] = 0;
    G_u8SnakeScore = 0;
    HSSD_S2P_vInit(G_xS2P);
}

void SCORE_vReset(void)
{
    G_au8Score[SCORE_PLAYER_ONE] = 0;
    G_au8Score[SCORE_PLAYER_TWO] = 0;
    G_u8SnakeScore = 0;

    HSSD_S2P_vAssignNumber(G_xS2P, SCORE_PLAYER_ONE, 0);
    HSSD_S2P_vAssignNumber(G_xS2P, SCORE_PLAYER_TWO, 0);
}

void SCORE_vResetSnake(void)
{
    G_u8SnakeScore = 0;
    HSSD_S2P_vAssignNumber(G_xS2P, SCORE_PLAYER_ONE, 0);
    HSSD_S2P_vAssignNumber(G_xS2P, SCORE_PLAYER_TWO, 0);
}

void SCORE_vIncrementSnakeItem(void)
{
    if (G_xS2P == NULL)
        return;

    G_u8SnakeScore++;
    if (G_u8SnakeScore >= 100)
        G_u8SnakeScore = 0;

    HSSD_S2P_vAssignNumber(G_xS2P, SCORE_PLAYER_ONE,
                           G_u8SnakeScore / 10);
    HSSD_S2P_vAssignNumber(G_xS2P, SCORE_PLAYER_TWO,
                           G_u8SnakeScore % 10);
}

void SCORE_vIncrement(SCORE_Player_t A_xPlayer)
{
    SCORE_Player_t L_xOtherPlayer;
    u8 L_u8Current;
    u8 L_u8Other;

    if (A_xPlayer > SCORE_PLAYER_TWO || G_xS2P == NULL)
        return;

    L_xOtherPlayer = (A_xPlayer == SCORE_PLAYER_ONE) ?
                     SCORE_PLAYER_TWO : SCORE_PLAYER_ONE;
    L_u8Current = G_au8Score[A_xPlayer];
    L_u8Other = G_au8Score[L_xOtherPlayer];

    if (L_u8Current == 9)
    {
        /*
         * A second win at 9 consumes the lower score. This gives the
         * documented example 5 versus 9 -> 0 versus 4.
         */
        G_au8Score[A_xPlayer] = L_u8Current - L_u8Other;
        G_au8Score[L_xOtherPlayer] = 0;
        HSSD_S2P_vAssignNumber(G_xS2P, L_xOtherPlayer, 0);
        HSSD_S2P_vAssignNumber(G_xS2P, A_xPlayer,
                               G_au8Score[A_xPlayer]);
    }
    else
    {
        G_au8Score[A_xPlayer]++;
        HSSD_S2P_vIncrement(G_xS2P, A_xPlayer);
    }
}

u8 SCORE_u8Get(SCORE_Player_t A_xPlayer)
{
    if (A_xPlayer > SCORE_PLAYER_TWO)
        return 0;

    return G_au8Score[A_xPlayer];
}
