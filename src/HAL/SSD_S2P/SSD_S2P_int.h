#ifndef HAL_SSD_S2P_SSD_S2P_INT_H_
#define HAL_SSD_S2P_SSD_S2P_INT_H_

#include "../../LIB/STD_TYPES.h"
#include "../S2P/S2P_int.h"

void HSSD_S2P_vInit(S2P_PinConfig_t *A_xS2P);
void HSSD_S2P_vAssignNumber(S2P_PinConfig_t *A_xS2P,
                            u8 A_u8Display,
                            u8 A_u8Number);
void HSSD_S2P_vIncrement(S2P_PinConfig_t *A_xS2P, u8 A_u8Display);
void HSSD_S2P_vDecrement(S2P_PinConfig_t *A_xS2P, u8 A_u8Display);

#endif /* HAL_SSD_S2P_SSD_S2P_INT_H_ */
