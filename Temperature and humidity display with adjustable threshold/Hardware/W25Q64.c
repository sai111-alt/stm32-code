#include "W25Q64.h"

void W25Q64_Init(void)
{
    MySPI_Init();
}

/// @brief 读取并通过指针返回厂商和设备ID
/// @param MID  厂商ID
/// @param DID  设备ID
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
    MySPI_Start();
    MySPI_SwapByte(W25Q64_JEDEC_ID); // 发送获取ID号的指令
    *MID = MySPI_SwapByte(0xFF);     // 接收 Manufacturer ID，对于我们发送的垃圾数据，一般用0xFF，当然你用其他数据也可以
    *DID = MySPI_SwapByte(0xFF);     // 接收 Memory Type
    *DID <<= 8;
    *DID |= MySPI_SwapByte(0xFF); // 接收 Capacity，整个DID就是设备ID，所以用|，不用|，原来的 Memory Type 数据会全部置0
    MySPI_Stop();
}

/// @brief 芯片写使能，在写操作前，必须先写使能，并且写使能仅对写入后的一条时序（即Start到Stop）有效，时序结束后会自动写失能
/// @param
void W25Q64_WriteEnable(void)
{
    MySPI_Start();
    MySPI_SwapByte(W25Q64_WRITE_ENABLE); // 发送写使能指令
    MySPI_Stop();
}

/// @brief 用于查询芯片的忙状态，在进行读写操作时，应先查询芯片是否处于忙状态
/// @param
void W25Q64_WaitBusy(void)
{
    uint32_t Timeout = 100000;
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
    while ((MySPI_SwapByte(0xFF) & 0x01) == 0x01)
    {
        Timeout--; // Timeout的作用就是防止while死循环卡死，一般来说busy的时间不会耗时太久，一旦超时直接跳出即可
        if (Timeout == 0)
        {
            break;
        }
    }
    MySPI_Stop();
}
/// @brief 按页对芯片写入数据
/// @param Address 指定地址写
/// @param DataArray 要发送的数据数组
/// @param Count 表示要写入的字节个数，取值范围1-256,写入数据有页的限制
void W25Q64_PageProgram(uint32_t Address, int8_t *DataArray, uint16_t Count)
{
    uint16_t i = 0;
    W25Q64_WriteEnable(); // 在写操作前，必须先写使能
    MySPI_Start();
    MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
    MySPI_SwapByte(Address >> 16); // 指定的3个字节地址高位先发,比如Address是0x123456，那么这里发送0x12
    MySPI_SwapByte(Address >> 8);  // 比如Address是0x123456，那么这里发送0x34，而0x12的高位自动舍弃，因为交换字节最大8位数据
    MySPI_SwapByte(Address);       // 比如Address是0x123456，那么这里发送0x56，高位数据自动舍弃
    for (i = 0; i < Count; i++)
    {
        MySPI_SwapByte(DataArray[i]);
    }
    MySPI_Stop();
    W25Q64_WaitBusy(); // 事后等待，最保险，当每次结束该后函数后，芯片肯定处于不忙状态
                       // 当然还有事前等待，在函数开始进行等待，这样效率要高一些，因为当函数执行完后，可以去干其他事
                       // 或许当干完其他事后，芯片已经退出了忙状态，但这样写的话，读取数据的函数也需要加事前等待
                       // 因为忙状态也是不能读取的
}

/// @brief 指定地址进行扇区擦除
/// @param Address
void W25Q64_SectorErase(uint32_t Address)
{
    W25Q64_WriteEnable(); // 在写操作前，必须先写使能

    MySPI_Start();
    MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
    MySPI_SwapByte(Address >> 16);
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    MySPI_Stop();
    W25Q64_WaitBusy(); // 事后等待
}

/// @brief 用于读取芯片指定地址及其后面的数据
/// @param Address
/// @param DataArray 用于存储读出的数据
/// @param Count   读取是没有限制的，所有字节均可读取
void W25Q64_ReadData(uint32_t Address, int8_t *DataArray, uint32_t Count)
{
    uint32_t i = 0;
    MySPI_Start();
    MySPI_SwapByte(W25Q64_READ_DATA);
    MySPI_SwapByte(Address >> 16);
    MySPI_SwapByte(Address >> 8);
    MySPI_SwapByte(Address);
    for (i = 0; i < Count; i++)
    {
        DataArray[i] = MySPI_SwapByte(0xFF);
    }
    MySPI_Stop();
}
