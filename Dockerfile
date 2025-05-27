FROM ubuntu:22.04

USER root

RUN apt-get update && apt-get install -y \
    cmake \
    wget \
    unzip \
    build-essential \
    openjdk-17-jdk

RUN java -version

RUN mkdir -p /root/antlr4-install \
    && cd /root/antlr4-install \
    && wget https://www.antlr.org/download/antlr-4.13.2-complete.jar \
    && wget https://www.antlr.org/download/antlr4-cpp-runtime-4.13.2-source.zip \
    && unzip antlr4-cpp-runtime-4.13.2-source.zip \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX:PATH=/root/antlr4-install .. \
    && make -j8 \
    && make install

ENTRYPOINT [ "bash" ]
