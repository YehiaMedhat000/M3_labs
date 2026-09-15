/*
 * FLASH_int.h
 *
 *  Created on: Sep 9, 2026
 *      Author: LENOVO
 */

#ifndef MCAL_FLASH_FLASH_INT_H_
#define MCAL_FLASH_FLASH_INT_H_

void MFLASH_vMassErase();
void MFLASH_vPageErase(u32 A_u8PageAddr);
void MFLASH_vWriteProgram(u32 A_u32PageAddr, u16 *A_u16Data, u8 A_u8DataLength);


#endif /* MCAL_FLASH_FLASH_INT_H_ */
