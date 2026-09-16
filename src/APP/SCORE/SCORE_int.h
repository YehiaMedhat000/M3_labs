#ifndef APP_SCORE_SCORE_INT_H_
#define APP_SCORE_SCORE_INT_H_

#include "../../LIB/STD_TYPES.h"
#include "../../HAL/S2P/S2P_int.h"

typedef enum
{
    SCORE_PLAYER_ONE = 0,
    SCORE_PLAYER_TWO
} SCORE_Player_t;

void SCORE_vInit(S2P_PinConfig_t *A_xS2P);
void SCORE_vReset(void);
void SCORE_vIncrement(SCORE_Player_t A_xPlayer);
void SCORE_vResetSnake(void);
void SCORE_vIncrementSnakeItem(void);
u8 SCORE_u8Get(SCORE_Player_t A_xPlayer);

#endif /* APP_SCORE_SCORE_INT_H_ */
