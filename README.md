## amazon-kinesis-video-streams-dcep

The goal of the [Data Channel Establishment Protocol (DCEP)](https://datatracker.ietf.org/doc/html/rfc8832) library is to provide DCEP Serializer and Deserializer functionalities.

## What is DCEP?

DCEP is a simple protocol for establishing symmetric data channels between WebRTC peers. 
It uses a two-way handshake and allows sending of user data without waiting for the handshake to complete.
The peer that initiates opening a data channel selects a stream identifier for which the corresponding incoming 
and outgoing streams are unused and sends a `DATA_CHANNEL_OPEN` message on the outgoing stream. 
The peer responds with a `DATA_CHANNEL_ACK` message on its corresponding outgoing stream. 
Then the data channel is open. DCEP messages are sent on the same stream as the 
user messages belonging to the data channel. 
The demultiplexing is based on the SCTP Payload Protocol Identifier (PPID), since DCEP uses a specific PPID.

### DCEP Message Formats

#### DATA_CHANNEL_OPEN Message

This message is initially sent using the data channel on the stream
used for user messages.

```
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Message Type |  Channel Type |            Priority           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                    Reliability Parameter                      |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |         Label Length          |       Protocol Length         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    \                                                               /
    |                             Label                             |
    /                                                               \
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    \                                                               /
    |                            Protocol                           |
    /                                                               \
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
```

#### DATA_CHANNEL_ACK Message

This message is sent in response to a DATA_CHANNEL_OPEN_RESPONSE
message.  It is sent on the stream used for user messages using the
data channel.  Reception of this message tells the opener that the
data channel setup handshake is complete.

```
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Message Type |
    +-+-+-+-+-+-+-+-+
```

## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

This project is licensed under the Apache-2.0 License.

