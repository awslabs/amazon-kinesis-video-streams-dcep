# Docker Endianness Testing

This directory contains Docker configurations for testing endianness functionality on different architectures.

## Files

- `little-endian/Dockerfile` - x86_64 (little-endian) test environment
- `big-endian/Dockerfile` - s390x (big-endian) test environment  
- `docker-compose.yml` - Orchestrates both test environments
- `run-tests.sh` - Script to run both tests sequentially

## Usage

### Option 1: Run Script (Recommended)
```bash
./docker/run-tests.sh
```

### Option 2: Manual Docker Commands
```bash
# Little-endian tests
docker build -f docker/little-endian/Dockerfile -t dcep-test-le .
docker run --rm dcep-test-le

# Big-endian tests (requires multi-arch support)
docker build -f docker/big-endian/Dockerfile -t dcep-test-be --platform linux/s390x .
docker run --rm --platform linux/s390x dcep-test-be
```

### Option 3: Docker Compose
```bash
cd docker
docker-compose up --build
```

## Requirements

- Docker Desktop with experimental features enabled
- Or Docker with buildx plugin for multi-architecture support

## Output

- `build-le/` - Little-endian build artifacts
- `build-be/` - Big-endian build artifacts  
- `coverage-le/` - Little-endian coverage reports
- `coverage-be/` - Big-endian coverage reports

## CI Integration

Add to `.github/workflows/ci.yml`:
```yaml
- name: Test Endianness
  run: ./docker/run-tests.sh
```