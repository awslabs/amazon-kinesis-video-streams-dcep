#!/bin/bash

set -e

echo "Building and running endianness tests..."

# Create output directories
mkdir -p build-le build-be coverage-combined

# Run little-endian tests
echo "Running little-endian tests (x86_64)..."
docker build -f docker/little-endian/Dockerfile -t dcep-test-le .
docker run --rm -v "$(pwd)/build-le:/workspace/build" dcep-test-le

# Run big-endian tests (requires Docker Desktop with experimental features or buildx)
echo "Running big-endian tests (s390x)..."
docker build -f docker/big-endian/Dockerfile -t dcep-test-be --platform linux/s390x .
docker run --rm --platform linux/s390x -v "$(pwd)/build-be:/workspace/build" dcep-test-be

# Combine coverage reports
echo "Combining coverage reports..."
if command -v lcov >/dev/null 2>&1; then
    # Combine the coverage.info files from both architectures
    lcov --add-tracefile build-le/coverage.info \
         --add-tracefile build-be/coverage.info \
         --output-file coverage-combined/combined_coverage.info \
         --rc branch_coverage=1
    
    # Generate HTML report
    genhtml coverage-combined/combined_coverage.info \
            --output-directory coverage-combined \
            --rc branch_coverage=1
    
    echo "Combined coverage report generated at: coverage-combined/index.html"
else
    echo "WARNING: lcov not found. Install lcov to generate combined coverage report."
    echo "   Individual coverage reports available in build-le/ and build-be/"
fi

echo "All endianness tests completed!"
echo "Combined coverage: coverage-combined/index.html"
