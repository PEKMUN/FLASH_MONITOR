#include "FLASH_MONITOR.h"
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

void flashCheckState(commadState cmd, uint32_t flashAddr, int dataSize)
{
	flashState *flash;
	flash->blockSize = 2 * KB;
	flash = (flashState *)malloc(flash->blockSize);
	flash->dataAddr = flash;
	
	flash->command = TARGET_READY;
	
	switch(cmd)
	{
		case WRITE_DATA:
			
			break;
	}
}