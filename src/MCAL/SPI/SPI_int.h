/*
 * SPI_int.c
 *
 *  Created on: Sep 2, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_SPI_SPI_INT_C_
#define MCAL_SPI_SPI_INT_C_

typedef struct {

} SPI_PinConfig_t;

void MSPI_vInit(void);
void MSPI_vSendData(u16 A_u16Data);
u16 MSPI_u16Transceive(u16 A_u8Data);
u16 MSPI_u32ReceiveData(void);



#endif /* MCAL_SPI_SPI_INT_C_ */
