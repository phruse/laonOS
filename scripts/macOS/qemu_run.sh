function cleanup {
  kill -9 $!
}

cd $(cd "$(dirname "$0")"; pwd -P)

if [[ -f "../../bin/release/laonOS.iso" && $DEBUG != 1 ]] ; then
  ISO_PATH="../../bin/release/laonOS.iso"
elif [[ -f "../../bin/debug/laonOS.iso" ]] ; then
  ISO_PATH="../../bin/debug/laonOS.iso"
else
  echo "error: Build should be first."
  exit 1
fi

trap cleanup EXIT
qemu-system-x86_64 \
  -cpu qemu64,+la57 \
  -smp 4 \
  -m 6G \
  -serial stdio \
  -name laonOS \
  $ADD_QEMU \
  -drive index=0,media=disk,format=raw,file=$ISO_PATH
