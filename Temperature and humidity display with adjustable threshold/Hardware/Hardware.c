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
    MyRTC_Init();
}

void ValueJudgeShow(int8_t *TemHemValue, int8_t *ArrayValue, uint8_t *KeyNum)
{
    MyRTC_ReadTime();               // RTC读取时间，最新的时间存储到MyRTC_Time数组中
    OLED_ShowNum(4, 1, Year, 4);    // 显示MyRTC_Time数组中的时间值，年
    OLED_ShowNum(4, 6, Month, 2);   // 月
    OLED_ShowNum(4, 9, Day, 2);     // 日
    OLED_ShowNum(4, 12, Hour, 2);   // 时
    OLED_ShowNum(4, 15, Minute, 2); // 分

    // 读取温湿度数据
    if (DHT22_Read_Data(TemHemValue))
    {
        OLED_ShowString(1, 1, "Error");
    }
    else
    {
        OLED_ShowString(1, 1, "Welcome");
    }
    Delay_ms(100); // 等待DHT22在输出完数据后还会输出50us的低电平，必须等待该电平过去，否则会出错

    // DHT22：温度整数字节的最高位是温度的符号位，0正1负
    // DHT11：温度小数字节的最高位是温度的符号位，0正1负
    // 测试新到的DHT22再加入温度的负值判断
    OLED_ShowNum(2, 3, Tem, 2);
    OLED_ShowString(2, 5, ".");
    OLED_ShowNum(2, 6, TemHemValue[3], 1);
    OLED_ShowString(2, 7, "!C");

    OLED_ShowNum(3, 3, Hem, 2);
    OLED_ShowString(3, 5, ".");
    OLED_ShowNum(3, 6, TemHemValue[1], 1);
    OLED_ShowString(3, 7, "%RH");
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

void ValueSet(int8_t *ArrayValue, uint8_t *KeyNum, uint8_t *SetPlace, uint8_t *SetPlaceFlashFlag, uint8_t *KeyFlag)
{
    // 阈值显示
    OLED_ShowString(1, 1, "Setting");
    OLED_ShowString(3, 1, "(_:");
    OLED_ShowString(3, 9, ")_:");
    OLED_ShowString(2, 1, "(^:");
    OLED_ShowString(2, 9, ")^:");
    OLED_ShowSignedNum(2, 4, THigh, 2);
    OLED_ShowSignedNum(3, 4, TLow, 2);
    OLED_ShowSignedNum(2, 12, HHigh, 2);
    OLED_ShowSignedNum(3, 12, HLow, 2);
    if ((*KeyNum) == 2)
    {
        (*SetPlace)++;    // 注意运算顺序
        (*SetPlace) %= 4; // 取值0~3
    }
    if ((*KeyNum) == 3 || ((Key3 == 0) && (*KeyFlag)))
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
    if ((*KeyNum) == 4 || ((Key4 == 0) && (*KeyFlag)))
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
    if ((*SetPlace) == 0 && (*SetPlaceFlashFlag) == 1)
    {
        OLED_ShowString(2, 7, "<");
    }
    else
    {
        OLED_ShowString(2, 7, "  ");
    }
    if ((*SetPlace) == 1 && (*SetPlaceFlashFlag) == 1)
    {
        OLED_ShowString(3, 7, "<");
    }
    else
    {
        OLED_ShowString(3, 7, "  ");
    }
    if ((*SetPlace) == 2 && (*SetPlaceFlashFlag) == 1)
    {
        OLED_ShowString(2, 15, "<");
    }
    else
    {
        OLED_ShowString(2, 15, "  ");
    }
    if ((*SetPlace) == 3 && (*SetPlaceFlashFlag) == 1)
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
void DataStorage(int8_t *TemHemValue, uint8_t *DataFlag)
{
    static uint16_t StorageCount = 1024;
    static uint32_t DataAddress = 0x001000;

    if (*DataFlag)
    {
        // 第二个扇区：1个扇区4096B，一次存4个B（温度整数，湿度整数，小时，分钟），这里就存一个扇区，也就是可以存1024次温湿度数据
        // 每42分存一次，共1024/42=24,也即第二个扇区只存一天的实时数据
        if (StorageCount)
        {
            W25Q64_PageProgram(DataAddress, TemHemValue + 2, 1);
            DataAddress += 1;
            StorageCount--;
            W25Q64_PageProgram(DataAddress, TemHemValue, 1);
            DataAddress += 1;
            StorageCount--;
            W25Q64_PageProgram(DataAddress, (int8_t *)(MyRTC_Time + 3), 1);
            DataAddress += 1;
            StorageCount--;
            W25Q64_PageProgram(DataAddress, (int8_t *)(MyRTC_Time + 4), 1);
            DataAddress += 1;
            StorageCount--;
        }
        else
        {
            DataAddress = 0x001000;
            StorageCount = 1024;
            W25Q64_SectorErase(0x001000);
        }
    }
}

/// @brief 将存储好的数据显示出来
/// @param
void DataStorageShow(uint8_t *KeyNum, uint8_t *KeyFlag, uint16_t *DataPage, uint32_t *DataAddress, uint8_t *flag)
{
    int8_t i = 0;
    int8_t TH[6] = {0}; // 0温度，1湿度，2小时，3分钟，4月份，5日期

    // 数据显示
    if ((*flag) == 0)
    {
        if ((*DataPage) >= 341)
        {
            (*DataPage) = 0;
            (*DataAddress) = 0x001000;
        }
        else
        {
            for (i = 2; i < 5; i++)
            {
                // 显示时间与对应温湿度
                W25Q64_ReadData((*DataAddress), TH, 4);
                if ((TH[0] == -1) || (TH[1] == -1)) // 0xFF是空白数据，这里用的有符号存储，所以应该是== -1
                {
                    OLED_ShowString(i, 1, "--");
                    OLED_ShowString(i, 3, ":");
                    OLED_ShowString(i, 4, "--");
                    OLED_ShowString(i, 6, "--");
                    OLED_ShowString(i, 8, "(");
                    OLED_ShowString(i, 9, "--");
                    OLED_ShowString(i, 11, "!");
                    OLED_ShowString(i, 13, ")");
                    OLED_ShowString(i, 14, "--");
                    OLED_ShowString(i, 16, "%");
                }
                else
                {
                    OLED_ShowNum(i, 1, TH[2], 2);
                    OLED_ShowString(i, 3, ":");
                    OLED_ShowNum(i, 4, TH[3], 2);
                    OLED_ShowString(i, 6, "--");
                    OLED_ShowString(i, 8, "(");
                    OLED_ShowNum(i, 9, TH[0], 2);
                    OLED_ShowString(i, 11, "!");
                    OLED_ShowString(i, 13, ")");
                    OLED_ShowNum(i, 14, TH[1], 2);
                    OLED_ShowString(i, 16, "%");
                }
                (*DataAddress) += 4;
            }
            (*flag) = 1;
            (*DataPage)++;
        }
    }

    // 页数显示
    OLED_ShowNum(1, 10, (*DataPage), 3);

    //  向下翻页
    if ((*KeyNum) == 3 || ((Key3 == 0) && (*KeyFlag)))
    {
        (*flag) = 0;
    }
    if ((*KeyNum) == 4 || ((Key4 == 0) && (*KeyFlag)))
    {
        (*flag) = 0;
        if ((*DataPage) == 1)
        {
            (*DataPage) = 340;
            (*DataAddress) = 0x001FF0;
        }
        else
        {
            (*DataPage) -= 2;
            (*DataAddress) -= 24;
        }
    }

    // 复位，回到第一页
    if ((*KeyNum) == 2)
    {
        (*flag) = 0;
        (*DataAddress) = 0x001000;
        (*DataPage) = 0;
    }
}
