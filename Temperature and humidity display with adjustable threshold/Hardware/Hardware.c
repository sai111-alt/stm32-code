#include "Hardware.h"

uint8_t LedFlashFlag = 0;

void HardWare_Init(void)
{
    OLED_Init();
    W25Q64_Init();
    Key_Init();
    LED_Init();
    DHT22_Init();
    Buzzer_Init();
    LED_Init();
    Timer_Init();
}

void ValueJudgeShow(int8_t *TemHemValue, int8_t *ArrayValue, uint8_t *KeyNum)
{
    if (DHT22_Read_Data(TemHemValue))
    {
        OLED_ShowString(1, 1, "Error");
    }
    else
    {
        OLED_ShowString(1, 1, "Welcome");
    }
    Delay_ms(100); // 等待DHT22在输出完数据后还会输出50us的低电平，必须等待该电平过去，否则会出错

    // 温度小数字节的最高位是温度的符号位，0正1负
    OLED_ShowNum(2, 4, Tem, 2);
    OLED_ShowString(2, 6, ".");
    OLED_ShowNum(2, 7, TemHemValue[3], 1);
    OLED_ShowString(2, 8, "!C");

    OLED_ShowNum(3, 4, Hem, 2);
    OLED_ShowString(3, 6, ".");
    OLED_ShowNum(3, 7, TemHemValue[1], 1);
    OLED_ShowString(3, 8, "%RH");
    if (LedFlashFlag == 0)
    {
        if (*KeyNum == 2)
        {
            LedFlashFlag = !LedFlashFlag;
            ; // 按下按键2则停止鸣叫和闪灯
        }
        else
        {
            W25Q64_ReadData(0X000000, ArrayValue, 4);
            if ((Tem < TLow) || (Tem > THigh))
            {
                Buzzer_Turn();
                LED1_Turn();
                Delay_ms(100);
                Buzzer_Turn();
                LED1_Turn();
                Delay_ms(100);
                Buzzer_Turn();
                LED1_Turn();
                Delay_ms(100);
                Buzzer_Turn();
                LED1_Turn();
                Delay_ms(700);
            }
            if ((Hem < HLow) || (Hem > HHigh))
            {
                Buzzer_Turn();
                LED2_Turn();
                Delay_ms(100);
                Buzzer_Turn();
                LED2_Turn();
                Delay_ms(100);
                Buzzer_Turn();
                LED2_Turn();
                Delay_ms(100);
                Buzzer_Turn();
                LED2_Turn();
                Delay_ms(700);
            }
        }
    }
}

void ValueSet(int8_t *ArrayValue, uint8_t *KeyNum, uint8_t *SetPlace, uint8_t *SetPlaceFlashFlag)
{

    // 阈值显示
    OLED_ShowString(1, 1, "Setting");
    OLED_ShowString(3, 1, "T_:");
    OLED_ShowString(3, 9, "H_:");
    OLED_ShowString(2, 1, "T^:");
    OLED_ShowString(2, 9, "H^:");
    OLED_ShowSignedNum(2, 4, THigh, 2);
    OLED_ShowSignedNum(3, 4, TLow, 2);
    OLED_ShowSignedNum(2, 12, HHigh, 2);
    OLED_ShowSignedNum(3, 12, HLow, 2);
    if (*KeyNum == 2)
    {
        (*SetPlace)++;    // 注意运算顺序
        (*SetPlace) %= 4; // 取值0~3
    }
    if (*KeyNum == 3)
    {
        ArrayValue[*SetPlace]++;
        // 越界判断
        if (THigh > 80) // 温度上阈值越界判断
        {
            THigh = 80;
        }
        if (HHigh > 99) // 湿度上阈值越界判断
        {
            HHigh = 99;
        }
        if (TLow >= THigh) // 温度下阈值++不能超过上阈值
        {
            TLow--;
        }
        if (HLow >= HHigh) // 湿度下阈值++不能超过上阈值
        {
            HLow--;
        }
    }
    if (*KeyNum == 4)
    {
        ArrayValue[*SetPlace]--;
        if (TLow < -40) // 温度下阈值越界判断
        {
            TLow = -40;
        }
        if (HLow < 0) // 湿度下阈值越界判断
        {
            HLow = 0;
        }
        if (THigh <= TLow) // 温度上阈值--不能少于上阈值
        {
            THigh++;
        }
        if (HHigh <= HLow) // 湿度上阈值--不能少于上阈值
        {
            HHigh++;
        }
    }

    // 闪烁显示
    if (*SetPlace == 0 && (*SetPlaceFlashFlag) == 1)
    {
        OLED_ShowString(2, 7, "<");
    }
    else
    {
        OLED_ShowString(2, 7, "  ");
    }
    if (*SetPlace == 1 && (*SetPlaceFlashFlag) == 1)
    {
        OLED_ShowString(3, 7, "<");
    }
    else
    {
        OLED_ShowString(3, 7, "  ");
    }
    if (*SetPlace == 2 && (*SetPlaceFlashFlag) == 1)
    {
        OLED_ShowString(2, 15, "<");
    }
    else
    {
        OLED_ShowString(2, 15, "  ");
    }
    if (*SetPlace == 3 && (*SetPlaceFlashFlag) == 1)
    {
        OLED_ShowString(3, 15, "<");
    }
    else
    {
        OLED_ShowString(3, 15, "  ");
    }
}

/// @brief 将实时的温湿度数值写入存储器的第二个扇区
/// @param TemHemValue 温湿度的数组指针
void DataStorage(int8_t *TemHemValue)
{
    static uint16_t StorageCount = 1024;
    static uint32_t Address = 0x001000;

    // 1个扇区4096B，一次存4个B，这里就存一个扇区，也就是可以存1024次温湿度数据
    if (StorageCount)
    {
        W25Q64_PageProgram(Address, TemHemValue, 4);
        Address += 4;
        StorageCount--;
    }
    else
    {
        Address = 0x001000;
        StorageCount = 1024;
        W25Q64_SectorErase(0x001000);
    }
}
