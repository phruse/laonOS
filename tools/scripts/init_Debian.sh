#
# SPDX-License-Identifier: MIT
#

# For Debian (run with sudo)

echo "Installing build tools..."

LOGDIR=$PWD/init_log.txt

apt update >"$LOGDIR" 2>&1
apt install -y \
  qemu python3-pip llvm clang lld xorriso grub2-common mtools ninja-build \
  git >"$LOGDIR" 2>&1
pip install cmake >"$LOGDIR" 2>&1

echo "Finish! Next step: run cmake."
