FROM fedora:28

RUN dnf update -y --refresh
RUN dnf install -y make gcc gcc-c++ cmake git
RUN dnf install -y zlib-devel openssl-devel boost-devel cpprest-devel

ENV USER=nimiq
RUN useradd -m ${USER}

ENV DATA_PATH=/nimiq
RUN mkdir ${DATA_PATH} && chown ${USER}:root ${DATA_PATH}
WORKDIR ${DATA_PATH}

COPY --chown=nimiq:root . miner


USER ${USER}

RUN cd miner && mkdir build && cd build && cmake .. && make
ENTRYPOINT [ "./miner/build/nimiq-miner" ]
