#include "../../LIB/STD_TYPES.h"
#include "../S2P/S2P_int.h"
#include "SSD_S2P_cfg.h"
#include "SSD_S2P_int.h"

static const u8 G_au8Numbers[10] = {
    0b11000000, /* 0 */
    0b11111001, /* 1 */
    0b10100100, /* 2 */
    0b10110000, /* 3 */
    0b10011001, /* 4 */
    0b10010010, /* 5 */
    0b10000010, /* 6 */
    0b11111000, /* 7 */
    0b10000000, /* 8 */
    0b10010000  /* 9 */
};

static u8 G_u8CurrentNumber[SSD_S2P_DISPLAY_COUNT];

static u8 SSD_S2P_u8IsValidDisplay(u8 A_u8Display)
{
    return A_u8Display < SSD_S2P_DISPLAY_COUNT;
}

static u16 SSD_S2P_u16BuildData(void)
{
    return ((u16)G_au8Numbers[G_u8CurrentNumber[0]] << 8) |
           G_au8Numbers[G_u8CurrentNumber[1]];
}

void HSSD_S2P_vInit(S2P_PinConfig_t *A_xS2P)
{
    u8 L_u8Display;

    HS2P_vInit(A_xS2P);

    for (L_u8Display = 0; L_u8Display < SSD_S2P_DISPLAY_COUNT; L_u8Display++)
        G_u8CurrentNumber[L_u8Display] = 0;

    HS2P_vSendData(A_xS2P, SSD_S2P_u16BuildData());
}

void HSSD_S2P_vAssignNumber(S2P_PinConfig_t *A_xS2P,
                            u8 A_u8Display,
                            u8 A_u8Number)
{
    if (!SSD_S2P_u8IsValidDisplay(A_u8Display) || A_u8Number > 9)
        return;

    G_u8CurrentNumber[A_u8Display] = A_u8Number;
    HS2P_vSendData(A_xS2P, SSD_S2P_u16BuildData());
}

void HSSD_S2P_vIncrement(S2P_PinConfig_t *A_xS2P, u8 A_u8Display)
{
    if (!SSD_S2P_u8IsValidDisplay(A_u8Display))
        return;

    G_u8CurrentNumber[A_u8Display] =
        (G_u8CurrentNumber[A_u8Display] + 1) % 10;
    HSSD_S2P_vAssignNumber(A_xS2P, A_u8Display,
                           G_u8CurrentNumber[A_u8Display]);
}

void HSSD_S2P_vDecrement(S2P_PinConfig_t *A_xS2P, u8 A_u8Display)
{
    if (!SSD_S2P_u8IsValidDisplay(A_u8Display))
        return;

    if (G_u8CurrentNumber[A_u8Display] == 0)
        G_u8CurrentNumber[A_u8Display] = 9;
    else
        G_u8CurrentNumber[A_u8Display]--;

    HSSD_S2P_vAssignNumber(A_xS2P, A_u8Display,
                           G_u8CurrentNumber[A_u8Display]);
}
