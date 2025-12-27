# CSC Latin America 2026 - HEP Computing Environment
# Base: Ubuntu 22.04 LTS for compatibility with HEP tools
FROM ubuntu:22.04

# Prevent interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

# Install essential build tools and dependencies
RUN apt-get update && apt-get install -y \
    # Build essentials
    build-essential \
    gcc-12 \
    g++-12 \
    gfortran \
    cmake \
    ninja-build \
    pkg-config \
    # Debugging tools
    gdb \
    valgrind \
    linux-tools-generic \
    # Version control
    git \
    git-lfs \
    # Python
    python3 \
    python3-pip \
    python3-dev \
    python3-venv \
    # Libraries for ROOT
    libx11-dev \
    libxpm-dev \
    libxft-dev \
    libxext-dev \
    libssl-dev \
    libpcre3-dev \
    libglu1-mesa-dev \
    libglew-dev \
    libftgl-dev \
    libfftw3-dev \
    libcfitsio-dev \
    libgraphviz-dev \
    libavahi-compat-libdnssd-dev \
    libldap2-dev \
    libxml2-dev \
    libkrb5-dev \
    libgsl-dev \
    libtbb-dev \
    # Additional utilities
    wget \
    curl \
    vim \
    nano \
    htop \
    tree \
    zip \
    unzip \
    ca-certificates \
    locales \
    sudo \
    # For perf tools
    linux-perf \
    && rm -rf /var/lib/apt/lists/*

# Set up locale
RUN locale-gen en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

# Set gcc-12 as default
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100

# Install ROOT (v6.30 - latest stable)
ARG ROOT_VERSION=6.30.06
RUN mkdir -p /tmp/root-build && cd /tmp/root-build \
    && wget -q https://root.cern/download/root_v${ROOT_VERSION}.Linux-ubuntu22.04-x86_64-gcc11.4.tar.gz \
    && tar -xzf root_v${ROOT_VERSION}.Linux-ubuntu22.04-x86_64-gcc11.4.tar.gz -C /opt \
    && mv /opt/root /opt/root-${ROOT_VERSION} \
    && ln -s /opt/root-${ROOT_VERSION} /opt/root \
    && rm -rf /tmp/root-build

# Set ROOT environment
ENV ROOTSYS=/opt/root
ENV PATH=${ROOTSYS}/bin:${PATH}
ENV LD_LIBRARY_PATH=${ROOTSYS}/lib:${LD_LIBRARY_PATH}
ENV PYTHONPATH=${ROOTSYS}/lib:${PYTHONPATH}

# Install Catch2 (v3.x for modern C++ testing)
RUN cd /tmp \
    && git clone --depth 1 --branch v3.5.2 https://github.com/catchorg/Catch2.git \
    && cd Catch2 \
    && cmake -B build -S . -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build build --target install -j$(nproc) \
    && rm -rf /tmp/Catch2

# Install Google Benchmark
RUN cd /tmp \
    && git clone --depth 1 --branch v1.8.3 https://github.com/google/benchmark.git \
    && cd benchmark \
    && cmake -B build -S . \
        -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON \
        -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build build --target install -j$(nproc) \
    && rm -rf /tmp/benchmark

# Install oneTBB (Threading Building Blocks)
RUN cd /tmp \
    && git clone --depth 1 --branch v2021.11.0 https://github.com/oneapi-src/oneTBB.git \
    && cd oneTBB \
    && cmake -B build -S . \
        -DTBB_TEST=OFF \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
    && cmake --build build --target install -j$(nproc) \
    && rm -rf /tmp/oneTBB

# Install Python packages
RUN pip3 install --no-cache-dir \
    numpy \
    pandas \
    matplotlib \
    scipy \
    uproot \
    awkward \
    vector \
    hist \
    mplhep \
    pytest \
    pytest-cov \
    pytest-benchmark \
    jupyter \
    jupyterlab \
    ipykernel \
    black \
    flake8 \
    pylint \
    mypy

# Install FlameGraph tools
RUN cd /opt \
    && git clone --depth 1 https://github.com/brendangregg/FlameGraph.git \
    && chmod +x /opt/FlameGraph/*.pl
ENV PATH=/opt/FlameGraph:${PATH}

# Create vscode user (for devcontainer compatibility)
RUN useradd -ms /bin/bash vscode \
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Set up perf permissions (for non-root profiling)
RUN echo 'kernel.perf_event_paranoid=1' >> /etc/sysctl.conf || true

# Create workspace directory
RUN mkdir -p /workspace && chown vscode:vscode /workspace

# Switch to vscode user
USER vscode
WORKDIR /workspace

# Add ROOT setup to bashrc
RUN echo 'source /opt/root/bin/thisroot.sh' >> ~/.bashrc \
    && echo 'export PATH=/opt/FlameGraph:$PATH' >> ~/.bashrc

# Default command
CMD ["/bin/bash"]
