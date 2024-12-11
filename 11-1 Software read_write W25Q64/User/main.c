#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

uint8_t MID = 0x00;	 // 存厂商ID
uint16_t DID = 0x00; // 存设备ID

uint8_t ArrayWrite[] = {0x01, 0x02, 0x03, 0x04};
uint8_t ArrayWrite[4];

int main(void)
{
	OLED_Init();
	W25Q64_Init();

	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");

	W25Q64_ReadID(&MID, &DID);
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);

	while (1)
	{
	}
} // 所以文件中最后一行必须是空行，否则会报警告: last line of file ends without a newline
