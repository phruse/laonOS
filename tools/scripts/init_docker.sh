#
# SPDX-License-Identifier: MIT
#

docker build --platform linux/amd64 -t phruse/build-space:0.1 .
docker push phruse/build-space:0.1
