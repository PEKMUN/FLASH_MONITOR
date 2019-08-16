#include <stdint.h>
#include <stdio.h>
#include "unity.h"
#include "CException.h"
#include "CExceptionConfig.h"
#include "Exception.h"
#include "FLASH_MONITOR.h"
#include "mock_Flash.h"

void fake_writeFlash(uint32_t addr, uint32_t data)
{
  int *ptr = (int *)addr;
  *ptr = data;
}

void fake_flashMassErase(uint32_t banks)
{
  uint32_t i;
  int *ptr = (int *)banks;
  for(i=0 ; i<(FLASH_MEM/4) ; i++)
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
}

void tearDown(void)
{
}

void test_flashCheckState_given_0x76008766_expect_a_error_return(void)
{
  CEXCEPTION_T ex;
  uint32_t data;
  
  flashFakeSeq();
  
  Try {
    flashCheckState(WRITE_DATA, 0x76008766, 0x123, 1);
    data = readFlash(0x76008766);
    TEST_FAIL_MESSAGE("Expect an error to be thrown.But none.");
  } Catch(ex) {
		printf("%s", ex->errorMsg);
    TEST_ASSERT_EQUAL(INVALID_ADDRESS, ex->errorCode);
  }
}

void test_flashCheckState_given_0x66fb58_expect_return_correct_data(void)
{
  CEXCEPTION_T ex;
  uint32_t data;
  
  flashFakeSeq();
  
  Try {
    flashCheckState(WRITE_DATA, 0x66fb58, 0x123, 1);
    data = readFlash(0x66fb58);
    TEST_ASSERT_EQUAL(0x123, data);
  } Catch(ex) {
		TEST_FAIL_MESSAGE(ex->errorMsg);
  }
}

void test_flashCheckState_given_0x0_expect_a_error_return(void)
{
  CEXCEPTION_T ex;
  uint32_t data;
  
  flashFakeSeq();
  
  Try {
    flashCheckState(WRITE_DATA, 0x0, 0x123, 1);
    data = readFlash(0x0);
    TEST_FAIL_MESSAGE("Expect an error to be thrown.But none.");
  } Catch(ex) {
		printf("%s", ex->errorMsg);
    TEST_ASSERT_EQUAL(INVALID_ADDRESS, ex->errorCode);
  }
}

void test_flashCheckState_given_command_mass_erase_expect_all_the_data_is_clear_to_0xffffffff(void)
{
  CEXCEPTION_T ex;
  uint32_t data;
  
  flashFakeSeq();
  
  Try {
    flashCheckState(MASS_ERASE, 0, 0, 0);
    TEST_ASSERT_EQUAL(0, readFlash(0x66f854));
    TEST_ASSERT_EQUAL(0, readFlash(0x66fb58));
    TEST_ASSERT_EQUAL(0, readFlash(0x670000));
  } Catch(ex) {
		TEST_FAIL_MESSAGE(ex->errorMsg);
  }
}