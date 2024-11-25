# Setup:
# docker build --progress plain -t ivm-apps .
# docker rm -f ivm-apps
# docker run -t --name ivm-apps ivm-apps

FROM debian:12.7-slim@sha256:36e591f228bb9b99348f584e83f16e012c33ba5cad44ef5981a1d7c0a93eca22 AS ivm-apps-deps

# Sanity
SHELL ["/bin/bash", "-c"]

# Install dependencies for dotnet
RUN apt update
RUN apt -y install wget

# Install dotnet signing keys and repo
ENV ROOT_DIR="/root"
WORKDIR ${ROOT_DIR}
RUN wget https://packages.microsoft.com/config/debian/12/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
RUN dpkg -i packages-microsoft-prod.deb
RUN rm packages-microsoft-prod.deb

# ivm-apps build dependencies
RUN apt update
RUN apt -y install\
    autoconf=2.71-3\
    bsdmainutils=12.1.8\
    build-essential=12.9\
    check=0.15.2-2+b1\
    dotnet-sdk-7.0=7.0.410-1\
    file=1:5.44-3\
    gawk=1:5.2.1-2\
    git=1:2.39.5-0+deb12u1\
    imagemagick=8:6.9.11.60+dfsg-1.6+deb12u2\
    libpng-dev=1.6.39-2\
    pkgconf=1.8.1-1\
    texlive-full=2022.20230122-3\
    unzip=6.0-28\
    xxd=2:9.0.1378-2

FROM ivm-apps-deps AS ivm-apps-build

# Install ivm-apps
ENV IVM_DIR="${ROOT_DIR}/immortalvm"
WORKDIR ${IVM_DIR}
RUN echo uncache7
# (Merge changes into immortalvm/ivm-apps before changing this back)
RUN git clone https://github.com/trgwii/ivm-apps
ENV IVM_APPS_DIR="${IVM_DIR}/ivm-apps"
WORKDIR ${IVM_APPS_DIR}

# Run ivm-apps install script to install libraries
RUN ./install-libs.sh -y

# 3.4 is needed by ROAEshell
ENV IVM12_BIN="${IVM_DIR}/ivmc/3.4/gcc-12.2.0-ivm64-bin/bin"
ENV IVM10_BIN="${IVM_DIR}/ivmc/2.0/gcc-10.2.0-ivm64-bin/bin"
ENV PATH="$PATH:${IVM10_BIN}"

# Build ivm-implementations
ENV IVM_IMPLEMENTATIONS_DIR="${IVM_DIR}/ivm-implementations"
WORKDIR ${IVM_IMPLEMENTATIONS_DIR}
RUN ./build.sh
ENV PATH="$PATH:${IVM_IMPLEMENTATIONS_DIR}/Command/bin/Release/net7.0/linux-x64"

# Build ivm-implementations/OtherMachines/vm.c
WORKDIR ${IVM_IMPLEMENTATIONS_DIR}/OtherMachines
RUN make
ENV PATH="$PATH:${IVM_IMPLEMENTATIONS_DIR}/OtherMachines"

# Build yafie
ENV IVM_YAFIE_DIR="${IVM_DIR}/yet-another-ivm-emulator"
WORKDIR ${IVM_YAFIE_DIR}
RUN make
ENV PATH="$PATH:${IVM_YAFIE_DIR}"

# Patch testdata with WAV file
ENV IVM_TESTDATA_DIR="${IVM_DIR}/testdata"
WORKDIR ${IVM_TESTDATA_DIR}
RUN ./create-testdata.sh

# Build ivm-apps
WORKDIR ${IVM_APPS_DIR}
ENV IVM_ASM_DIR="${IVM_DIR}/ivm-v0.37_linux-x64"
RUN bash <<EOF
. ./setup.sh "${IVM10_BIN}" "${IVM_ASM_DIR}"
DOTNET_SYSTEM_GLOBALIZATION_INVARIANT=1 ./build.sh -t ivm -b lib
EOF

RUN bash <<EOF
. ./setup.sh "${IVM10_BIN}" "${IVM_ASM_DIR}"
DOTNET_SYSTEM_GLOBALIZATION_INVARIANT=1 ./build.sh -t ivm -b app
EOF

FROM ivm-apps-build AS ivm-apps

# Install Zig
ENV ZIG_VERSION="zig-linux-x86_64-0.14.0-dev.2253+3a6a8b8aa"
WORKDIR ${ROOT_DIR}/zig
RUN wget -q https://ziglang.org/builds/${ZIG_VERSION}.tar.xz
RUN tar -xf ${ZIG_VERSION}.tar.xz
ENV ZIG_DIR="${ROOT_DIR}/zig/${ZIG_VERSION}"
ENV PATH="$PATH:${ZIG_DIR}"

# Install ivm-zig and build
WORKDIR ${ROOT_DIR}
RUN git clone https://github.com/trgwii/ivm-zig
ENV IVM_ZIG_DIR="${ROOT_DIR}/ivm-zig"
WORKDIR ${IVM_ZIG_DIR}
RUN zig build
ENV PATH="$PATH:${IVM_ZIG_DIR}/zig-out/bin"

# Install ROAEshell
ENV ROAESHELL_DIR="${IVM_DIR}/ROAEshell"
WORKDIR ${ROAESHELL_DIR}
RUN PATH="${IVM12_BIN}:$PATH" ./build.sh -c ivm
WORKDIR ${ROAESHELL_DIR}/run-ivm64
RUN PATH="${IVM12_BIN}:$PATH" ivm64-as roaeshell --bin roaeshell.b --sym /dev/null

# Done
WORKDIR ${IVM_APPS_DIR}
RUN echo PATH=$PATH >> ${ROOT_DIR}/.bashrc
RUN echo "alias run-ivm-apps='mkdir -p \$IVM_APPS_DIR/out; \$IVM_YAFIE_DIR/ivm64-emu -i \$IVM_TESTDATA_DIR/reel/png -o \$IVM_APPS_DIR/out \$IVM_APPS_DIR/build/ivm/initial-program/ivmip.b'" >> ${ROOT_DIR}/.bashrc
