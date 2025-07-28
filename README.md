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

## Building Unit Tests

### Platform Prerequisites

- For running unit tests:
    - C99 compiler like gcc.
    - CMake 3.13.0 or later.
    - Ruby 2.0.0 or later (It is required for the CMock test framework that we
      use).
- For running the coverage target, gcov and lcov are required.

### Checkout CMock Submodule

By default, the submodules in this repository are configured with `update=none`
in [.gitmodules](./.gitmodules) to avoid increasing clone time and disk space
usage of other repositories.

To build unit tests, the submodule dependency of CMock is required. Use the
following command to clone the submodule:

```sh
git submodule update --checkout --init --recursive test/CMock
```

### Steps to Build Unit Tests

1. Run the following command from the root directory of this repository to
   generate Makefiles:

   ```sh
   cmake -S test/unit-test -B build/ -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_CLONE_SUBMODULES=ON \
    -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG'
   ```

2. Run the following command to build the unit tests:
   ```sh
   cd build && make
   ```

### Steps to Generate Code Coverage Report and Run Unit Tests

1. Build Unit Tests as described in [Steps to Build Unit Tests](#steps-to-build-unit-tests).
2. Generate coverage report in the `build/coverage` folder:

   ```
   cd build && make coverage
   ```

### Script to Run Unit Test and Generate Code Coverage Report

```sh
 cmake -S test/unit-test -B build/ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_CLONE_SUBMODULES=ON -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG -DLIBRARY_LOG_LEVEL=LOG_DEBUG'
 cd build && make coverage
```

## Security

See [CONTRIBUTING](CONTRIBUTING.md#security-issue-notifications) for more information.

## License

This project is licensed under the Apache-2.0 License.

