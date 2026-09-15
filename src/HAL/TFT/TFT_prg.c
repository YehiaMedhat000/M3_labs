#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"
#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SPI/SPI_int.h"
#include "../../MCAL/TIM/TIM_int.h"
#include "TFT_prv.h"
#include "TFT_int.h"

static const u8 Font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space (32)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0 (48)
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A (65)
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // \
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a (97)
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    {0x08, 0x14, 0x14, 0x14, 0x7C}, // q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s
    {0x04, 0x3E, 0x44, 0x40, 0x20}, // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // z
    {0x08, 0x36, 0x41, 0x41, 0x00}, // {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // |
    {0x00, 0x41, 0x41, 0x36, 0x08}, // }
    {0x08, 0x08, 0x2A, 0x1C, 0x08}  // ~
};

static u16 u16_Xstart;
static u16 u16_Xend;
static u16 u16_Ystart;
static u16 u16_Yend;

GPIOx_PinConfig_t TFT_RESET = {
	.Port = GPIO_PORTA, .Pin = GPIO_PIN4, .Mode = GPIO_MODE_OUTPUT,
	.PullType = GPIO_NO_PULL, .Speed = GPIO_SPEED_HIGH, .OutputType = GPIO_OT_PUSHPULL
};

GPIOx_PinConfig_t TFT_A0 = {
	.Port = GPIO_PORTA, .Pin = GPIO_PIN6, .Mode = GPIO_MODE_OUTPUT,
	.PullType = GPIO_NO_PULL, .Speed = GPIO_SPEED_HIGH, .OutputType = GPIO_OT_PUSHPULL
};

static void Write_cmd(u8 A_u8cmd)
{
	MGPIO_vSetPinValue(TFT_A0.Port, TFT_A0.Pin, 0);
	MSPI_u16Transceive(A_u8cmd);
}

static void Write_data(u8 A_u8data)
{
	MGPIO_vSetPinValue(TFT_A0.Port, TFT_A0.Pin, 1);
	MSPI_u16Transceive(A_u8data);
}

static void HTFT_vReset()
{
	MGPIO_vSetPinValue(TFT_RESET.Port, TFT_RESET.Pin, 1);
	MTIM_vSetDelayUS(100);
	MGPIO_vSetPinValue(TFT_RESET.Port, TFT_RESET.Pin, 0);
	MTIM_vSetDelayUS(1);
	MGPIO_vSetPinValue(TFT_RESET.Port, TFT_RESET.Pin, 1);
	MTIM_vSetDelayUS(100);
	MGPIO_vSetPinValue(TFT_RESET.Port, TFT_RESET.Pin, 0);
	MTIM_vSetDelayUS(100);
	MGPIO_vSetPinValue(TFT_RESET.Port, TFT_RESET.Pin, 1);
	MTIM_vSetDelayMS(120);
}

void HTFT_vInit(void)
{
	MGPIO_vPinInit(&TFT_RESET);
	MGPIO_vPinInit(&TFT_A0);
	MSPI_vInit();

	// Reset
	HTFT_vReset();

	// Sleep Out
	Write_cmd(0x11);

	// Wait 15 msec
	MTIM_vSetDelayMS(15);

	// Select Color Mode
	Write_cmd(0x3A);
	Write_data(0x05);

	// Enable Display
	Write_cmd(0x29);
}

void HTFT_vShowImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize)
{
	volatile u8 MSB = 0;
	volatile u8 LSB = 0;

	Write_cmd(CASET);
	Write_data(0);
	Write_data(0);
	Write_data(0);
	Write_data(127);
	Write_cmd(RASET);
	Write_data(0);
	Write_data(0);
	Write_data(0);
	Write_data(159);

	// Send Image
	Write_cmd(RAMWR);

	for (u16 i = 0; i < A_u16ImgSize; i++)
		{
			MSB = (A_u16ImgArr[i] & 0xff00) >> 8;
			LSB = A_u16ImgArr[i] & 0x00ff;

			Write_data(MSB);
			Write_data(LSB);
		}
}

void HTFT_vSetXPos(u16 A_u16xStart, u16 A_u16xEnd)
{
	u16_Xstart = A_u16xStart;
	u16_Xend = A_u16xEnd;

	Write_cmd(CASET);
	Write_data(0);
	Write_data(A_u16xStart);
	Write_data(0);
	Write_data(A_u16xEnd);
}

void HTFT_vSetYPos(u16 A_u16yStart, u16 A_u16yEnd)
{
	u16_Ystart = A_u16yStart;
	u16_Yend = A_u16yEnd;

	Write_cmd(RASET);
	Write_data(0);
	Write_data(A_u16yStart);
	Write_data(0);
	Write_data(A_u16yEnd);
}

void HTFT_vFillRectangle(u16 A_u16Color)
{

	u16 MSB = 0;
	u16 LSB = 0;
	u16 Pixels = (u16_Xend - u16_Xstart + 1) * (u16_Yend - u16_Ystart + 1);

	Write_cmd(0x2C);

	MSB = (A_u16Color & 0xff00) >> 8;
	LSB = A_u16Color & 0x00ff;

	for (u16 i = 0; i < Pixels; i++)
	{
		Write_data(MSB);
		Write_data(LSB);
	}
}

void HTFT_vFillBackgroundColor(u16 A_u16Color)
{
	HTFT_vSetXPos(0,127);
	HTFT_vSetYPos(0,159);
	HTFT_vFillRectangle(A_u16Color);
}


void HTFT_vDrawVerticalLine(u16 A_u16xStart, u16 A_u16yStart, u16 A_u16yEnd, u16 A_u16Color)
{
	if (A_u16xStart < 0)
		A_u16xStart = 0;

	if (A_u16yStart < 0 || A_u16yEnd > 159 || A_u16yEnd < 1 )
	{
		A_u16yStart = 0;
		A_u16yEnd = 159;
	}

	HTFT_vSetXPos(A_u16xStart, A_u16xStart);
	HTFT_vSetYPos(A_u16yStart, A_u16yEnd);
	HTFT_vFillRectangle(A_u16Color);
}

void HTFT_vDrawHorizontalLine(u16 A_u16yStart, u16 A_u16xStart, u16 A_u16xEnd, u16 A_u16Color)
{
	if (A_u16yStart < 0)
		A_u16yStart = 0;

	if (A_u16xStart < 0 || A_u16xEnd > 127 || A_u16xEnd < 1 )
	{
		A_u16xStart = 0;
		A_u16xEnd = 127;
	}

	HTFT_vSetXPos(A_u16xStart, A_u16xEnd);
	HTFT_vSetYPos(A_u16yStart, A_u16yStart);
	HTFT_vFillRectangle(A_u16Color);
}

void HTFT_vDrawGrid(u16 A_u16xStart, u16 A_u16xEnd, u16 A_u16yStart, u16 A_u16yEnd, u16 A_u16LineColor, u16 A_u16BGColor, u8 A_u8NCols, u8 A_u8NRows)
{
	if (A_u16xStart < 0 || A_u16xEnd > 127 || A_u16xEnd < 1 )
	{
		A_u16xStart = 0;
		A_u16xEnd = 127;
	}

	if (A_u16yStart < 0 || A_u16yEnd > 159 || A_u16yEnd < 1 )
	{
		A_u16yStart = 0;
		A_u16yEnd = 159;
	}

	u8 L_colWidth = (A_u16xEnd - A_u16xStart) / A_u8NCols;
	u8 L_rowHeight = (A_u16yEnd - A_u16yStart) / A_u8NRows;

	HTFT_vSetXPos(A_u16xStart, A_u16xEnd);
	HTFT_vSetYPos(A_u16yStart, A_u16yEnd);
	HTFT_vFillRectangle(A_u16BGColor);

	for (u8 col = 0; col < A_u8NCols + 1; col++)
		HTFT_vDrawVerticalLine(A_u16xStart + (col * L_colWidth), A_u16yStart, A_u16yEnd, A_u16LineColor);

	for (u8 row = 0; row < A_u8NRows + 1; row++)
		HTFT_vDrawHorizontalLine(A_u16yStart + (row * L_rowHeight), A_u16xStart, A_u16xEnd, A_u16LineColor);
}

void HTFT_vDrawChar(u16 A_u16X, u16 A_u16Y, char A_charCh, u16 A_u16Color, u16 A_u16BGColor, u8 A_u8Size)
{
	s8 L_s8Col, L_s8Row;
	u8 L_u8Line;

	if (A_charCh < 32 || A_charCh > 126)
	{
		A_charCh = ' ';
	}

	/* 1-column spacing (rendered first for 180� rotation offset) */
	for (L_s8Row = 0; L_s8Row < 8; L_s8Row++)
	{
		u16 L_u16Px = A_u16X;
		u16 L_u16Py = A_u16Y + (L_s8Row * A_u8Size);

		HTFT_vSetXPos(L_u16Px, L_u16Px + A_u8Size - 1);
		HTFT_vSetYPos(L_u16Py, L_u16Py + A_u8Size - 1);
		HTFT_vFillRectangle(A_u16BGColor);
	}

	/* Traverses columns right-to-left (4 down to 0) */
	for (L_s8Col = 4; L_s8Col >= 0; L_s8Col--)
	{
		L_u8Line = Font5x7[A_charCh - 32][L_s8Col];

		/* Traverses bits bottom-to-top (bit 7 down to bit 0) */
		for (L_s8Row = 7; L_s8Row >= 0; L_s8Row--)
		{
			/* Check bit from most-significant to least-significant */
			u16 L_u16DrawColor = (L_u8Line & (1 << L_s8Row)) ? A_u16Color : A_u16BGColor;

			/* Invert screen coordinate offsets */
			u16 L_u16Px = A_u16X + ((4 - L_s8Col + 1) * A_u8Size);
			u16 L_u16Py = A_u16Y + ((7 - L_s8Row) * A_u8Size);

			HTFT_vSetXPos(L_u16Px, L_u16Px + A_u8Size - 1);
			HTFT_vSetYPos(L_u16Py, L_u16Py + A_u8Size - 1);
			HTFT_vFillRectangle(L_u16DrawColor);
		}
	}
}

void HTFT_vWriteString(u16 A_u16X, u16 A_u16Y, const char *A_pcStr, u16 A_u16Color, u16 A_u16BGColor, u8 A_u8Size)
{
	u16 L_u16StrLen = 0;
	u16 L_u16CurrentX;
	u16 L_u16CurrentY = A_u16Y;
	const char *L_pcPtr = A_pcStr;

	if (A_pcStr == NULL)
		return;

	while (L_pcPtr[L_u16StrLen] != '\0')
		L_u16StrLen++;

	L_u16CurrentX = A_u16X + (L_u16StrLen * 6 * A_u8Size);

	while (*A_pcStr)
	{
		if (*A_pcStr == '\n')
		{
			L_u16CurrentX = A_u16X + (L_u16StrLen * 6 * A_u8Size);
			L_u16CurrentY += (8 * A_u8Size);
		}
		else
		{
			/* Move cursor backward (right to left) for 180-degree string flow */
			L_u16CurrentX -= (6 * A_u8Size);
			HTFT_vDrawChar(L_u16CurrentX, L_u16CurrentY, *A_pcStr, A_u16Color, A_u16BGColor, A_u8Size);
		}
		A_pcStr++;
	}
}
