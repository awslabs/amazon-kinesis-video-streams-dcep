/* API includes. */
#include "dcep_endianness.h"

#define SWAP_BYTES_16( value )          \
    ( ( ( ( value ) >> 8 ) & 0xFF ) |   \
      ( ( ( value ) & 0xFF ) << 8 ) )

#define SWAP_BYTES_32( value )           \
    ( ( ( ( value ) >> 24 ) & 0xFF )  |  \
      ( ( ( value ) >> 8 ) & 0xFF00 ) |  \
      ( ( ( value ) & 0xFF00 ) << 8 ) |  \
      ( ( ( value ) & 0xFF ) << 24 ) )

/*-----------------------------------------------------------*/

static void DcepWriteUint16Swap( uint8_t * pDst, uint16_t val )
{
    *( ( uint16_t * )( pDst ) ) = SWAP_BYTES_16( val );
}

/*-----------------------------------------------------------*/

static void DcepWriteUint32Swap( uint8_t * pDst, uint32_t val )
{
    *( ( uint32_t * )( pDst ) ) = SWAP_BYTES_32( val );
}

/*-----------------------------------------------------------*/

static uint16_t DcepReadUint16Swap( const uint8_t * pSrc )
{
    return SWAP_BYTES_16( *( ( uint16_t * )( pSrc ) ) );
}

/*-----------------------------------------------------------*/

static uint32_t DcepReadUint32Swap( const uint8_t * pSrc )
{
    return SWAP_BYTES_32( *( ( uint32_t * )( pSrc ) ) );
}

/*-----------------------------------------------------------*/

static void DcepWriteUint16NoSwap( uint8_t * pDst, uint16_t val )
{
    *( ( uint16_t * )( pDst ) ) = ( val );
}

/*-----------------------------------------------------------*/

static void DcepWriteUint32NoSwap( uint8_t * pDst, uint32_t val )
{
    *( ( uint32_t * )( pDst ) ) = ( val );
}

/*-----------------------------------------------------------*/

static uint16_t DcepReadUint16NoSwap( const uint8_t * pSrc )
{
    return *( ( uint16_t * )( pSrc ) );
}

/*-----------------------------------------------------------*/

static uint32_t DcepReadUint32NoSwap( const uint8_t * pSrc )
{
    return *( ( uint32_t * )( pSrc ) );
}

/*-----------------------------------------------------------*/

void Dcep_InitReadWriteFunctions( DcepReadWriteFunctions_t * pReadWriteFunctions )
{
    uint8_t isLittleEndian;

    isLittleEndian = ( *( uint8_t * )( &( uint16_t ){ 1 } ) == 1 );

    if( isLittleEndian != 0 )
    {
        pReadWriteFunctions->writeUint16Fn = DcepWriteUint16Swap;
        pReadWriteFunctions->writeUint32Fn = DcepWriteUint32Swap;
        pReadWriteFunctions->readUint16Fn = DcepReadUint16Swap;
        pReadWriteFunctions->readUint32Fn = DcepReadUint32Swap;
    }
    else
    {
        pReadWriteFunctions->writeUint16Fn = DcepWriteUint16NoSwap;
        pReadWriteFunctions->writeUint32Fn = DcepWriteUint32NoSwap;
        pReadWriteFunctions->readUint16Fn = DcepReadUint16NoSwap;
        pReadWriteFunctions->readUint32Fn = DcepReadUint32NoSwap;
    }
}

/*-----------------------------------------------------------*/
