/* Unity includes. */
#include "unity.h"

/* Standard includes. */
#include <string.h>
#include <stdint.h>

/* API includes. */
#include "dcep_endianness.h"

/* ==============================  Test Cases for Endianness Functions ============================== */

/**
 * @brief Validate Dcep_InitReadWriteFunctions initializes function pointers.
 */
void test_dcepInitReadWriteFunctions( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    TEST_ASSERT_NOT_NULL( readWriteFunctions.writeUint16Fn );
    TEST_ASSERT_NOT_NULL( readWriteFunctions.writeUint32Fn );
    TEST_ASSERT_NOT_NULL( readWriteFunctions.readUint16Fn );
    TEST_ASSERT_NOT_NULL( readWriteFunctions.readUint32Fn );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate uint16 network byte order (big-endian).
 */
void test_dcepUint16NetworkByteOrder( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;
    uint8_t buffer[ 2 ];
    uint16_t testValue = 0x1234;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    readWriteFunctions.writeUint16Fn( &( buffer[ 0 ] ),
                                      testValue );

    /* Should produce network byte order (big-endian) */
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             buffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             buffer[ 1 ] );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate uint32 network byte order (big-endian).
 */
void test_dcepUint32NetworkByteOrder( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;
    uint8_t buffer[ 4 ];
    uint32_t testValue = 0x12345678;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    readWriteFunctions.writeUint32Fn( &( buffer[ 0 ] ),
                                      testValue );

    /* Should produce network byte order (big-endian) */
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             buffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             buffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x56,
                             buffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x78,
                             buffer[ 3 ] );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate uint16 read function interprets network byte order correctly.
 */
void test_dcepUint16ReadFunction( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;
    uint8_t buffer[ 2 ] = { 0x12, 0x34 }; /* Network byte order (big-endian) */
    uint16_t readValue;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    readValue = readWriteFunctions.readUint16Fn( &( buffer[ 0 ] ) );
    
    /* Should convert from network to host order */
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             ( readValue >> 8 ) & 0xFF );
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             readValue & 0xFF );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate uint32 read function interprets network byte order correctly.
 */
void test_dcepUint32ReadFunction( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;
    uint8_t buffer[ 4 ] = { 0x12, 0x34, 0x56, 0x78 }; /* Network byte order (big-endian) */
    uint32_t readValue;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    readValue = readWriteFunctions.readUint32Fn( &( buffer[ 0 ] ) );
    
    /* Should convert from network to host order */
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             ( readValue >> 24 ) & 0xFF );
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             ( readValue >> 16 ) & 0xFF );
    TEST_ASSERT_EQUAL_UINT8( 0x56,
                             ( readValue >> 8 ) & 0xFF );
    TEST_ASSERT_EQUAL_UINT8( 0x78,
                             readValue & 0xFF );
}

/*-----------------------------------------------------------*/
