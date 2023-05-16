// SPDX-License-Identifier: MIT

#include "loader.h"

#include <stdio.h>
#include <utilities.h>

#include "info.h"
#include "init.h"
#include "module.h"
#include "platform.h"

#define STR_CHECK "Bootloader check"
#define STR_MB "Multiboot2 init"

#define STR_PRE_INIT "Set GDT"
#define STR_POST_INIT "Setup long-mode"

_Noreturn void lmain(uint32_t magic_value, uintptr_t info_address) {
  clear_screen();

  printf("> laonOS %s loader\n", OS_VERSION);

  module_t modules[OS_MODULE_MAX + 1]; // 0 is kernel module

  ERROR_BOUNDARY(check_bootloader(magic_value), STR_CHECK)

  ERROR_BOUNDARY(platform_pre_init(), STR_PRE_INIT)
  ERROR_BOUNDARY(multiboot2_init(info_address, modules), STR_MB)
  ERROR_BOUNDARY(platform_post_init(modules), STR_POST_INIT)

  while (true)
    ;
}
