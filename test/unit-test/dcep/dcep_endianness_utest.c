/* Unity includes. */
#include "unity.h"

/* Standard includes. */
#include <string.h>
#include <stdint.h>

/* API includes. */
#include "dcep_endianness.h"

/* ===========================  EXTERN VARIABLES  =========================== */

DcepReadWriteFunctions_t readWriteFunctions;
uint8_t testBuffer[16];

/* ==============================  Test Cases for Endianness Functions ============================== */

/**
 * @brief Validate Dcep_InitReadWriteFunctions initializes function pointers.
 */
void test_Dcep_InitReadWriteFunctions(void)
{
    memset(&readWriteFunctions, 0, sizeof(readWriteFunctions));
    memset(testBuffer, 0, sizeof(testBuffer));
    
    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    TEST_ASSERT_NOT_NULL(readWriteFunctions.writeUint16Fn);
    TEST_ASSERT_NOT_NULL(readWriteFunctions.writeUint32Fn); 
    TEST_ASSERT_NOT_NULL(readWriteFunctions.readUint16Fn);
    TEST_ASSERT_NOT_NULL(readWriteFunctions.readUint32Fn);
}

/**
 * @brief Validate 16-bit read/write functions work correctly.
 */
void test_Dcep_ReadWrite_Uint16(void)
{
    const uint16_t testValue = 0x1234;
    uint16_t readValue;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Write the value to buffer. */
    readWriteFunctions.writeUint16Fn(testBuffer, testValue);

    /* Read the value back from buffer. */
    readValue = readWriteFunctions.readUint16Fn(testBuffer);

    /* Value should be identical after round-trip. */
    TEST_ASSERT_EQUAL_UINT16(testValue, readValue);
}

/**
 * @brief Validate 32-bit read/write functions work correctly.
 */
void test_Dcep_ReadWrite_Uint32(void)
{
    const uint32_t testValue = 0x12345678;
    uint32_t readValue;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Write the value to buffer. */
    readWriteFunctions.writeUint32Fn(testBuffer, testValue);

    /* Read the value back from buffer. */
    readValue = readWriteFunctions.readUint32Fn(testBuffer);

    /* Value should be identical after round-trip. */
    TEST_ASSERT_EQUAL_UINT32(testValue, readValue);
}

/**
 * @brief Validate network byte order for 16-bit values.
 */
void test_Dcep_NetworkByteOrder_Uint16(void)
{
    const uint16_t testValue = 0x1234;
    
    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Write the value to buffer. */
    readWriteFunctions.writeUint16Fn(testBuffer, testValue);

    /* Check that it's in network byte order (big-endian). */
    /* First byte should be 0x12, second byte should be 0x34. */
    TEST_ASSERT_EQUAL_UINT8(0x12, testBuffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0x34, testBuffer[1]);
}

/**
 * @brief Validate network byte order for 32-bit values.
 */
void test_Dcep_NetworkByteOrder_Uint32(void)
{
    const uint32_t testValue = 0x12345678;
    
    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Write the value to buffer. */
    readWriteFunctions.writeUint32Fn(testBuffer, testValue);

    /* Check that it's in network byte order (big-endian). */
    /* Bytes should be 0x12, 0x34, 0x56, 0x78 in that order. */
    TEST_ASSERT_EQUAL_UINT8(0x12, testBuffer[0]);
    TEST_ASSERT_EQUAL_UINT8(0x34, testBuffer[1]);
    TEST_ASSERT_EQUAL_UINT8(0x56, testBuffer[2]);
    TEST_ASSERT_EQUAL_UINT8(0x78, testBuffer[3]);
}

/**
 * @brief Validate multiple 16-bit values in sequence.
 */
void test_Dcep_Multiple_Uint16_Values(void)
{
    const uint16_t testValues[] = {0x0000, 0x1234, 0x5678, 0x9ABC, 0xDEF0, 0xFFFF};
    const size_t numValues = sizeof(testValues) / sizeof(testValues[0]);
    uint16_t readValue;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    for (size_t i = 0; i < numValues; i++)
    {
        /* Clear buffer. */
        memset(testBuffer, 0, sizeof(testBuffer));

        /* Write and read back the value. */
        readWriteFunctions.writeUint16Fn(testBuffer, testValues[i]);
        readValue = readWriteFunctions.readUint16Fn(testBuffer);

        /* Verify round-trip integrity. */
        TEST_ASSERT_EQUAL_UINT16(testValues[i], readValue);
    }
}

/**
 * @brief Validate multiple 32-bit values in sequence.
 */
void test_Dcep_Multiple_Uint32_Values(void)
{
    const uint32_t testValues[] = {
        0x00000000, 0x12345678, 0x9ABCDEF0, 
        0xFFFFFFFF, 0x01234567, 0x89ABCDEF
    };
    const size_t numValues = sizeof(testValues) / sizeof(testValues[0]);
    uint32_t readValue;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    for (size_t i = 0; i < numValues; i++)
    {
        /* Clear buffer. */
        memset(testBuffer, 0, sizeof(testBuffer));

        /* Write and read back the value. */
        readWriteFunctions.writeUint32Fn(testBuffer, testValues[i]);
        readValue = readWriteFunctions.readUint32Fn(testBuffer);

        /* Verify round-trip integrity. */
        TEST_ASSERT_EQUAL_UINT32(testValues[i], readValue);
    }
}

/**
 * @brief Validate boundary values for 16-bit operations.
 */
void test_Dcep_Boundary_Values_Uint16(void)
{
    uint16_t readValue;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Test minimum value. */
    readWriteFunctions.writeUint16Fn(testBuffer, 0x0000);
    readValue = readWriteFunctions.readUint16Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT16(0x0000, readValue);

    /* Test maximum value. */
    memset(testBuffer, 0, sizeof(testBuffer));
    readWriteFunctions.writeUint16Fn(testBuffer, 0xFFFF);
    readValue = readWriteFunctions.readUint16Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT16(0xFFFF, readValue);

    /* Test alternating bit pattern. */
    memset(testBuffer, 0, sizeof(testBuffer));
    readWriteFunctions.writeUint16Fn(testBuffer, 0xAAAA);
    readValue = readWriteFunctions.readUint16Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT16(0xAAAA, readValue);

    /* Test inverted alternating bit pattern. */
    memset(testBuffer, 0, sizeof(testBuffer));
    readWriteFunctions.writeUint16Fn(testBuffer, 0x5555);
    readValue = readWriteFunctions.readUint16Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT16(0x5555, readValue);
}

/**
 * @brief Validate boundary values for 32-bit operations.
 */
void test_Dcep_Boundary_Values_Uint32(void)
{
    uint32_t readValue;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Test minimum value. */
    readWriteFunctions.writeUint32Fn(testBuffer, 0x00000000);
    readValue = readWriteFunctions.readUint32Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT32(0x00000000, readValue);

    /* Test maximum value. */
    memset(testBuffer, 0, sizeof(testBuffer));
    readWriteFunctions.writeUint32Fn(testBuffer, 0xFFFFFFFF);
    readValue = readWriteFunctions.readUint32Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFF, readValue);

    /* Test alternating bit pattern. */
    memset(testBuffer, 0, sizeof(testBuffer));
    readWriteFunctions.writeUint32Fn(testBuffer, 0xAAAAAAAA);
    readValue = readWriteFunctions.readUint32Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, readValue);

    /* Test inverted alternating bit pattern. */
    memset(testBuffer, 0, sizeof(testBuffer));
    readWriteFunctions.writeUint32Fn(testBuffer, 0x55555555);
    readValue = readWriteFunctions.readUint32Fn(testBuffer);
    TEST_ASSERT_EQUAL_UINT32(0x55555555, readValue);
}

/**
 * @brief Validate sequential write operations don't interfere.
 */
void test_Dcep_Sequential_Operations(void)
{
    const uint16_t value16_1 = 0x1234;
    const uint16_t value16_2 = 0x5678;
    const uint32_t value32 = 0x9ABCDEF0;
    uint16_t read16_1, read16_2;
    uint32_t read32;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Write values sequentially. */
    readWriteFunctions.writeUint16Fn(&testBuffer[0], value16_1);
    readWriteFunctions.writeUint16Fn(&testBuffer[2], value16_2);
    readWriteFunctions.writeUint32Fn(&testBuffer[4], value32);

    /* Read values back. */
    read16_1 = readWriteFunctions.readUint16Fn(&testBuffer[0]);
    read16_2 = readWriteFunctions.readUint16Fn(&testBuffer[2]);
    read32 = readWriteFunctions.readUint32Fn(&testBuffer[4]);

    /* Verify all values are correct. */
    TEST_ASSERT_EQUAL_UINT16(value16_1, read16_1);
    TEST_ASSERT_EQUAL_UINT16(value16_2, read16_2);
    TEST_ASSERT_EQUAL_UINT32(value32, read32);
}

/**
 * @brief Validate that unaligned access works correctly.
 */
void test_Dcep_Unaligned_Access(void)
{
    const uint16_t value16 = 0x1234;
    const uint32_t value32 = 0x56789ABC;
    uint16_t read16;
    uint32_t read32;

    Dcep_InitReadWriteFunctions(&readWriteFunctions);

    /* Write to unaligned addresses. */
    readWriteFunctions.writeUint16Fn(&testBuffer[1], value16);  /* Unaligned by 1 byte */
    readWriteFunctions.writeUint32Fn(&testBuffer[5], value32);  /* Unaligned by 1 byte */

    /* Read from unaligned addresses. */
    read16 = readWriteFunctions.readUint16Fn(&testBuffer[1]);
    read32 = readWriteFunctions.readUint32Fn(&testBuffer[5]);

    /* Verify values are correct. */
    TEST_ASSERT_EQUAL_UINT16(value16, read16);
    TEST_ASSERT_EQUAL_UINT32(value32, read32);
}

/*-----------------------------------------------------------*/
