FROM s390x/ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    lcov \
    ruby \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Copy source code
COPY . .

# Initialize submodules
RUN git submodule update --init --recursive test/CMock

# Build and run tests
RUN cmake -S test/unit-test -B build-docker/ -G "Unix Makefiles" \
    -DBUILD_CLONE_SUBMODULES=ON \
    -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG'

RUN make -C build-docker all

# Run tests
CMD ["sh", "-c", "echo 'Starting Big Endian Tests (s390x)...' && echo '#include <stdio.h>\n#include <stdint.h>\nint main() { uint32_t test = 0x12345678; uint8_t *bytes = (uint8_t*)&test; printf(\"System: %s ENDIAN\\n\", bytes[0] == 0x78 ? \"LITTLE\" : \"BIG\"); return 0; }' > /tmp/check.c && gcc -o /tmp/check /tmp/check.c && /tmp/check && cd build-docker && echo '📋 Running CTest with detailed output...' && ctest --output-on-failure --verbose && echo 'Big Endian Tests PASSED - Network byte order handling verified on s390x'"]
