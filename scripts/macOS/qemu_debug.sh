export ADD_QEMU="-s -S" DEBUG=1
$(cd "$(dirname "$0")"; pwd -P)/qemu_run.sh
