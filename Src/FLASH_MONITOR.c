#include "FLASH_MONITOR.h"
#include "CException.h"
#include "Exception.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

uint8_t transferBuffer[TRANFER_BUFFER_SIZE];
__attribute__((section(".monitor_data"))) volatile FlashState flashState = {
  TARGET_NOT_READY, 0x0, 0, 0, 0
};


uint32_t readFlash(uint32_t addr)
{
  uint32_t data;
  int *ptr = (int *)addr;
  data = *ptr;
  return data;
}

// void runMonitor(void )
void flashCheckState(void)
{
	volatile int i;
  uint32_t *src = (uint32_t *)flashState.dataAddr;
  uint32_t *dst = (uint32_t *)flashState.flashAddr;
	switch(flashState.command)
	{
	  case TARGET_NOT_READY:
	    flashState.blockSize = TRANFER_BUFFER_SIZE;
	    flashState.dataAddr = (uint32_t)transferBuffer;
	    flashState.command = TARGET_READY;
	    break;

    case WRITE_DATA:
      for(i=0 ; i<(flashState.dataSize/4) ; i++)
      {
        writeFlash(dst, *src);
        dst++;
        src++;
      }
      flashState.command = TARGET_READY;          // Don't touch this
      break;

	  case MASS_ERASE:
		flashMassErase();
		flashState.command = TARGET_READY;          // Don't touch this
		break;

	  case TARGET_READY:
		  break;

	  default:
		  break;
	}
}
