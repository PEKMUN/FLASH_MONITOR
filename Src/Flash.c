#include "Flash.h"
#include "stm32f1xx_hal.h"

// Remember to unlock first before issuing any of the flash related functions.
// HAL_FLASH_Unlock
// HAL_FLASH_Lock

// Mass erase ==> HAL_FLASHEx_Erase()
void flashMassErase()
{
	uint32_t PageError;
	HAL_FLASH_Unlock();
	HAL_FLASHEx_Erase(FLASH_TYPEERASE_MASSERASE, &PageError);
	HAL_FLASH_Lock();
}

// Page erase ==> FLASH_PageErase(uint32_t PageAddress)
void flashPageErase(uint32_t pageAddress, int numOfSector)
{
	HAL_FLASH_Unlock();
	for(int i=0 ; i<numOfSector ; i++)
	{
		FLASH_PageErase(pageAddress);
		pageAddress+=FLASH_PAGE_SIZE;
	}
	HAL_FLASH_Lock();
}

// program flash ==> HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
// TypeProgram:
//   FLASH_TYPEPROGRAM_HALFWORD(2 bytes)
//   FLASH_TYPEPROGRAM_WORD(4 bytes)
//   FLASH_TYPEPROGRAM_DOUBLEWORD(8 bytes)
void writeFlash(uint32_t addr, uint32_t data)
{
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, data);
	HAL_FLASH_Lock();
}
