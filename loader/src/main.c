// SPDX-License-Identifier: MIT

#include "main.h"

#include "multiboot.h"
#include "stdio.h"
#include "utilities.h"

#include "info.h"
#include "init.h"
#include "module.h"
#include "platform.h"

#ifdef TEST_ENABLE
#include "test.h"
#endif

#define STR_CHECK "Bootloader check"
#define STR_MB "Multiboot2 init"

#define STR_PRE_INIT "Set GDT"
#define STR_POST_INIT "Jump to kernel!"

_Noreturn void main(uint32_t magic_value, uintptr_t info_address) {
  init_screen();

#ifdef TEST_ENABLE
  TEST_START;
#endif

  printf("> laonOS %s loader\n", OS_VERSION);

  module_t modules[OS_MODULE_MAX + 1]; // 0 is kernel module

  ERROR_BOUNDARY(check_bootloader(magic_value), STR_CHECK)

  ERROR_BOUNDARY(platform_pre_init(), STR_PRE_INIT)
  ERROR_BOUNDARY(multiboot2_init(info_address, modules), STR_MB)
  ERROR_BOUNDARY(platform_post_init(modules), STR_POST_INIT)

  while (true)
    ;
}
