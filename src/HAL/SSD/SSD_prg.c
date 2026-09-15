/*
 * SSD_prg.c
 *
 *  Created on: Aug 21, 2026
 *      Author: LENOVO
 */
#include "../../LIB/BIT_MATH.h"
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "SSD_int.h"

u8 Numbers[10] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
u8 G_current = 0;

void SSD_hAssignNumber(GPIOx_PinConfig_t *SEG, u8 Num)
{
	/** TODO: Add a check if the array SEG contains exactly 7 structs
	 * Upon which it raises and error associated to some led or whatever
	 */
	G_current = Num;

	for (u8 i = 0; i < 7; i++)
		MGPIO_vSetPinValue(SEG[i].Port, SEG[i].Pin, GET_BIT(Num, i));

}

void SSD_hIncrement(GPIOx_PinConfig_t *SEG)
{
	/** TODO: Add a check if the array SEG contains exactly 7 structs
	 * Upon which it raises and error associated to some led or whatever
	 */

	if (G_current == NINE)
	{
		G_current = ZERO;
		SSD_hAssignNumber(SEG, G_current);
	}
	else
	{
		for (u8 i = 0; i < 10; i++)
		{
			if (G_current == Numbers[i])
			{
				G_current = Numbers[i + 1];
				break;
			}
		}
		SSD_hAssignNumber(SEG, G_current);
	}
}

void SSD_hDecrement(GPIOx_PinConfig_t *SEG)
{
	/** TODO: Add a check if the array SEG contains exactly 7 structs
	 * Upon which it raises and error associated to some led or whatever
	 */

	if (G_current == ZERO)
	{
		G_current = NINE;
		SSD_hAssignNumber(SEG, G_current);
	}

	else
	{
		for (u8 i = 0; i < 10; i++)
		{
			if (G_current == Numbers[i])
			{
				G_current = Numbers[i - 1];
				break;
			}
		}
		SSD_hAssignNumber(SEG, G_current);
	}
}
