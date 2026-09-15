/*
 * SPI_prg.c
 *
 *  Created on: Sep 2, 2026
 *      Author: LENOVO
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "SPI_prv.h"
#include "SPI_int.h"



void MSPI_vInit(void)
{
	// Full Duplex Unidirectional two lines
	CLR_BIT(SPI1->CR1, 15);
	CLR_BIT(SPI1->CR1, DFF);

	// Slave Management
	SET_BIT(SPI1->CR1, SSM);
	SET_BIT(SPI1->CR1, SSI);

	// Endianess (MSB)
	CLR_BIT(SPI1->CR1, LSBFIRST);

	// Clock polarity (HIGH on IDLE)
	CLR_BIT(SPI1->CR1, CPOL);
	// Clock phase (In phase with first capture)
	CLR_BIT(SPI1->CR1, CPHA);

	// Configure to be master or slave
	SET_BIT(SPI1->CR1, MSTR);

	// Enable SPI
	SET_BIT(SPI1->CR1, SPE);
}

u16 MSPI_u16Transceive(u16 A_u8Data)
{
	while (!GET_BIT(SPI1->SR, TXE)){ asm("NOP"); }
	SPI1->DR = A_u8Data;

	while (!GET_BIT(SPI1->SR, RXNE)){ asm("NOP"); }
	return SPI1->DR;
}
