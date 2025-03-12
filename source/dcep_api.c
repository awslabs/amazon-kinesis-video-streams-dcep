/* Standard includes. */
#include <string.h>

/* API includes. */
#include "dcep_api.h"

/*-----------------------------------------------------------*/

/*
 * Helper macros.
 */
#define DCEP_WRITE_UINT16   ( pCtx->readWriteFunctions.writeUint16Fn )
#define DCEP_WRITE_UINT32   ( pCtx->readWriteFunctions.writeUint32Fn )
#define DCEP_READ_UINT16    ( pCtx->readWriteFunctions.readUint16Fn )
#define DCEP_READ_UINT32    ( pCtx->readWriteFunctions.readUint32Fn )

/*-----------------------------------------------------------*/

/* DCEP Header:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  Message Type |  Channel Type |            Priority           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                    Reliability Parameter                      |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |         Label Length          |       Protocol Length         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#define DCEP_HEADER_LENGTH                      12
#define DCEP_MESSAGE_TYPE_OFFSET                0
#define DCEP_MESSAGE_TYPE_LENGTH                1
#define DCEP_CHANNEL_TYPE_OFFSET                1
#define DCEP_PRIORITY_OFFSET                    2
#define DCEP_RELIABILITY_PARAMETER_OFFSET       4
#define DCEP_LABEL_LENGTH_OFFSET                8
#define DCEP_PROTOCOL_LENGTH_OFFSET             10

/*-----------------------------------------------------------*/

DcepResult_t Dcep_Init( DcepContext_t * pCtx )
{
    DcepResult_t result = DCEP_RESULT_OK;

    if( pCtx == NULL )
    {
        result = DCEP_RESULT_BAD_PARAM;
    }

    if( result == DCEP_RESULT_OK )
    {
        Dcep_InitReadWriteFunctions( &( pCtx->readWriteFunctions ) );
    }

    return result;
}

/*-----------------------------------------------------------*/

DcepResult_t Dcep_SerializeChannelOpenMessage( DcepContext_t * pCtx,
                                               const DcepChannelOpenMessage_t * pChannelOpenMessage,
                                               uint8_t * pBuffer,
                                               size_t * pBufferLength )
{
    DcepResult_t result = DCEP_RESULT_OK;
    uint32_t reliabilityValue = 0;
    size_t serializedMessageLength = 0;

    if( ( pCtx == NULL ) ||
        ( pChannelOpenMessage == NULL ) ||
        ( pBuffer == NULL ) ||
        ( pBufferLength == NULL ) ||
        ( *pBufferLength < DCEP_HEADER_LENGTH ) )
    {
        result = DCEP_RESULT_BAD_PARAM;
    }

    if( result == DCEP_RESULT_OK )
    {
        pBuffer[ DCEP_MESSAGE_TYPE_OFFSET ] = DCEP_MESSAGE_DATA_CHANNEL_OPEN;
        pBuffer[ DCEP_CHANNEL_TYPE_OFFSET ] = pChannelOpenMessage->channelType;

        DCEP_WRITE_UINT16( &( pBuffer[ DCEP_PRIORITY_OFFSET ] ),
                           pChannelOpenMessage->priority );

        if( ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT ) ||
            ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED ) )
        {
            reliabilityValue = pChannelOpenMessage->numRetransmissions;
        }
        else if( ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED ) ||
                 ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED ) )
        {
            reliabilityValue = pChannelOpenMessage->maxLifetimeInMilliseconds;
        }
        else
        {
            reliabilityValue = 0;
        }

        DCEP_WRITE_UINT32( &( pBuffer[ DCEP_RELIABILITY_PARAMETER_OFFSET ] ),
                           reliabilityValue );

        DCEP_WRITE_UINT16( &( pBuffer[ DCEP_LABEL_LENGTH_OFFSET ] ),
                           pChannelOpenMessage->channelNameLength );

        DCEP_WRITE_UINT16( &( pBuffer[ DCEP_PROTOCOL_LENGTH_OFFSET ] ),
                           pChannelOpenMessage->protocolLength );

        serializedMessageLength += DCEP_HEADER_LENGTH;
    }

    if( result == DCEP_RESULT_OK )
    {
        if( pChannelOpenMessage->channelNameLength > 0 )
        {
            if( *pBufferLength < ( serializedMessageLength + pChannelOpenMessage->channelNameLength ) )
            {
                result = DCEP_RESULT_OUT_OF_MEMORY;
            }
            else
            {
                memcpy( &( pBuffer[ serializedMessageLength ] ),
                        pChannelOpenMessage->pChannelName,
                        pChannelOpenMessage->channelNameLength );

                serializedMessageLength += pChannelOpenMessage->channelNameLength;
            }
        }
    }

    if( result == DCEP_RESULT_OK )
    {
        if( pChannelOpenMessage->protocolLength > 0 )
        {
            if( *pBufferLength < ( serializedMessageLength + pChannelOpenMessage->protocolLength ) )
            {
                result = DCEP_RESULT_OUT_OF_MEMORY;
            }
            else
            {
                memcpy( &( pBuffer[ serializedMessageLength ] ),
                        pChannelOpenMessage->pProtocol,
                        pChannelOpenMessage->protocolLength );

                serializedMessageLength += pChannelOpenMessage->protocolLength;
            }
        }
    }

    if( result == DCEP_RESULT_OK )
    {
        *pBufferLength = serializedMessageLength;
    }

    return result;
}

/*-----------------------------------------------------------*/

DcepResult_t Dcep_SerializeChannelAckMessage( DcepContext_t * pCtx,
                                               uint8_t * pBuffer,
                                               size_t * pBufferLength )
{
    DcepResult_t result = DCEP_RESULT_OK;
    uint32_t reliabilityValue = 0;
    size_t serializedMessageLength = 0;

    if( ( pCtx == NULL ) ||
        ( pBuffer == NULL ) ||
        ( pBufferLength == NULL ) ||
        ( *pBufferLength < DCEP_MESSAGE_TYPE_LENGTH ) )
    {
        result = DCEP_RESULT_BAD_PARAM;
    }

    if( result == DCEP_RESULT_OK )
    {
        pBuffer[ DCEP_MESSAGE_TYPE_OFFSET ] = DCEP_MESSAGE_DATA_CHANNEL_ACK;
        *pBufferLength = DCEP_MESSAGE_TYPE_LENGTH;
    }

    return result;
}

/*-----------------------------------------------------------*/

DcepResult_t Dcep_DeserializeChannelOpenMessage( DcepContext_t * pCtx,
                                                 const uint8_t * pDcepMessage,
                                                 size_t dcepMessageLength,
                                                 DcepChannelOpenMessage_t * pChannelOpenMessage )
{
    DcepResult_t result = DCEP_RESULT_OK;
    uint32_t reliabilityValue = 0;
    size_t consumedLength = 0;

    if( ( pCtx == NULL ) ||
        ( pDcepMessage == NULL ) ||
        ( pChannelOpenMessage == NULL ) ||
        ( dcepMessageLength < DCEP_HEADER_LENGTH ) ||
        ( pDcepMessage[ DCEP_MESSAGE_TYPE_OFFSET ] != DCEP_MESSAGE_DATA_CHANNEL_OPEN ) )
    {
        result = DCEP_RESULT_BAD_PARAM;
    }

    if( result == DCEP_RESULT_OK )
    {
        pChannelOpenMessage->channelType = pDcepMessage[ DCEP_CHANNEL_TYPE_OFFSET ];
        pChannelOpenMessage->priority = DCEP_READ_UINT16( &( pDcepMessage[ DCEP_PRIORITY_OFFSET ] ) );

        reliabilityValue = DCEP_READ_UINT32( &( pDcepMessage[ DCEP_RELIABILITY_PARAMETER_OFFSET ] ) );

        if( ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT ) ||
            ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED ) )
        {
            pChannelOpenMessage->numRetransmissions = reliabilityValue;
        }
        else if( ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED ) ||
                 ( pChannelOpenMessage->channelType == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED ) )
        {
            pChannelOpenMessage->maxLifetimeInMilliseconds = reliabilityValue;
        }

        pChannelOpenMessage->channelNameLength = DCEP_READ_UINT16( &( pDcepMessage[ DCEP_LABEL_LENGTH_OFFSET ] ) );
        pChannelOpenMessage->protocolLength = DCEP_READ_UINT16( &( pDcepMessage[ DCEP_PROTOCOL_LENGTH_OFFSET ] ) );

        consumedLength += DCEP_HEADER_LENGTH;
    }

    if( result == DCEP_RESULT_OK )
    {
        if( pChannelOpenMessage->channelNameLength > 0 )
        {
            if( dcepMessageLength < ( consumedLength + pChannelOpenMessage->channelNameLength ) )
            {
                result = DCEP_RESULT_MALFORMED_MESSAGE;
            }
            else
            {
                pChannelOpenMessage->pChannelName = &( pDcepMessage[ consumedLength ] );
                consumedLength += pChannelOpenMessage->channelNameLength;
            }
        }
        else
        {
            pChannelOpenMessage->pChannelName = NULL;
        }
    }

    if( result == DCEP_RESULT_OK )
    {
        if( pChannelOpenMessage->protocolLength > 0 )
        {
            if( dcepMessageLength < ( consumedLength + pChannelOpenMessage->protocolLength ) )
            {
                result = DCEP_RESULT_MALFORMED_MESSAGE;
            }
            else
            {
                pChannelOpenMessage->pProtocol = &( pDcepMessage[ consumedLength ] );
                consumedLength += pChannelOpenMessage->protocolLength;
            }
        }
        else
        {
            pChannelOpenMessage->pProtocol = NULL;
        }
    }

    return result;
}

/*-----------------------------------------------------------*/

DcepResult_t Dcep_GetMessageType( DcepContext_t * pCtx,
                                  const uint8_t * pDcepMessage,
                                  size_t dcepMessageLength,
                                  DcepMessageType_t * pDcepMessageType )
{
    DcepResult_t result = DCEP_RESULT_OK;

    if( ( pCtx == NULL ) ||
        ( pDcepMessage == NULL ) ||
        ( dcepMessageLength == 0 ) ||
        ( pDcepMessageType == NULL ) )
    {
        result = DCEP_RESULT_BAD_PARAM;
    }

    if( result == DCEP_RESULT_OK )
    {
        if( ( pDcepMessage[ DCEP_MESSAGE_TYPE_OFFSET ] == DCEP_MESSAGE_DATA_CHANNEL_OPEN ) ||
            ( pDcepMessage[ DCEP_MESSAGE_TYPE_OFFSET ] == DCEP_MESSAGE_DATA_CHANNEL_ACK ) )
        {
            *pDcepMessageType = pDcepMessage[ DCEP_MESSAGE_TYPE_OFFSET ];
        }
        else
        {
            result = DCEP_RESULT_MALFORMED_MESSAGE;
        }
    }

    return result;
}

/*-----------------------------------------------------------*/
