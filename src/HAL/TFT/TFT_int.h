#ifndef HAL_TFT_TFT_INT_H_
#define HAL_TFT_TFT_INT_H_

#define TFT_BLACK       0x0000
#define TFT_WHITE       0xFFFF

#define TFT_RED         0xF800
#define TFT_GREEN       0x07E0
#define TFT_BLUE        0x001F

#define TFT_YELLOW      0xFFE0
#define TFT_CYAN        0x07FF
#define TFT_MAGENTA     0xF81F

#define TFT_ORANGE      0xFD20
#define TFT_PURPLE      0x780F
#define TFT_PINK        0xF81F

#define TFT_LIGHT_GRAY  0xC618
#define TFT_GRAY        0x8410
#define TFT_DARK_GRAY   0x4208

#define TFT_BROWN       0xA145
#define TFT_LIME        0x07E0

#define CASET 0x2A
#define RASET 0x2B
#define RAMWR 0x2C

void HTFT_vInit(void);
void HTFT_vShowImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize);
void HTFT_vSetXPos(u16 A_u16xStart, u16 A_u16xEnd);
void HTFT_vSetYPos(u16 A_u16yStart, u16 A_u16yEnd);
void HTFT_vFillBackgroundColor(u16 A_u16Color);
void HTFT_vFillRectangle(u16 A_u16Color);

void HTFT_vDrawVerticalLine(u16 A_u16xStart, u16 A_u16yStart, u16 A_u16yEnd, u16 A_u16Color);
void HTFT_vDrawHorizontalLine(u16 A_u16yStart, u16 A_u16xStart, u16 A_u16xEnd, u16 A_u16Color);
void HTFT_vDrawGrid(u16 A_u16xStart, u16 A_u16xEnd, u16 A_u16yStart, u16 A_u16yEnd, u16 A_u16LineColor, u16 A_u16BGColor, u8 A_u8NCols, u8 A_u8NRows);

void HTFT_vDrawChar(u16 A_u16X, u16 A_u16Y, char A_charCh, u16 A_u16Color, u16 A_u16BgColor, u8 A_u8Size);
void HTFT_vWriteString(u16 A_u16X, u16 A_u16Y, const char *A_pcStr, u16 A_u16Color, u16 A_u16BGColor, u8 A_u8Size);

#endif /* HAL_TFT_TFT_INT_H_ */
