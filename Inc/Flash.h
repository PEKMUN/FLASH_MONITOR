#ifndef _FLASH_H
#define _FLASH_H
#include <stdint.h>

void writeFlash(uint32_t addr, uint32_t data);
void flashMassErase();
void flashPageErase(uint32_t pageAddress, int numOfSector);

#endif // _FLASH_H
