#ifndef _FLASH_H
#define _FLASH_H
#include <stdint.h>

void writeFlash(uint32_t addr, uint32_t data);
void flashMassErase(uint32_t banks);

#endif // _FLASH_H
