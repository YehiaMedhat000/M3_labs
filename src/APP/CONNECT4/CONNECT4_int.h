#ifndef APP_CONNECT4_CONNECT4_INT_H_
#define APP_CONNECT4_CONNECT4_INT_H_

#include "../../LIB/STD_TYPES.h"

/* Board Configuration Constants */
#define CONNECT4_COLS 7
#define CONNECT4_ROWS 6

/* Connect4 Life-Cycle API */
void CONNECT4_vInit(void);
void CONNECT4_vPlay(void);

/* Input callbacks: set game flags; rendering runs in CONNECT4_vPlay(). */
void CONNECT4_vBtnLeft_ISR(void);
void CONNECT4_vBtnDrop_ISR(void);
void CONNECT4_vBtnRight_ISR(void);
void CONNECT4_vBtnExit_ISR(void);
void CONNECT4_vHandleIRCommand(u8 A_u8Command);

#endif /* APP_CONNECT4_CONNECT4_INT_H_ */
