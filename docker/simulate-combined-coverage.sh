#!/bin/bash

# Simulate combined coverage by running tests with both endianness flags
# This demonstrates what the combined Docker approach would achieve

set -e

echo "🧪 Simulating combined endianness coverage..."

# Clean previous builds
rm -rf build-le build-be coverage-combined

# Create directories
mkdir -p build-le build-be coverage-combined

echo "📱 Building little-endian test..."
cmake -S test/unit-test -B build-le/ -G "Unix Makefiles" \
    -DBUILD_CLONE_SUBMODULES=ON \
    -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG -DFORCE_LITTLE_ENDIAN'

make -C build-le all
cd build-le && ctest --output-on-failure && make coverage
cd ..

echo "🖥️  Building big-endian test..."
cmake -S test/unit-test -B build-be/ -G "Unix Makefiles" \
    -DBUILD_CLONE_SUBMODULES=ON \
    -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG -DFORCE_BIG_ENDIAN'

make -C build-be all
cd build-be && ctest --output-on-failure && make coverage
cd ..

echo "📊 Combining coverage reports..."
lcov --add-tracefile build-le/coverage.info \
     --add-tracefile build-be/coverage.info \
     --output-file coverage-combined/combined_coverage.info \
     --rc branch_coverage=1

# Generate HTML report
genhtml coverage-combined/combined_coverage.info \
        --output-directory coverage-combined \
        --rc branch_coverage=1 \
        --title "Combined Endianness Coverage Report"

echo "✅ Combined coverage simulation complete!"
echo "📊 Coverage report: coverage-combined/index.html"

# Show summary
echo ""
echo "📈 Final Coverage Summary:"
lcov --summary coverage-combined/combined_coverage.info