#ifndef _FLASH_MONITOR_H
#define _FLASH_MONITOR_H
#include <stdint.h>
#include <stdio.h>

#define KB	                  1024
#define TRANFER_BUFFER_SIZE   (2*KB)
#define FLASH_PAGE_SIZE		    0x400U

// Error number
#define INVALID_ADDRESS	1

typedef enum {
  TARGET_NOT_READY,
  TARGET_READY,
  WRITE_DATA,
  MASS_ERASE,
  PAGE_ERASE,
} commandState;

typedef struct FlashState FlashState;
struct FlashState {
	commandState command;
	uint32_t dataAddr;
	int blockSize;
	uint32_t flashAddr;
	int dataSize;
};

extern uint8_t transferBuffer[];

uint32_t readFlash(uint32_t addr);
void flashCheckState(int data);

#endif // _FLASH_MONITOR_H
