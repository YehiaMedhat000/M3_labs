/*
 * EFIO_prg.c
 *
 *  Created on: Aug 24, 2026
 *      Author: LENOVO
 */
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"
#include "../AFIO/AFIO_int.h"
#include "../AFIO/AFIO_prv.h"

void MAFIO_vSetLinePort(u8 A_u8LineNo, u8 A_u8PortNo)
{
	AFIO->EXTICR[A_u8LineNo / 4] &=  ~(0b1111 << ((A_u8LineNo % 4) * 4));
	AFIO->EXTICR[A_u8LineNo / 4] |=  A_u8PortNo << ((A_u8LineNo % 4) * 4);
}

