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
    TEST_ASSERT_EQUAL(DCEP_HEADER_LENGTH + sizeof(expectedChannelName) - 1 + sizeof(expectedProtocol) - 1,
                     bufferLength);

    /* Verify header fields. */
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, testBuffer[0]);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_RELIABLE, testBuffer[1]);
    
    /* Verify channel name is present. */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedChannelName,
                                 &testBuffer[DCEP_HEADER_LENGTH],
                                 sizeof(expectedChannelName) - 1);
    
    /* Verify protocol is present. */
    TEST_ASSERT_EQUAL_UINT8_ARRAY(expectedProtocol,
                                 &testBuffer[DCEP_HEADER_LENGTH + sizeof(expectedChannelName) - 1],
                                 sizeof(expectedProtocol) - 1);
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
    TEST_ASSERT_EQUAL(DCEP_HEADER_LENGTH, bufferLength);

    /* Verify header fields. */
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, testBuffer[0]);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT, testBuffer[1]);
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
    TEST_ASSERT_EQUAL(DCEP_HEADER_LENGTH, bufferLength);

    /* Verify header fields. */
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, testBuffer[0]);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED, testBuffer[1]);
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
    TEST_ASSERT_EQUAL(DCEP_HEADER_LENGTH, bufferLength);

    /* Verify header fields. */
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, testBuffer[0]);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_RELIABLE_UNORDERED, testBuffer[1]);
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
    TEST_ASSERT_EQUAL(DCEP_HEADER_LENGTH, bufferLength);

    /* Verify header fields. */
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, testBuffer[0]);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED, testBuffer[1]);
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
    TEST_ASSERT_EQUAL(DCEP_HEADER_LENGTH, bufferLength);

    /* Verify header fields. */
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, testBuffer[0]);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED, testBuffer[1]);
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
    TEST_ASSERT_EQUAL(DCEP_HEADER_LENGTH, bufferLength);

    /* Verify header fields. */
    TEST_ASSERT_EQUAL(DCEP_MESSAGE_DATA_CHANNEL_OPEN, testBuffer[0]);
    TEST_ASSERT_EQUAL(DCEP_DATA_CHANNEL_RELIABLE, testBuffer[1]);
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
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    DcepChannelOpenMessage_t deserializedMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;
    uint8_t expectedChannelName[] = "test-channel";
    uint8_t expectedProtocol[] = "test-protocol";

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* First serialize a message. */
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

    /* Now deserialize it. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               testBuffer,
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
 * @brief Validate Dcep_DeserializeChannelOpenMessage with partial reliable retransmit channel.
 */
void test_dcepDeserializeChannelOpenMessage_PartialReliableRetransmit(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    DcepChannelOpenMessage_t deserializedMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* First serialize a message. */
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

    /* Now deserialize it. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               testBuffer,
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
 * @brief Validate Dcep_DeserializeChannelOpenMessage with partial reliable timed channel.
 */
void test_dcepDeserializeChannelOpenMessage_PartialReliableTimed(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    DcepChannelOpenMessage_t deserializedMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* First serialize a message. */
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

    /* Now deserialize it. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               testBuffer,
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
    DcepChannelOpenMessage_t channelOpenMessage = {0};
    DcepChannelOpenMessage_t deserializedMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* First serialize a message with empty name and protocol. */
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

    /* Now deserialize it. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               testBuffer,
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
void test_dcepRoundTrip_AllChannelTypes(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t originalMessage = {0};
    DcepChannelOpenMessage_t deserializedMessage = {0};
    size_t bufferLength;
    uint8_t channelName[] = "test-channel";
    uint8_t protocol[] = "test-protocol";
    DcepChannelType_t channelTypes[] = {
        DCEP_DATA_CHANNEL_RELIABLE,
        DCEP_DATA_CHANNEL_RELIABLE_UNORDERED,
        DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT,
        DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED,
        DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED,
        DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED
    };
    size_t numChannelTypes = sizeof(channelTypes) / sizeof(channelTypes[0]);

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Test each channel type. */
    for (size_t i = 0; i < numChannelTypes; i++)
    {
        memset(&originalMessage, 0, sizeof(originalMessage));
        memset(&deserializedMessage, 0, sizeof(deserializedMessage));
        bufferLength = MAX_BUFFER_LENGTH;

        /* Setup original message. */
        originalMessage.channelType = channelTypes[i];
        originalMessage.priority = 0x1000 + (uint16_t)i;
        originalMessage.pChannelName = channelName;
        originalMessage.channelNameLength = sizeof(channelName) - 1;
        originalMessage.pProtocol = protocol;
        originalMessage.protocolLength = sizeof(protocol) - 1;

        if ((channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT) ||
            (channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED))
        {
            originalMessage.numRetransmissions = 3 + (uint32_t)i;
        }
        else if ((channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED) ||
                 (channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED))
        {
            originalMessage.maxLifetimeInMilliseconds = 1000 + (uint32_t)i * 100;
        }

        /* Serialize. */
        result = Dcep_SerializeChannelOpenMessage(&ctx,
                                                 &originalMessage,
                                                 testBuffer,
                                                 &bufferLength);
        TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

        /* Deserialize. */
        result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                                   testBuffer,
                                                   bufferLength,
                                                   &deserializedMessage);
        TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

        /* Verify all fields match. */
        TEST_ASSERT_EQUAL(originalMessage.channelType, deserializedMessage.channelType);
        TEST_ASSERT_EQUAL(originalMessage.priority, deserializedMessage.priority);
        TEST_ASSERT_EQUAL(originalMessage.channelNameLength, deserializedMessage.channelNameLength);
        TEST_ASSERT_EQUAL(originalMessage.protocolLength, deserializedMessage.protocolLength);

        if ((channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT) ||
            (channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT_UNORDERED))
        {
            TEST_ASSERT_EQUAL(originalMessage.numRetransmissions, deserializedMessage.numRetransmissions);
        }
        else if ((channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED) ||
                 (channelTypes[i] == DCEP_DATA_CHANNEL_PARTIAL_RELIABLE_TIMED_UNORDERED))
        {
            TEST_ASSERT_EQUAL(originalMessage.maxLifetimeInMilliseconds, deserializedMessage.maxLifetimeInMilliseconds);
        }

        TEST_ASSERT_EQUAL_UINT8_ARRAY(originalMessage.pChannelName,
                                     deserializedMessage.pChannelName,
                                     originalMessage.channelNameLength);
        TEST_ASSERT_EQUAL_UINT8_ARRAY(originalMessage.pProtocol,
                                     deserializedMessage.pProtocol,
                                     originalMessage.protocolLength);
    }
}

/**
 * @brief Validate round-trip with maximum size channel name and protocol.
 */
void test_dcepRoundTrip_MaximumSizeData(void)
{
    DcepResult_t result;
    DcepContext_t ctx;
    DcepChannelOpenMessage_t originalMessage = {0};
    DcepChannelOpenMessage_t deserializedMessage = {0};
    size_t bufferLength = MAX_BUFFER_LENGTH;

    /* Create maximum size channel name and protocol. */
    uint8_t maxChannelName[255];
    uint8_t maxProtocol[255];
    
    /* Fill with pattern data. */
    for (size_t i = 0; i < sizeof(maxChannelName); i++)
    {
        maxChannelName[i] = (uint8_t)('A' + (i % 26));
        maxProtocol[i] = (uint8_t)('a' + (i % 26));
    }

    result = Dcep_Init(&ctx);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Setup original message. */
    originalMessage.channelType = DCEP_DATA_CHANNEL_RELIABLE;
    originalMessage.priority = 0xFFFF;
    originalMessage.pChannelName = maxChannelName;
    originalMessage.channelNameLength = sizeof(maxChannelName);
    originalMessage.pProtocol = maxProtocol;
    originalMessage.protocolLength = sizeof(maxProtocol);

    /* Serialize. */
    result = Dcep_SerializeChannelOpenMessage(&ctx,
                                             &originalMessage,
                                             testBuffer,
                                             &bufferLength);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Deserialize. */
    result = Dcep_DeserializeChannelOpenMessage(&ctx,
                                               testBuffer,
                                               bufferLength,
                                               &deserializedMessage);
    TEST_ASSERT_EQUAL(DCEP_RESULT_OK, result);

    /* Verify all fields match. */
    TEST_ASSERT_EQUAL(originalMessage.channelType, deserializedMessage.channelType);
    TEST_ASSERT_EQUAL(originalMessage.priority, deserializedMessage.priority);
    TEST_ASSERT_EQUAL(originalMessage.channelNameLength, deserializedMessage.channelNameLength);
    TEST_ASSERT_EQUAL(originalMessage.protocolLength, deserializedMessage.protocolLength);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(originalMessage.pChannelName,
                                 deserializedMessage.pChannelName,
                                 originalMessage.channelNameLength);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(originalMessage.pProtocol,
                                 deserializedMessage.pProtocol,
                                 originalMessage.protocolLength);
}

/*-----------------------------------------------------------*/
