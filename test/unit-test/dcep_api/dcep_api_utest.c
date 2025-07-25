/* Unity includes. */
#include "unity.h"

/* Standard includes. */
#include <string.h>
#include <stdint.h>

/* API includes. */
#include "dcep_api.h"

/* ===========================  EXTERN VARIABLES  =========================== */

#define MAX_BUFFER_LENGTH       1024
#define MAX_CHANNEL_NAME_LENGTH 256
#define MAX_PROTOCOL_LENGTH     256

uint8_t testBuffer[MAX_BUFFER_LENGTH];
uint8_t channelNameBuffer[MAX_CHANNEL_NAME_LENGTH];
uint8_t protocolBuffer[MAX_PROTOCOL_LENGTH];

void setUp(void)
{
    memset(&(testBuffer[0]), 0, sizeof(testBuffer));
    memset(&(channelNameBuffer[0]), 0, sizeof(channelNameBuffer));
    memset(&(protocolBuffer[0]), 0, sizeof(protocolBuffer));
}

void tearDown(void)
{
}

/* ==============================  Test Cases for Initialization ============================== */

/**
 * @brief Validate Dcep_Init happy path.
 */
void test_dcepInit(void)
{
    DcepResult_t result;
    DcepContext_t ctx;

    result = Dcep_Init(&ctx);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_NOT_NULL(ctx.readWriteFunctions.writeUint16Fn);
    TEST_ASSERT_NOT_NULL(ctx.readWriteFunctions.writeUint32Fn);
    TEST_ASSERT_NOT_NULL(ctx.readWriteFunctions.readUint16Fn);
    TEST_ASSERT_NOT_NULL(ctx.readWriteFunctions.readUint32Fn);
}

/**
 * @brief Validate Dcep_Init with bad parameters.
 */
void test_dcepInit_BadParams(void)
{
    DcepResult_t result;

    result = Dcep_Init(NULL);

    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);
}

/* ==============================  Test Cases for Channel Open Message Serialization ============================== */

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage happy path with reliable channel.
 */
void test_dcepSerializeChannelOpenMessage_Reliable(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;
    uint8_t expectedChannelName[] = "test-channel";
    uint8_t expectedProtocol[] = "test-protocol";

    // Expected serialized message
    uint8_t expected[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN,    // 0x03
        DCEP_DATA_CHANNEL_RELIABLE,        // 0x00
        0x12, 0x34,                        // Priority (0x1234) in little endian
        0x00, 0x00, 0x00, 0x00,            // Reliability param = 0
        0x00, 0x0C,                        // Channel name length = 12
        0x00, 0x0D,                        // Protocol length = 13
        't','e','s','t','-','c','h','a','n','n','e','l',
        't','e','s','t','-','p','r','o','t','o','c','o','l'
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup channel open message. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_RELIABLE;
    channelOpenMessage.priority = 0x1234;
    channelOpenMessage.pChannelName = expectedChannelName;
    channelOpenMessage.channelNameLength = sizeof(expectedChannelName) - 1;
    channelOpenMessage.pProtocol = expectedProtocol;
    channelOpenMessage.protocolLength = sizeof(expectedProtocol) - 1;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(sizeof(expected), bufferLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, testBuffer, sizeof(expected));
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with partial reliable retransmit channel.
 */
void test_dcepSerializeChannelOpenMessage_PartialReliableRetransmit(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;
    // Expected serialized message
    uint8_t expected[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN,     // 0x03
        DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT,  // 0x01
        0x56, 0x78,                         // Priority (0x5678)
        0x00, 0x00, 0x00, 0x05,             // Retransmissions = 5
        0x00, 0x00,                         // Channel name length = 0
        0x00, 0x00                          // Protocol length = 0
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup channel open message. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT;
    channelOpenMessage.priority = 0x5678;
    channelOpenMessage.numRetransmissions = 5;
    channelOpenMessage.pChannelName = NULL;
    channelOpenMessage.channelNameLength = 0;
    channelOpenMessage.pProtocol = NULL;
    channelOpenMessage.protocolLength = 0;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(sizeof(expected), bufferLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, testBuffer, sizeof(expected));
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with partial reliable timed channel.
 */
    void test_dcepSerializeChannelOpenMessage_PartialReliableTimed(void)
    {
        DcepResult_t result;
        DcepContext_t ctx;
        DcepChannelOpenMessage_t channelOpenMessage = {0};
        size_t bufferLength = MAX_BUFFER_LENGTH;
        // Expected serialized message
        uint8_t expected[] = {
            DCEP_MESSAGE_DATA_CHANNEL_OPEN,              // 0x03
            DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED,   // 0x02
            0xAB, 0xCD,                                  // Priority (0xABCD)
            0x00, 0x00, 0x03, 0xE8,                      // maxLifetimeInMilliseconds = 1000 (0x03E8)
            0x00, 0x00,                                  // Channel name length = 0
            0x00, 0x00                                   // Protocol length = 0
        };

        result = Dcep_Init(&ctx);
        TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

        /* Setup channel open message. */
        channelOpenMessage.channelType = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED;
        channelOpenMessage.priority = 0xABCD;
        channelOpenMessage.maxLifetimeInMilliseconds = 1000;
        channelOpenMessage.pChannelName = NULL;
        channelOpenMessage.channelNameLength = 0;
        channelOpenMessage.pProtocol = NULL;
        channelOpenMessage.protocolLength = 0;

        result = Dcep_SerializeChannelOpenMessage(&ctx,
                                                &channelOpenMessage,
                                                testBuffer,
                                                &bufferLength);

        TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
        TEST_ASSERT_EQUAL(sizeof(expected), bufferLength);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, testBuffer, sizeof(expected));
    }

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with unordered reliable channel.
 */
void test_dcepSerializeChannelOpenMessage_ReliableUnordered(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;
    // Expected serialized message
    uint8_t expected[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN,              // 0x03
        DCEP_DATA_CHANNEL_RELIABLE_UNORDERED,        // 0x80
        0xEF, 0x01,                                  // Priority (0xEF01)
        0x00, 0x00, 0x00, 0x00,                      
        0x00, 0x00,                                  // Channel name length = 0
        0x00, 0x00                                   // Protocol length = 0
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup channel open message. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_RELIABLE_UNORDERED;
    channelOpenMessage.priority = 0xEF01;
    channelOpenMessage.pChannelName = NULL;
    channelOpenMessage.channelNameLength = 0;
    channelOpenMessage.pProtocol = NULL;
    channelOpenMessage.protocolLength = 0;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(sizeof(expected), bufferLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, testBuffer, sizeof(expected));
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with partial reliable retransmit unordered channel.
 */
void test_dcepSerializeChannelOpenMessage_PartialReliableRetransmitUnordered(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    // Expected serialized message
    uint8_t expected[] =
    {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN,                  // 0x03
        DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED,  // e.g. 0x81
        0x23, 0x45,                                      // Priority (0x2345)
        0x00, 0x00, 0x00, 0x0A,                          // numRetransmissions = 10
        0x00, 0x00,                                      // Channel name length = 0
        0x00, 0x00                                       // Protocol length = 0
    };

    /* Setup channel open message. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED;
    channelOpenMessage.priority = 0x2345;
    channelOpenMessage.numRetransmissions = 10;
    channelOpenMessage.pChannelName = NULL;
    channelOpenMessage.channelNameLength = 0;
    channelOpenMessage.pProtocol = NULL;
    channelOpenMessage.protocolLength = 0;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(sizeof(expected), bufferLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, testBuffer, sizeof(expected));
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with partial reliable timed unordered channel.
 */
void test_dcepSerializeChannelOpenMessage_PartialReliableTimedUnordered(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;
    uint8_t expected[] = 
    {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN,                     // 0x03
        DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED, // Likely 0x82
        0x67, 0x89,                                         // Priority = 0x6789
        0x00, 0x00, 0x07, 0xD0,                             // maxLifetimeInMilliseconds = 2000 = 0x07D0
        0x00, 0x00,                                         // Channel name length = 0
        0x00, 0x00                                          // Protocol length = 0
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup channel open message. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED;
    channelOpenMessage.priority = 0x6789;
    channelOpenMessage.maxLifetimeInMilliseconds = 2000;
    channelOpenMessage.pChannelName = NULL;
    channelOpenMessage.channelNameLength = 0;
    channelOpenMessage.pProtocol = NULL;
    channelOpenMessage.protocolLength = 0;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(sizeof(expected), bufferLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, testBuffer, sizeof(expected));
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with empty channel name and protocol.
 */
void test_dcepSerializeChannelOpenMessage_EmptyNameAndProtocol(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;
    uint8_t expected[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN,    // 0x03
        DCEP_DATA_CHANNEL_RELIABLE,        // 0x00
        0x00, 0x00,                         // Priority
        0x00, 0x00, 0x00, 0x00,             // Reliability parameter (0)
        0x00, 0x00,                         // Channel name length = 0
        0x00, 0x00                          // Protocol length = 0
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup channel open message with empty name and protocol. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_RELIABLE;
    channelOpenMessage.priority = 0x0000;
    channelOpenMessage.pChannelName = NULL;
    channelOpenMessage.channelNameLength = 0;
    channelOpenMessage.pProtocol = NULL;
    channelOpenMessage.protocolLength = 0;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(sizeof(expected), bufferLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, testBuffer, sizeof(expected));
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with bad parameters.
 */
void test_dcepSerializeChannelOpenMessage_BadParams(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Test with NULL context. */
    result = Dcep_SerializeChannelOpenMessage(NULL,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL channel open message. */
    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             NULL,
                                             testBuffer,
                                             &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL buffer. */
    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             NULL,
                                             &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL buffer length. */
    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             NULL);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with insufficient buffer length. */
    bufferLength = DCEP_HEADER_LENGTH - 1;
    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with out of memory for channel name.
 */
void test_dcepSerializeChannelOpenMessage_OutOfMemory_ChannelName(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = DCEP_HEADER_LENGTH + 5; /* Small buffer. */
    uint8_t channelName[] = "very-long-channel-name";

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup channel open message with long channel name. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_RELIABLE;
    channelOpenMessage.priority = 0x1234;
    channelOpenMessage.pChannelName = channelName;
    channelOpenMessage.channelNameLength = sizeof(channelName) - 1;
    channelOpenMessage.pProtocol = NULL;
    channelOpenMessage.protocolLength = 0;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OUT_OF_MEMORY, result);
}

/**
 * @brief Validate Dcep_SerializeChannelOpenMessage with out of memory for protocol.
 */
void test_dcepSerializeChannelOpenMessage_OutOfMemory_Protocol(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    size_t bufferLength = DCEP_HEADER_LENGTH + 10; /* Small buffer. */
    uint8_t channelName[] = "short";
    uint8_t protocol[] = "very-long-protocol-name";

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup channel open message with long protocol. */
    channelOpenMessage.channelType = DCEP_DATA_CHANNEL_RELIABLE;
    channelOpenMessage.priority = 0x1234;
    channelOpenMessage.pChannelName = channelName;
    channelOpenMessage.channelNameLength = sizeof(channelName) - 1;
    channelOpenMessage.pProtocol = protocol;
    channelOpenMessage.protocolLength = sizeof(protocol) - 1;

    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &channelOpenMessage,
                                             testBuffer,
                                             &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OUT_OF_MEMORY, result);
}

/* ==============================  Test Cases for Channel Ack Message Serialization ============================== */

/**
 * @brief Validate Dcep_SerializeChannelAckMessage happy path.
 */
void test_dcepSerializeChannelAckMessage(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_SerializeChannelAckMessage(&ctx,
                                            testBuffer,
                                            &bufferLength);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(1, bufferLength);
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_ACK, testBuffer[0]);
}

/**
 * @brief Validate Dcep_SerializeChannelAckMessage with bad parameters.
 */
void test_dcepSerializeChannelAckMessage_BadParams(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Test with NULL context. */
    result = Dcep_SerializeChannelAckMessage(NULL,
                                            testBuffer,
                                            &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL buffer. */
    result = Dcep_SerializeChannelAckMessage(&ctx,
                                            NULL,
                                            &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL buffer length. */
    result = Dcep_SerializeChannelAckMessage(&ctx,
                                            testBuffer,
                                            NULL);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with insufficient buffer length. */
    bufferLength = 0;
    result = Dcep_SerializeChannelAckMessage(&ctx,
                                            testBuffer,
                                            &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);
}

/* ==============================  Test Cases for Channel Open Message Deserialization ============================== */

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage happy path with reliable channel.
 */
void test_dcepDeserializeChannelOpenMessage_Reliable(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};

    uint8_t expected[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN,    // 0x03
        DCEP_DATA_CHANNEL_RELIABLE,        // 0x00
        0x12, 0x34,                         // Priority = 0x1234
        0x00, 0x00, 0x00, 0x00,             // Reliability parameter = 0
        0x00, 0x0C,                         // Channel name length = 12
        0x00, 0x0D,                         // Protocol length = 13
        't','e','s','t','-','c','h','a','n','n','e','l',         // channel name
        't','e','s','t','-','p','r','o','t','o','c','o','l'      // protocol
    };

    size_t bufferLength = sizeof(expected);
    uint8_t expectedChannelName[] = "test-channel";
    uint8_t expectedProtocol[] = "test-protocol";

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               expected,
                                               bufferLength,
                                               &deserializedMessage);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_RELIABLE, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(0x1234, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(sizeof(expectedChannelName) - 1, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(sizeof(expectedProtocol) - 1, deserializedMessage.protocolLength);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedChannelName,
                                   deserializedMessage.pChannelName,
                                   deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedProtocol,
                                   deserializedMessage.pProtocol,
                                   deserializedMessage.protocolLength);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with reliable unordered channel.
 */
void test_dcepDeserializeChannelOpenMessage_ReliableUnordered(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};

    /* Serialized buffer for:
     * - Message Type = DCEP_MESSAGE_DATA_CHANNEL_OPEN (0x03)
     * - Channel Type = DCEP_DATA_CHANNEL_RELIABLE_UNORDERED (0x80)
     * - Priority = 0x1234
     * - Reliability parameter = 0
     * - Label Length = 0
     * - Protocol Length = 0
     */
    uint8_t expectedBuffer[] = {
        0x03,                   // DCEP_MESSAGE_DATA_CHANNEL_OPEN
        0x80,                   // DCEP_DATA_CHANNEL_RELIABLE_UNORDERED
        0x12, 0x34,             // Priority = 0x1234
        0x00, 0x00, 0x00, 0x00, // Reliability parameter
        0x00, 0x00,             // Label length
        0x00, 0x00              // Protocol length
    };
    size_t bufferLength = sizeof(expectedBuffer);

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                                expectedBuffer,
                                                bufferLength,
                                                &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_RELIABLE_UNORDERED, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(0x1234, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(0, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(0, deserializedMessage.protocolLength);
    TEST_ASSERT_NULL(deserializedMessage.pChannelName);
    TEST_ASSERT_NULL(deserializedMessage.pProtocol);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with partial reliable retransmit channel.
 */
void test_dcepDeserializeChannelOpenMessage_PartialReliableRetransmit(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};

    /* Serialized buffer for:
     * - channelType = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT (0x01)
     * - priority = 0x5678
     * - numRetransmissions = 5
     * - channelNameLength = 0
     * - protocolLength = 0
     */
    uint8_t expectedBuffer[] = {
        0x03,                   // DCEP_MESSAGE_DATA_CHANNEL_OPEN
        0x01,                   // DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT
        0x56, 0x78,             // Priority = 0x5678
        0x00, 0x00, 0x00, 0x05, // numRetransmissions = 5
        0x00, 0x00,             // Channel name length = 0
        0x00, 0x00              // Protocol length = 0
    };
    size_t bufferLength = sizeof(expectedBuffer);

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                                expectedBuffer,
                                                bufferLength,
                                                &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(0x5678, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(5, deserializedMessage.numRetransmissions);
    TEST_ASSERT_EQUAL(0, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(0, deserializedMessage.protocolLength);
    TEST_ASSERT_NULL(deserializedMessage.pChannelName);
    TEST_ASSERT_NULL(deserializedMessage.pProtocol);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with partial reliable retransmit unordered channel.
 */
void test_dcepDeserializeChannelOpenMessage_PartialReliableRetransmitUnordered(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};

    /* Serialized buffer for:
     * - Message Type = 0x03
     * - Channel Type = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT | UNORDERED (0x81)
     * - Priority = 0x5678
     * - numRetransmissions = 3
     * - Label Length = 0
     * - Protocol Length = 0
     */
    uint8_t expectedBuffer[] = {
        0x03,                   // DCEP_MESSAGE_DATA_CHANNEL_OPEN
        0x81,                   // Partial Reliable Retransmit | Unordered
        0x56, 0x78,             // Priority = 0x5678
        0x00, 0x00, 0x00, 0x03, // numRetransmissions = 3
        0x00, 0x00,             // Label length
        0x00, 0x00              // Protocol length
    };
    size_t bufferLength = sizeof(expectedBuffer);

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                                expectedBuffer,
                                                bufferLength,
                                                &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(0x5678, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(3, deserializedMessage.numRetransmissions);
    TEST_ASSERT_EQUAL(0, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(0, deserializedMessage.protocolLength);
    TEST_ASSERT_NULL(deserializedMessage.pChannelName);
    TEST_ASSERT_NULL(deserializedMessage.pProtocol);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with partial reliable timed unordered channel.
 */
void test_dcepDeserializeChannelOpenMessage_PartialReliableTimedUnordered(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};

    /* Serialized buffer for:
     * - Message Type = 0x03
     * - Channel Type = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED | UNORDERED (0x82)
     * - Priority = 0x9ABC
     * - maxLifetimeInMilliseconds = 2000
     * - Label Length = 0
     * - Protocol Length = 0
     */
    uint8_t expectedBuffer[] = {
        0x03,                   // DCEP_MESSAGE_DATA_CHANNEL_OPEN
        0x82,                   // Partial Reliable Timed | Unordered
        0x9A, 0xBC,             // Priority = 0x9ABC
        0x00, 0x00, 0x07, 0xD0, // maxLifetimeInMilliseconds = 2000
        0x00, 0x00,             // Label length
        0x00, 0x00              // Protocol length
    };
    size_t bufferLength = sizeof(expectedBuffer);

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                                expectedBuffer,
                                                bufferLength,
                                                &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(0x9ABC, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(2000, deserializedMessage.maxLifetimeInMilliseconds);
    TEST_ASSERT_EQUAL(0, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(0, deserializedMessage.protocolLength);
    TEST_ASSERT_NULL(deserializedMessage.pChannelName);
    TEST_ASSERT_NULL(deserializedMessage.pProtocol);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with partial reliable timed channel.
 */
void test_dcepDeserializeChannelOpenMessage_PartialReliableTimed(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};

    /* Serialized buffer for:
     * - channelType = DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED (0x02)
     * - priority = 0xABCD
     * - maxLifetimeInMilliseconds = 1000 (0x000003E8)
     * - channelNameLength = 0
     * - protocolLength = 0
     */
    uint8_t expectedBuffer[] = {
        0x03,                   // DCEP_MESSAGE_DATA_CHANNEL_OPEN
        0x02,                   // DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED
        0xAB, 0xCD,             // Priority = 0xABCD
        0x00, 0x00, 0x03, 0xE8, // maxLifetimeInMilliseconds = 1000
        0x00, 0x00,             // Channel name length = 0
        0x00, 0x00              // Protocol length = 0
    };
    size_t bufferLength = sizeof(expectedBuffer);

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                                expectedBuffer,
                                                bufferLength,
                                                &deserializedMessage);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(0xABCD, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(1000, deserializedMessage.maxLifetimeInMilliseconds);
    TEST_ASSERT_EQUAL(0, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(0, deserializedMessage.protocolLength);
    TEST_ASSERT_NULL(deserializedMessage.pChannelName);
    TEST_ASSERT_NULL(deserializedMessage.pProtocol);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with empty channel name and protocol.
 */
void test_dcepDeserializeChannelOpenMessage_EmptyNameAndProtocol(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};

    /* Serialized buffer for:
     * - Message Type: DCEP_MESSAGE_DATA_CHANNEL_OPEN (0x03)
     * - Channel Type: DCEP_DATA_CHANNEL_RELIABLE (0x00)
     * - Priority: 0x0000
     * - Reserved/Unused: 0x00000000
     * - Channel Name Length: 0x0000
     * - Protocol Length: 0x0000
     */
    uint8_t expectedBuffer[] = {
        0x03,       // Message Type
        0x00,       // Channel Type (Reliable)
        0x00, 0x00, // Priority
        0x00, 0x00, 0x00, 0x00, // Reserved/unused field
        0x00, 0x00, // Channel name length
        0x00, 0x00  // Protocol length
    };
    size_t bufferLength = sizeof(expectedBuffer);

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                                expectedBuffer,
                                                bufferLength,
                                                &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_RELIABLE, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(0x0000, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(0, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(0, deserializedMessage.protocolLength);
    TEST_ASSERT_NULL(deserializedMessage.pChannelName);
    TEST_ASSERT_NULL(deserializedMessage.pProtocol);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with bad parameters.
 */
void test_dcepDeserializeChannelOpenMessage_BadParams(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};
    uint8_t validMessage[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN, /* Message Type */
        DCEP_DATA_CHANNEL_RELIABLE,     /* Channel Type */
        0x12, 0x34,                     /* Priority */
        0x00, 0x00, 0x00, 0x00,         /* Reliability Parameter */
        0x00, 0x00,                     /* Label Length */
        0x00, 0x00                      /* Protocol Length */
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Test with NULL context. */
    result = Dcep_DeserializeChannelOpenMessage(NULL,
                                               validMessage,
                                               sizeof(validMessage),
                                               &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL message. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               NULL,
                                               sizeof(validMessage),
                                               &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL output structure. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               validMessage,
                                               sizeof(validMessage),
                                               NULL);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with message too short. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               validMessage,
                                               DCEP_HEADER_LENGTH - 1,
                                               &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with wrong message type. */
    validMessage[0] = DCEP_MESSAGE_DATA_CHANNEL_ACK;
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               validMessage,
                                               sizeof(validMessage),
                                               &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with malformed message - insufficient data for channel name.
 */
void test_dcepDeserializeChannelOpenMessage_MalformedMessage_ChannelName(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};
    uint8_t malformedMessage[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN, /* Message Type */
        DCEP_DATA_CHANNEL_RELIABLE,     /* Channel Type */
        0x12, 0x34,                     /* Priority */
        0x00, 0x00, 0x00, 0x00,         /* Reliability Parameter */
        0x00, 0x05,                     /* Label Length = 5 */
        0x00, 0x00,                     /* Protocol Length = 0 */
        0x74, 0x65, 0x73                /* Only 3 bytes of channel name instead of 5 */
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               malformedMessage,
                                               sizeof(malformedMessage),
                                               &deserializedMessage);

    TEST_ASSERT_EQUAL(DCEP_RESULT_MALFORMED_MESSAGE, result);
}

/**
 * @brief Validate Dcep_DeserializeChannelOpenMessage with malformed message - insufficient data for protocol.
 */
void test_dcepDeserializeChannelOpenMessage_MalformedMessage_Protocol(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t deserializedMessage = {0};
    uint8_t malformedMessage[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN, /* Message Type */
        DCEP_DATA_CHANNEL_RELIABLE,     /* Channel Type */
        0x12, 0x34,                     /* Priority */
        0x00, 0x00, 0x00, 0x00,         /* Reliability Parameter */
        0x00, 0x04,                     /* Label Length = 4 */
        0x00, 0x05,                     /* Protocol Length = 5 */
        0x74, 0x65, 0x73, 0x74,         /* Channel name: "test" */
        0x68, 0x74                      /* Only 2 bytes of protocol instead of 5 */
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               malformedMessage,
                                               sizeof(malformedMessage),
                                               &deserializedMessage);

    TEST_ASSERT_EQUAL(DCEP_RESULT_MALFORMED_MESSAGE, result);
}

/* ==============================  Test Cases for Message Type Detection ============================== */

/**
 * @brief Validate Dcep_GetMessageType happy path with channel open message.
 */
void test_dcepGetMessageType_ChannelOpen(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepMessageType_t messageType;
    uint8_t channelOpenMessage[] = {
        DCEP_MESSAGE_DATA_CHANNEL_OPEN, /* Message Type */
        0x00,                           /* Some data */
        0x00, 0x00
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_GetMessageType(&ctx,
                                channelOpenMessage,
                                sizeof(channelOpenMessage),
                                &messageType);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, messageType);
}

/**
 * @brief Validate Dcep_GetMessageType happy path with channel ack message.
 */
void test_dcepGetMessageType_ChannelAck(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepMessageType_t messageType;
    uint8_t channelAckMessage[] = {
        DCEP_MESSAGE_DATA_CHANNEL_ACK   /* Message Type */
    };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_GetMessageType(&ctx,
                                channelAckMessage,
                                sizeof(channelAckMessage),
                                &messageType);

    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_ACK, messageType);
}

/**
 * @brief Validate Dcep_GetMessageType with bad parameters.
 */
void test_dcepGetMessageType_BadParams(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepMessageType_t messageType;
    uint8_t validMessage[] = { DCEP_MESSAGE_DATA_CHANNEL_OPEN };

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Test with NULL context. */
    result = Dcep_GetMessageType(NULL,
                                validMessage,
                                sizeof(validMessage),
                                &messageType);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL message. */
    result = Dcep_GetMessageType(&ctx,
                                NULL,
                                sizeof(validMessage),
                                &messageType);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with NULL message type output. */
    result = Dcep_GetMessageType(&ctx,
                                validMessage,
                                sizeof(validMessage),
                                NULL);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);

    /* Test with zero message length. */
    result = Dcep_GetMessageType(&ctx,
                                validMessage,
                                0,
                                &messageType);
    TEST_ASSERT_EQUAL(DCEP_RESULT_BAD_PARAM, result);
}

/**
 * @brief Validate Dcep_GetMessageType with malformed message - invalid message type.
 */
void test_dcepGetMessageType_MalformedMessage(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepMessageType_t messageType;
    uint8_t invalidMessage[] = { 0xFF }; /* Invalid message type */

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_GetMessageType(&ctx,
                                invalidMessage,
                                sizeof(invalidMessage),
                                &messageType);

    TEST_ASSERT_EQUAL(DCEP_RESULT_MALFORMED_MESSAGE, result);
}

/* ==============================  Test Cases for Round-trip Serialization/Deserialization ============================== */

/**
 * @brief Validate round-trip serialization and deserialization with all channel types.
 */
void test_dcepRoundTrip_ReliableOnly(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t original = {0}, deserialized = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    original.channelType = DCEP_DATA_CHANNEL_RELIABLE;
    original.priority = 0x1234;

    result = Dcep_SerializeChannelOpenMessage(&ctx, &original, testBuffer, &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    result = Dcep_DeserializeChannelOpenMessage(&ctx, testBuffer, bufferLength, &deserialized);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    TEST_ASSERT_EQUAL(original.channelType, deserialized.channelType);
    TEST_ASSERT_EQUAL(original.priority, deserialized.priority);
    TEST_ASSERT_EQUAL(0, deserialized.channelNameLength);
    TEST_ASSERT_EQUAL(0, deserialized.protocolLength);
    TEST_ASSERT_NULL(deserialized.pChannelName);
    TEST_ASSERT_NULL(deserialized.pProtocol);
}
