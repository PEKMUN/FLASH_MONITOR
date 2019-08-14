#include "FLASH_MONITOR.h"
#include "CException.h"
#include "Exception.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

uint32_t readFlash(uint32_t addr)
{
  uint32_t data;
  int *ptr = (int *)addr;
  data = *ptr;
  return data;
}

void flashCheckState(commandState cmd, uint32_t flashAddr, uint32_t data, int dataSize)
{
	flashState *flash;
	flash = (flashState *)malloc(2*KB);
  flash->blockSize = 2*KB;
	flash->dataAddr = &flash;
	
  while(flash->blockSize == 0 || flash->dataAddr == 0);
  flash->command = TARGET_READY;
	
	switch(cmd)
	{
		case WRITE_DATA:
      if(flashAddr >= &flash && flashAddr <= (&flash + (flash->blockSize / 4)))
      {
        flash->dataSize = dataSize;
        writeFlash(flashAddr, data);
      }
      else
        throwError(1, "Error: Invalid flash address given by user!");
			break;
	}
}