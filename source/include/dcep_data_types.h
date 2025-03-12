#ifndef DCEP_DATA_TYPES_H
#define DCEP_DATA_TYPES_H

/* Standard includes. */
#include <stdint.h>
#include <stddef.h>

/* Endianness includes. */
#include "dcep_endianness.h"

/*-----------------------------------------------------------*/

typedef enum DcepResult
{
    DCEP_RESULT_OK,
    DCEP_RESULT_BAD_PARAM,
    DCEP_RESULT_OUT_OF_MEMORY,
    DCEP_RESULT_MALFORMED_MESSAGE
} DcepResult_t;

typedef enum DcepMessageType
{
    DCEP_MESSAGE_DATA_CHANNEL_OPEN = 0x03,
    DCEP_MESSAGE_DATA_CHANNEL_ACK  = 0x02
} DcepMessageType_t;

typedef enum DcepChannelType
{
    DCEP_DATA_CHANNEL_RELIABLE                          = 0x00,
    DCEP_DATA_CHANNEL_RELIABLE_UNORDERED                = 0x80,
    DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT           = 0x01,
    DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED = 0x81,
    DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED            = 0x02,
    DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED  = 0x82
} DcepChannelType_t;

/*-----------------------------------------------------------*/

typedef struct DcepContext
{
    DcepReadWriteFunctions_t readWriteFunctions;
} DcepContext_t;

/*
 * RFC - https://datatracker.ietf.org/doc/html/draft-ietf-rtcweb-data-protocol-09#section-5.1
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
 * \                                                               /
 * |                             Label                             |
 * /                                                               \
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * \                                                               /
 * |                            Protocol                           |
 * /                                                               \
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
typedef struct DcepChannelOpenMessage
{
    DcepChannelType_t channelType;
    uint16_t priority;

    /* Only used when channelType is DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT
     * or DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED. */
    uint32_t numRetransmissions;

    /* Only used when channelType is DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED
     * or DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED. */
    uint32_t maxLifetimeInMilliseconds;

    const uint8_t * pChannelName;
    uint16_t channelNameLength;

    const uint8_t * pProtocol;
    uint16_t protocolLength;
} DcepChannelOpenMessage_t;

/*-----------------------------------------------------------*/

#endif /* DCEP_DATA_TYPES_H */
