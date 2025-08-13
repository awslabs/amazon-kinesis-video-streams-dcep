# Docker Endianness Testing

This directory contains Docker configurations for testing endianness functionality on different architectures.

## Files

- `little-endian/Dockerfile` - x86_64 (little-endian) test environment
- `big-endian/Dockerfile` - s390x (big-endian) test environment  
- `docker-compose.yml` - Orchestrates both test environments

## Usage

### Option 1: Manual Docker Commands
```bash
# Little-endian tests
docker build -f docker/little-endian/Dockerfile -t dcep-test-le .
docker run --rm dcep-test-le

# Big-endian tests (requires multi-arch support)
docker build -f docker/big-endian/Dockerfile -t dcep-test-be --platform linux/s390x .
docker run --rm --platform linux/s390x dcep-test-be
```

### Option 2: Docker Compose
```bash
cd docker
docker-compose up --build
```

## Requirements

- Docker Desktop with experimental features enabled
- Or Docker with buildx plugin for multi-architecture support

## CI Integration

Endianness tests are automatically run in GitHub CI via the workflow in `.github/workflows/ci.yml`.