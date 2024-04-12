#
# SPDX-License-Identifier: MIT
#

FROM ubuntu:latest

RUN mkdir -p /scripts
COPY init_Debian.sh /scripts/init.sh
WORKDIR /scripts
RUN chmod +x init.sh
RUN ./init.sh
