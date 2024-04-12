#
# SPDX-License-Identifier: MIT
#

#export ADD_QEMU="-s -S"
export RUN_PATH="../../bin/debug/laonOS.iso"

$(
  cd "$(dirname "$0")"
  pwd -P
)/qemu_run.sh
