/*
 * FLASH_prv.h
 *
 *  Created on: Sep 9, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_FLASH_FLASH_PRV_H_
#define MCAL_FLASH_FLASH_PRV_H_

#define FLASH_BASE_ADDR 0x40022000U

typedef struct {
	u32 ACR;
	u32 KEYR;
	u32 OPTKEYR;
	u32 SR;
	u32 CR;
	u32 AR;
	u32 Res;
	u32 OBR;
	u32 WRPR;
} FLASH_MemMap_t;


#define FLASH ((volatile FLASH_MemMap_t*)(FLASH_BASE_ADDR))

// Bits
#define BSY 	0
#define PG 		0
#define PER 	1
#define PGERR 	2
#define	MER 	2
#define OPTER 	5
#define EOP 	5
#define STRT 	6
#define LOCK 	7
#define OPTWRE 	9

#define KEY1 0x45670123
#define KEY2 0xCDEF89AB

#endif /* MCAL_FLASH_FLASH_PRV_H_ */
