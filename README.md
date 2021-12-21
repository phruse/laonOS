# laonOS : OS for study (in progress)

[![License: MIT](https://shields.io/badge/License-MIT-green?style=flat-square)](https://opensource.org/licenses/MIT)

It is an operating system for study.

## Quick Overview

## Feature
### kernel
* ☑️ Exokernel
* ☑️ Higher half kernel load

### libOS
* ❌ Coroutines supported by the libOS level

<sup>✅</sup>:completion, <sup>☑️</sup>: in progress, <sup>❌</sup>: planned

## Build Dependencies
* CMake (`cmake` + `ninja` or `make`)
* LLVM (`clang`, `ld`, `llvm-objcopy-13`)
* NASM (`nasm`)
* GNU GRUB 2 (`grub-mkrescue`)

Read more `cmake/Tools.cmake`

## References
This project would not have been possible without the help of these papers.

* Dawson R. Engler, "The Exokernel Operating System Architecture", October 1998.