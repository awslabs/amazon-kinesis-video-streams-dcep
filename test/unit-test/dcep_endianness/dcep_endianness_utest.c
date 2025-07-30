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
 * @brief Validate uint16 functions handle misaligned buffers.
 */
void test_dcepUint16MisalignedBuffer( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;
    uint8_t buffer[ 5 ];
    uint16_t testValue = 0x1234;
    uint16_t readValue;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    /* Write to misaligned address (offset 1) */
    readWriteFunctions.writeUint16Fn( &( buffer[ 1 ] ),
                                      testValue );
    
    /* Read back from same misaligned address */
    readValue = readWriteFunctions.readUint16Fn( &( buffer[ 1 ] ) );
    TEST_ASSERT_EQUAL_UINT16( testValue,
                              readValue );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate uint32 functions handle misaligned buffers.
 */
void test_dcepUint32MisalignedBuffer( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;
    uint8_t buffer[ 7 ];
    uint32_t testValue = 0x12345678;
    uint32_t readValue;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    /* Write to misaligned address (offset 1) */
    readWriteFunctions.writeUint32Fn( &( buffer[ 1 ] ),
                                      testValue );
    
    /* Read back from same misaligned address */
    readValue = readWriteFunctions.readUint32Fn( &( buffer[ 1 ] ) );
    TEST_ASSERT_EQUAL_UINT32( testValue,
                              readValue );
}

/*-----------------------------------------------------------*/

/**
 * @brief Validate critical edge values for both uint16 and uint32.
 */
void test_dcepEdgeValues( void )
{
    DcepReadWriteFunctions_t readWriteFunctions;
    uint8_t buffer[ 4 ];
    uint16_t readValue16;
    uint32_t readValue32;

    Dcep_InitReadWriteFunctions( &( readWriteFunctions ) );

    /* Test uint16 critical edge case */
    readWriteFunctions.writeUint16Fn( &( buffer[ 0 ] ),
                                      0xFFFF );
    readValue16 = readWriteFunctions.readUint16Fn( &( buffer[ 0 ] ) );
    TEST_ASSERT_EQUAL_UINT16( 0xFFFF,
                              readValue16 );

    /* Test uint32 critical edge case */
    readWriteFunctions.writeUint32Fn( &( buffer[ 0 ] ),
                                      0xFFFFFFFF );
    readValue32 = readWriteFunctions.readUint32Fn( &( buffer[ 0 ] ) );
    TEST_ASSERT_EQUAL_UINT32( 0xFFFFFFFF,
                              readValue32 );
}

/*-----------------------------------------------------------*/
