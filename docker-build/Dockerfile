FROM docker.io/ubuntu:focal

ARG gnu_arm_toolchain=https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2019q3/RC1.1/gcc-arm-none-eabi-8-2019-q3-update-linux.tar.bz2?revision=c34d758a-be0c-476e-a2de-af8c6e16a8a2?product=GNU%20Arm%20Embedded%20Toolchain%20Downloads,64-bit,,Linux,8-2019-q3-update
ARG nrf51_sdk=https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v12.x.x/nRF5_SDK_12.3.0_d7731ad.zip
MAINTAINER josnelihurt rodriguez <josnelihurt@gmail.com>

RUN apt-get update
RUN apt-get install -y zip unzip patch make wget
WORKDIR /opt
#ADD $gnu_arm_toolchain gcc-arm-none-eabi.tar.bz2
RUN wget $gnu_arm_toolchain -O gcc-arm-none-eabi.tar.bz2
RUN tar -xf gcc-arm-none-eabi.tar.bz2
RUN rm gcc-arm-none-eabi.tar.bz2
RUN mv gcc-arm-none-eabi-* gcc-arm-none-eabi

#ADD $nrf51_sdk nrf51_sdk.zip
RUN wget $nrf51_sdk -O nrf51_sdk.zip
RUN unzip nrf51_sdk.zip
RUN rm nrf51_sdk.zip
RUN mv n* nRF5_SDK_12/
RUN rm nRF5_SDK_12/components/toolchain/gcc/Makefile.posix
COPY Makefile.posix nRF5_SDK_12/components/toolchain/gcc/Makefile.posix

#This is just to verify that the build system is complete set
ARG J="12"
ARG OPT=''
WORKDIR /opt/nRF5_SDK_12/examples/peripheral/blinky/pca10028/s130/armgcc
RUN make -j$J
RUN mkdir /src 
WORKDIR /src
ENTRYPOINT make -j$J $OPT