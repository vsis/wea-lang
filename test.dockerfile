# Build
#   $ docker build -t wea-test -f test.dockerfile .
# Run
#   $ docker run --rm -ti -v $(pwd)/src:/home/wea/src wea-test make tests

FROM ubuntu:21.10

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y build-essential libcriterion-dev gdb gdbserver gcovr splint splint-data && \
    useradd -m -U -s /bin/bash wea

USER wea

WORKDIR /home/wea/src

CMD make check
