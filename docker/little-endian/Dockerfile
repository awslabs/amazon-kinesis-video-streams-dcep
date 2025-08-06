FROM ubuntu:22.04

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

# Build and run tests
RUN cmake -S test/unit-test -B build/ -G "Unix Makefiles" \
    -DBUILD_CLONE_SUBMODULES=ON \
    -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG'

RUN make -C build all

# Run tests and generate coverage
CMD ["sh", "-c", "cd build && ctest --output-on-failure && make coverage"]