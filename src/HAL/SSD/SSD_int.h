#ifndef HAL_SSD_SSD_INT_H_
#define HAL_SSD_SSD_INT_H_

// Mapping numbers to the pins of the common anode SSD
#define ZERO   0b11000000
#define ONE    0b11111001
#define TWO    0b10100100
#define THREE  0b10110000
#define FOUR   0b10011001
#define FIVE   0b10010010
#define SIX    0b10000010
#define SEVEN  0b11111000
#define EIGHT  0b10000000
#define NINE   0b10010000

void SSD_hAssignNumber(GPIOx_PinConfig_t *SEG, u8 Num);
void SSD_hIncrement(GPIOx_PinConfig_t *SEG);
void SSD_hDecrement(GPIOx_PinConfig_t *SEG);


#endif /* HAL_SSD_SSD_INT_H_ */
