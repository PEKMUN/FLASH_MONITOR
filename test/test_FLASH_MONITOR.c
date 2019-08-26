#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "unity.h"
#include "CException.h"
#include "CExceptionConfig.h"
#include "Exception.h"
#include "FLASH_MONITOR.h"
#include "mock_Flash.h"

FlashState flashState;

void fake_writeFlash(uint32_t addr, uint32_t data)
{
  int *ptr = (int *)addr;
  *ptr = data;
}

void fake_flashMassErase()
{
  uint32_t i;
  int *ptr = flashState.flashAddr;
  //volatile int a = TRANFER_BUFFER_SIZE/4;
  for(i=0 ; i<TOTAL_FLASH_SIZE/4 ; i++)
  {
    *ptr = 0xffffffff;
    ptr++;
  }
}

void fake_flashPageErase(uint32_t pageAddress)
{
  uint32_t i;
  int *ptr = pageAddress;
  volatile int a = FLASH_PAGE_SIZE/4;
  for(i=0 ; i<a ; i++)
  {
    *ptr = 0xffffffff;
    ptr++;
  }
}

void setUp(void)
{
}

void flashFakeSeq(void)
{
  writeFlash_StubWithCallback(fake_writeFlash);
  flashMassErase_StubWithCallback(fake_flashMassErase);
  flashPageErase_StubWithCallback(fake_flashPageErase);
}

void tearDown(void)
{
}

void test_flashCheckState_given_TARGET_NOT_READY_expect_blockSize_and_dataAddr_is_written(void)
{
  uint32_t data;
  
  flashFakeSeq();
  flashState.command = TARGET_NOT_READY;
  
  flashCheckState();
  TEST_ASSERT_EQUAL(TRANFER_BUFFER_SIZE, flashState.blockSize);
  TEST_ASSERT_EQUAL((uint32_t)transferBuffer, flashState.dataAddr);
}

void fillMemWithRandomNumbers(uint8_t *memAddr, int lenInBytes)
{
  time_t t;
  
  srand((unsigned) time(&t));
  for(int i=0 ; i<lenInBytes ; i++)
  {
    memAddr[i] = rand() & 0xff;
  }
}

void test_flashCheckState_given_dataSize_2KB_WRITE_DATA_expect_all_data_in_dataAddr_and_flashAddr_is_equal(void)
{
  uint32_t data;
  
  flashFakeSeq();
  flashState.command = WRITE_DATA;
  flashState.flashAddr = (uint32_t)malloc(TOTAL_FLASH_SIZE);
  flashState.dataSize = 2*KB;
  
  fillMemWithRandomNumbers(flashState.dataAddr, flashState.dataSize);
  
  flashCheckState();
  TEST_ASSERT_EQUAL_HEX32_ARRAY(flashState.dataAddr, flashState.flashAddr, flashState.dataSize/sizeof(uint32_t));
}

void test_flashCheckState_given_MASS_ERASE_expect_all_the_data_is_clear_to_0xffffffff(void)
{
  uint8_t *dst = (uint8_t *)flashState.flashAddr;
  uint32_t data;
  
  flashFakeSeq();
  flashState.command = MASS_ERASE;
  flashState.flashAddr = (uint32_t)malloc(TOTAL_FLASH_SIZE);
  
  flashCheckState();
  for(int i=0 ; i<TOTAL_FLASH_SIZE ; i++)
    TEST_ASSERT_EQUAL_HEX8(0xff, *(dst++));
}

