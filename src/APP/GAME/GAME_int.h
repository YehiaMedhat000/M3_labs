#ifndef APP_GAME_GAME_INT_H_
#define APP_GAME_GAME_INT_H_

typedef enum {
    GAME_CONNECT_FOUR = 0,
    GAME_TIC_TAC_TOE,
    GAME_SNAKE,
    MENU_TOTAL_ITEMS
} Menu_Item_t;

typedef enum {
    STATE_MENU = 0,
    STATE_CONNECT_FOUR,
    STATE_TIC_TAC_TOE,
    STATE_SNAKE,
    STATE_MODE_MENU
} AppState_t;

typedef enum {
    GAME_MODE_OFFLINE = 0,
    GAME_MODE_ONLINE
} GameMode_t;

void GAME_vDrawMenu(void);

/* Main Menu Task Handler (Call inside main's while(1) loop) */
void GAME_vTaskHandler(void);
void GAME_vHandleIRCommand(u8 A_u8Command);

#endif /* APP_GAME_GAME_INT_H_ */
