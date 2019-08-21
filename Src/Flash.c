#include "Flash.h"
#include "stm32f1xx_hal.h"

// Remember to unlock first before issuing any of the flash related functions.
// HAL_FLASH_Unlock
// HAL_FLASH_Lock

// Mass erase ==> HAL_FLASHEx_Erase

// Page erase ==> FLASH_PageErase

// program flash ==> HAL_FLASH_Program
// TypeProgram:
//   FLASH_TYPEPROGRAM_HALFWORD
//   FLASH_TYPEPROGRAM_WORD
//   FLASH_TYPEPROGRAM_DOUBLEWORD
