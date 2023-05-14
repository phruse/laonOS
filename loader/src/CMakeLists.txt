include(src/platform/CMakeLists.txt)

set(LOADER_SRC
        src/init.c
        src/elf64.c
        ${LOADER_SRC_PLATFORM}
        )
