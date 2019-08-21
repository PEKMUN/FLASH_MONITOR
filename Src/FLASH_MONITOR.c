#include "FLASH_MONITOR.h"
#include "CException.h"
#include "Exception.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>


uint32_t dataAddr = (uint32_t *)malloc(FLASH_MEM);

uint32_t readFlash(uint32_t addr)
{
  uint32_t data;
  int *ptr = (int *)addr;
  data = *ptr;
  return data;
}

void flashCheckState(commandState cmd, uint32_t flashAddr, uint32_t data, int dataSize)
{
  uint32_t i;
	flashState *flash;
  flash->blockSize = FLASH_MEM;
	flash->dataAddr = dataAddr;
	
  while(flash->blockSize == 0 || flash->dataAddr == 0);
  flash->command = TARGET_READY;
	
	switch(cmd)
	{
		case WRITE_DATA:
      if(flashAddr >= &flash && flashAddr <= (&flash + (FLASH_MEM / 4)))
      {
        flash->dataSize = dataSize;
        writeFlash(flashAddr, data);
      }
      else
        throwError(1, "Error: Invalid flash address given by user!");
			break;
      
    case MASS_ERASE:
      flashMassErase(flash->dataAddr);
      break;
	}
}