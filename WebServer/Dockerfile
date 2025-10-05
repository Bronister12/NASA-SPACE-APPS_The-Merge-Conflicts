# --- Stage 1: The Build Environment ---
# This stage acts as a temporary workshop to compile your C++ code.
FROM debian:bookworm-slim AS builder

# Define the version of CMake to install
ARG CMAKE_VERSION=3.29.3

# Install build tools and dependencies for vcpkg
RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
    build-essential \
    git \
    ninja-build \
    curl \
    zip \
    unzip \
    tar \
    ca-certificates \
    pkg-config \
    linux-libc-dev && \
    rm -rf /var/lib/apt/lists/*

# Install a modern version of CMake
RUN curl -sSL https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh -o /tmp/cmake.sh && \
    chmod +x /tmp/cmake.sh && \
    /tmp/cmake.sh --skip-license --prefix=/usr/local && \
    rm /tmp/cmake.sh

# Set up vcpkg
WORKDIR /app
ENV VCPKG_ROOT=/app/vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

# Copy your project's source code into the workshop
COPY . .

# Configure and compile your 'Rocket' application in Release mode
RUN cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake && \
    cmake --build build


# --- Stage 2: The Final Runtime Image ---
# This stage builds the final, lightweight container for your application.
FROM debian:bookworm-slim

# Install only the essential runtime libraries.
RUN apt-get update && apt-get install -y --no-install-recommends \
    libssl3 \
    ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory for the application
WORKDIR /app

# Copy ONLY the compiled program from the build stage
COPY --from=builder /app/build/Rocket .

# Copy your 'public' directory, which is required by main.cpp
COPY public ./public

# Inform Docker that the container listens on port 8080
EXPOSE 8080

# The command to run your web server when the container starts
CMD ["./Rocket"]