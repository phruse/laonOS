# laonOS - in progress
[![License: MIT](https://shields.io/badge/License-MIT-green?style=flat-square)](https://opensource.org/licenses/MIT)
[![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/phruse/laonOS/release-build.yml?style=flat-square)](https://github.com/phruse/laonOS/releases)

> [!CAUTION]
> Do not use for serious work. Security has been deliberately ignored because of the speed of development.

It is a multiplatform 64bit operating system for the research. Currently, OS can boot with x86-64, AArch64 ISA.

![structure](/docs/images/main_structure.jpg)

The core of the OS is the JIT (Just In Time) system, like an eBPF.
However, unlike an eBPF, this OS's nearly systems are run in the JIT compiler above the very lightweight exokernel that abstracts hardware devices.
In particular, the JIT compiler can transparently offload code to other connected devices using mesh networking. This feature is useful in the embedded cases.
If an application needs performance, it can interact directly with the device. Otherwise, it can be built once, run anywhere, and offloaded.

If you are interested in this OS, you can read more in [this documentation](docs.laon.io).

## 👀 Quick Start
This tutorial provides how to build the laonOS. When you finish this tutorial, you can boot the laonOS.

### Build (with Docker)
This build requires an additional dependency on Docker. I used Docker so that we can build across all major platforms.

First, we initialize the container with `phruse/build-space:0.1`.
```shell
docker run --restart always --name build-space -dt phruse/build-space:0.1
docker exec -it build-space bash
```

Next, you should clone this repository. And, just use the cmake workflow. Done! You can find the OS image in `bin/release/laonOS.iso`.
```shell
git clone -b stable https://github.com/phruse/laonOS.git
cd laonOS
cmake --workflow --preset release
```

If you want to build OS without Docker, `/tools/scripts/init_{platform}.sh` can help you! You may need to adjust some options. I recommend to read `cmake/Tools.cmake`.
### Run in QEMU
The laonOS had been tested in real machine, but we'll use QEMU for convenience. The loader uses multiboot2, so you can easily install real machine. If you hate virtual machine, try it!

Now run this script. If you use low memory system, you should change `-m 6G` option.
```shell
./tools/scripts/qemu_run.sh
```

You can find more [tutorials here]()!

## 🧰 Build Dependencies
- CMake (`cmake` + `ninja` or `make`)
- LLVM ^17 (`clang`, `ld`, `llvm-objcopy`)
- GNU GRUB 2 (`grub-mkrescue`)

Read more `cmake/Tools.cmake`

## ✍️ Author
- name: Taeyeon Lee (phruse)
- mail: phruse7@gmail.com
- blog: https://phruse.com