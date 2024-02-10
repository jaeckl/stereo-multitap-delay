# Use a base image that GitHub Actions supports (e.g., ubuntu:20.04)
FROM ubuntu:22.04

# Avoid user interaction when installing packages
ARG DEBIAN_FRONTEND=noninteractive

# Install system dependencies for JUCE and other build essentials
RUN apt-get update && apt-get install -y \
    clang \
    build-essential \
    cmake \
    git \
    libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev \
    libfreetype6-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev \
    libgsl-dev \
    && rm -rf /var/lib/apt/lists/*
# Install clang compiler

# Clone the JUCE repository
RUN git clone --depth 1 https://github.com/juce-framework/JUCE.git

# Build and install JUCE
RUN cd JUCE && \
    cmake -B cmake-build-install -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ && \
    cmake --build cmake-build-install --target install

# Set the working directory (optional, for when you run the container locally)
WORKDIR /workspace

# Any additional setup you might have
