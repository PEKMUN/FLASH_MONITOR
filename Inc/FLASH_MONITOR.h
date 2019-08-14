#ifndef _FLASH_MONITOR_H
#define _FLASH_MONITOR_H
#include <stdint.h>
#include <stdio.h>

#define KB	1024 

// Error number
#define INVALID_ADDRESS	1

typedef enum {
  TARGET_NOT_READY,
  TARGET_READY,
  WRITE_DATA,
  MASS_ERASE,
  PAGE_ERASE,
} commandState;

typedef struct flashState flashState;
struct flashState {
	commandState command;
	uint32_t dataAddr;
	int blockSize;
	uint32_t flashAddr;
	int dataSize;
};

uint32_t readFlash(uint32_t addr);
void flashCheckState(commandState cmd, uint32_t flashAddr, uint32_t data, int dataSize);

#endif // _FLASH_MONITOR_H
