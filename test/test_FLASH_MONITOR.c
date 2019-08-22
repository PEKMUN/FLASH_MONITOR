#include <stdint.h>
#include <stdio.h>
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
  int *ptr = (uint32_t)transferBuffer;
  //volatile int a = TRANFER_BUFFER_SIZE/4;
  for(i=0 ; i<TRANFER_BUFFER_SIZE/4 ; i++)
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
  CEXCEPTION_T ex;
  uint32_t data;
  
  flashFakeSeq();
  flashState.command = TARGET_NOT_READY;
  
  Try {
    flashCheckState(0);
    TEST_ASSERT_EQUAL(2*1024, flashState.blockSize);
    TEST_ASSERT_EQUAL((uint32_t)transferBuffer, flashState.dataAddr);
  } Catch(ex) {
		TEST_FAIL_MESSAGE(ex->errorMsg);
  }
}

void test_flashCheckState_given_flashAddr_0x76008766_expect_a_error_return(void)
{
  CEXCEPTION_T ex;
  uint32_t data;

  flashFakeSeq();
  flashState.command = WRITE_DATA;
  flashState.flashAddr = 0x76008766;

  Try {
    flashCheckState(0x123);
    TEST_FAIL_MESSAGE("Expect an error to be thrown.But none.");
  } Catch(ex) {
		printf("%s", ex->errorMsg);
    TEST_ASSERT_EQUAL(INVALID_ADDRESS, ex->errorCode);
  }
}

void test_flashCheckState_given_flashAddr_is_less_than_transferBuffer_expect_a_error_return(void)
{
  CEXCEPTION_T ex;
  uint32_t data;

  flashFakeSeq();
  flashState.command = WRITE_DATA;
  flashState.flashAddr = (uint32_t)transferBuffer - 0x200;

  Try {
    flashCheckState(0x123);
    TEST_FAIL_MESSAGE("Expect an error to be thrown.But none.");
  } Catch(ex) {
		printf("%s", ex->errorMsg);
    TEST_ASSERT_EQUAL(INVALID_ADDRESS, ex->errorCode);
  }
}

void test_flashCheckState_given_WRITE_DATA_expect_read_the_data_written_is_equal_to_0x51842(void)
{
  CEXCEPTION_T ex;
  uint32_t data;
  
  flashFakeSeq();
  flashState.command = WRITE_DATA;
  flashState.flashAddr = (uint32_t)transferBuffer + 0x18;
  flashState.dataSize = 4;
  
  Try {
    flashCheckState(0x51842);
    TEST_ASSERT_EQUAL(0x51842, readFlash((uint32_t)transferBuffer + 0x18));
  } Catch(ex) {
		TEST_FAIL_MESSAGE(ex->errorMsg);
  }
}

void test_flashCheckState_given_MASS_ERASE_expect_all_the_data_is_clear_to_0xffffffff(void)
{
  CEXCEPTION_T ex;
  uint32_t data;
  
  flashFakeSeq();
  flashState.command = MASS_ERASE;
  flashState.flashAddr = (uint32_t)transferBuffer;
  
  Try {
    flashCheckState(0);
    TEST_ASSERT_EQUAL(0xffffffff, readFlash((uint32_t)transferBuffer));
    TEST_ASSERT_EQUAL(0xffffffff, readFlash((uint32_t)transferBuffer + 0x40));
    TEST_ASSERT_EQUAL(0xffffffff, readFlash((uint32_t)transferBuffer + 0x5c));
  } Catch(ex) {
		TEST_FAIL_MESSAGE(ex->errorMsg);
  }
}

