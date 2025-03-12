#ifndef DCEP_ENDIANNESS_H
#define DCEP_ENDIANNESS_H

/* Standard includes. */
#include <stdint.h>

/* Endianness Function types. */
typedef void ( * DcepWriteUint16_t ) ( uint8_t * pDst, uint16_t val );
typedef void ( * DcepWriteUint32_t ) ( uint8_t * pDst, uint32_t val );
typedef uint16_t ( * DcepReadUint16_t ) ( const uint8_t * pSrc );
typedef uint32_t ( * DcepReadUint32_t ) ( const uint8_t * pSrc );

typedef struct DcepReadWriteFunctions
{
    DcepWriteUint16_t writeUint16Fn;
    DcepWriteUint32_t writeUint32Fn;
    DcepReadUint16_t readUint16Fn;
    DcepReadUint32_t readUint32Fn;
} DcepReadWriteFunctions_t;

void Dcep_InitReadWriteFunctions( DcepReadWriteFunctions_t * pReadWriteFunctions );

#endif /* DCEP_ENDIANNESS_H */
