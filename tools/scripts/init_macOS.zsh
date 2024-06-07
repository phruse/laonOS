#
# SPDX-License-Identifier: MIT
#

# For macOS

echo "Installing build tools..."

LOGDIR=$PWD/init_log.txt

which -s brew > "$LOGDIR" 2>&1
if [[ $? != 0 ]] ; then
    ruby -e "$(curl -fsSL \
    https://raw.githubusercontent.com/Homebrew/install/master/install)" \
    > "$LOGDIR" 2>&1
fi
brew install \
cmake llvm autoconf automake x86_64-elf-gcc qemu ninja coreutils \
> "$LOGDIR" 2>&1

TEMPDIR=$(mktemp -d 2>/dev/null || mktemp -d -t 'laon_os_init_temp')

cd "$TEMPDIR"

echo "Installing compiler-rt..."

echo " - Downloading"
wget https://github.com/llvm/llvm-project/releases/download/llvmorg-13.0.0/\
clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz > "$LOGDIR" 2>&1

if [[ $(gsha256sum clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz ) =
$(echo -e \
"76d0bf002ede7a893f69d9ad2c4e101d15a8f4186fbfe24e74856c8449acd7c1" \
" clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz") ]] ; then
    echo " - Extracting files"
    xz -d clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-16.04.tar.xz > "$LOGDIR" 2>&1
    tar -xf clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-16.04.tar > "$LOGDIR" 2>&1
    echo " - sudo for 'mv /usr/local/lib'"
    sudo mkdir /usr/local/lib/clang
    sudo mv clang+llvm-13.0.0-x86_64-linux-gnu-ubuntu-16.04/\
lib/clang/13.0.0/lib/linux /usr/local/lib/clang
else
    echo "error: checksum not invalid."
fi

echo "Installing grub..."
echo " - Cloning"

git clone git://git.savannah.gnu.org/grub.git > "$LOGDIR" 2>&1
git clone https://github.com/vertis/objconv.git > "$LOGDIR" 2>&1

echo " - Building objconv"
# temp/objconv
cd objconv
g++ -o objconv -O2 src/*.cpp > "$LOGDIR" 2>&1
echo " - sudo for 'mv /usr/local/bin/objconv'"
sudo mv objconv /usr/local/bin/objconv
cd ../

echo " - Bootstrapping"
# temp/grub
cd grub
./bootstrap > "$LOGDIR" 2>&1
./autogen.sh > "$LOGDIR" 2>&1

# temp/grub-build
cd ../
mkdir grub-build > "$LOGDIR" 2>&1
cd grub-build

echo " - Configuring"
../grub/configure --disable-werror TARGET_CC=x86_64-elf-gcc \
TARGET_LAON_OBJCOPY=x86_64-elf-objcopy \
TARGET_STRIP=x86_64-elf-strip \
TARGET_NM=x86_64-elf-nm \
TARGET_RANLIB=x86_64-elf-ranlib \
--target=x86_64-elf > "$LOGDIR" 2>&1

echo " - Making"
make > "$LOGDIR" 2>&1
echo " - Installing"
echo " - sudo for 'make install' to grub2"
sudo make install

rm -rf "$TEMPDIR"

echo "Finish! Next step: run cmake."
