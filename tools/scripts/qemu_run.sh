#
# SPDX-License-Identifier: MIT
#

function cleanup {
  kill -9 $!
}

cd $(
  cd "$(dirname "$0")"
  pwd -P
)

if [[ -f $RUN_PATH ]]; then
  :
elif [[ -f "../../bin/release/laonOS.iso" ]]; then
  RUN_PATH="../../bin/release/laonOS.iso"
else
  echo "error: Build should be first."
  exit 1
fi

trap cleanup EXIT
qemu-system-x86_64 \
  -cpu qemu64,+la57 \
  -smp 4 \
  -m 6G \
  -name laonOS \
  -nographic \
  -drive index=0,media=disk,format=raw,file=$RUN_PATH \
  $ADD_QEMU
