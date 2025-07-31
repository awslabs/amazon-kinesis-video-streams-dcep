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

#ifdef FORCE_BIG_ENDIAN
    /* Big-endian mode: NoSwap functions on little-endian host produce host order */
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             buffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             buffer[ 1 ] );
#else
    /* Little-endian or auto mode: should produce network byte order (big-endian) */
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             buffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             buffer[ 1 ] );
#endif
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

#ifdef FORCE_BIG_ENDIAN
    /* Big-endian mode: NoSwap functions on little-endian host produce host order */
    TEST_ASSERT_EQUAL_UINT8( 0x78,
                             buffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x56,
                             buffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             buffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             buffer[ 3 ] );
#else
    /* Little-endian or auto mode: should produce network byte order (big-endian) */
    TEST_ASSERT_EQUAL_UINT8( 0x12,
                             buffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x34,
                             buffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x56,
                             buffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0x78,
                             buffer[ 3 ] );
#endif
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
    
#ifdef FORCE_BIG_ENDIAN
    /* Big-endian mode: should read as host order */
    TEST_ASSERT_EQUAL_UINT16( 0x3412, readValue );
#else
    /* Little-endian or auto mode: should convert from network to host order */
    TEST_ASSERT_EQUAL_UINT16( 0x1234, readValue );
#endif
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
    
#ifdef FORCE_BIG_ENDIAN
    /* Big-endian mode: should read as host order */
    TEST_ASSERT_EQUAL_UINT32( 0x78563412, readValue );
#else
    /* Little-endian or auto mode: should convert from network to host order */
    TEST_ASSERT_EQUAL_UINT32( 0x12345678, readValue );
#endif
}

/*-----------------------------------------------------------*/
