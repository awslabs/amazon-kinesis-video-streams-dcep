#!/bin/bash

# Script to combine coverage reports from little-endian and big-endian test runs
# This demonstrates how to merge coverage from both architectures

set -e

echo "Combining coverage reports from both architectures..."

# Check if coverage files exist
if [ ! -f "build-le/coverage.info" ]; then
    echo "ERROR: Little-endian coverage file not found: build-le/coverage.info"
    exit 1
fi

if [ ! -f "build-be/coverage.info" ]; then
    echo "ERROR: Big-endian coverage file not found: build-be/coverage.info"
    exit 1
fi

# Create output directory
mkdir -p coverage-combined

# Combine coverage reports using lcov
echo "Merging coverage data..."
lcov --add-tracefile build-le/coverage.info \
     --add-tracefile build-be/coverage.info \
     --output-file coverage-combined/combined_coverage.info \
     --rc branch_coverage=1

# Generate HTML report
echo "Generating HTML report..."
genhtml coverage-combined/combined_coverage.info \
        --output-directory coverage-combined \
        --rc branch_coverage=1 \
        --title "Combined Endianness Coverage Report"

echo "Combined coverage report generated!"
echo "Open: coverage-combined/index.html"

# Display summary
echo ""
echo "Coverage Summary:"
lcov --summary coverage-combined/combined_coverage.info
