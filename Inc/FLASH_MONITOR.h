#ifndef _FLASH_MONITOR_H
#define _FLASH_MONITOR_H
#include <stdint.h>

#define KB	1024 

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

#endif // _FLASH_MONITOR_H
