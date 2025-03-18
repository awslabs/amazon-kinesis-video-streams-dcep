#ifndef DCEP_API_H
#define DCEP_API_H

/* Data types includes. */
#include "dcep_data_types.h"

/*-----------------------------------------------------------*/

DcepResult_t Dcep_Init( DcepContext_t * pCtx );

DcepResult_t Dcep_SerializeChannelOpenMessage( DcepContext_t * pCtx,
                                               const DcepChannelOpenMessage_t * pChannelOpenMessage,
                                               uint8_t * pBuffer,
                                               size_t * pBufferLength );

DcepResult_t Dcep_SerializeChannelAckMessage( DcepContext_t * pCtx,
                                                uint8_t * pBuffer,
                                                size_t * pBufferLength );

DcepResult_t Dcep_DeserializeChannelOpenMessage( DcepContext_t * pCtx,
                                                 const uint8_t * pDcepMessage,
                                                 size_t dcepMessageLength,
                                                 DcepChannelOpenMessage_t * pChannelOpenMessage );

DcepResult_t Dcep_GetMessageType( DcepContext_t * pCtx,
                                  const uint8_t * pDcepMessage,
                                  size_t dcepMessageLength,
                                  DcepMessageType_t * pDcepMessageType );

/*-----------------------------------------------------------*/

#endif /* DCEP_API_H */
