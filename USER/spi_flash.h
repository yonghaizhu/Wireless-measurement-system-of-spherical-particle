#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"

#define SPI_FLASH_SPI                           SPI1
#define SPI_FLASH_SPI_CLK                       RCC_APB2Periph_SPI1
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_5                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_6                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_7                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOA
#define SPI_FLASH_CS_PIN                        GPIO_Pin_4                  /* PA.04 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOA                       /* GPIOA */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define SPI_FLASH_RESET_PIN                     GPIO_Pin_3                  /* PA.03 */
#define SPI_FLASH_RESET_GPIO_PORT               GPIOA                       /* GPIOA */
#define SPI_FLASH_RESET_GPIO_CLK                RCC_APB2Periph_GPIOA


#define SPI_FLASH_CS_LOW()                      GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FLASH_CS_HIGH()                     GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define SPI_FLASH_RESET_LOW()                   GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define SPI_FLASH_RESET_HIGH()                  GPIO_SetBits(GPIOA, GPIO_Pin_3)

#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			        0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			        0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0xFF


void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32  SPI_FLASH_ReadID(void);
u32  SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_FlASH_PowerDown(void);
void SPI_FlASH_WAKEUP(void);

u8   SPI_FLASH_ReadByte(void);
u8   SPI_FLASH_SendByte(u8 byte);
u16  SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* __SPI_FLASH_H */

