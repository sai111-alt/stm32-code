#include "MyDMA.h"

uint16_t MyDMA_Size = 0;

void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
    MyDMA_Size = Size;

    // 开启时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* 注意DMA是AHB总线的外设 */

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;                       /* 存放要转运数据的外设地址 */
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* 选择数据宽度位字节，也就是8位 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;         /* 选择转运外设地址自增 */
    DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;                           /* 存放要转运数据的存储器地址 */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         /* 选择数据宽度位字节，也就是8位 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 /* 选择转运存储器地址自增 */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      /* 选择外设站点作为源头，也就是A为源头 */
    DMA_InitStructure.DMA_BufferSize = Size;                                /* 选择转运传输的次数 */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           /* 选择传输计数器不自动重装 */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;                             /* 选择软件触发方式 */
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   /* 选择中等优先级，如果同时使用多个DMA通道需要注意这个参数 */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                            /* 选择DMA1的通道1 */
    /* 这里是存储器到存储器的数据传输，所以是软件触发，既然是软件的连续触发，就不能选择自动重装，否则DMA会一直死循环疯狂传输 */

    // 关闭（失能）DMA1的通道1，把是否传输的权力交给MyDMA_Transfer函数
    DMA_Cmd(DMA1_Channel1, DISABLE);
}

/// @brief 开启一次DMA的数据传输
void MyDMA_Transfer(void)
{
    /* 注意：每次启动DMA传输，需要先对DMA失能，在给传输计数器赋值，
    最后再给DMA使能，而不能直接给传输计数器赋值，这样是无法启动DMA传输的 */
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, MyDMA_Size);
    DMA_Cmd(DMA1_Channel1, ENABLE);
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET)
        ;                         /* 等待DMA转运完成，即对应标志位置1 */
    DMA_ClearFlag(DMA1_FLAG_TC1); /* 转运完成的标志位需手动清零 */
}
