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

// void flashCheckState(void )
void flashCheckState(int data)
{
	volatile int i;
	switch(flashState.command)
	{
	  case TARGET_NOT_READY:
	    flashState.blockSize = TRANFER_BUFFER_SIZE;
	    flashState.dataAddr = (uint32_t)transferBuffer;
	    flashState.command = TARGET_READY;
	    break;

	  case WRITE_DATA:
		if(flashState.flashAddr >= flashState.dataAddr && flashState.flashAddr <= flashState.dataAddr+(TRANFER_BUFFER_SIZE/4))
			for(i=0 ; i<(flashState.dataSize/4) ; i++)
			{
				writeFlash(flashState.flashAddr, data);
			}
		else
			throwError(1, "Error: Invalid flash address given by user!");
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
