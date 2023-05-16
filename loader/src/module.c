// SPDX-License-Identifier: MIT

#include "module.h"

void multiboot_converter(const multiboot_module_t *multiboot_module,
                         module_t *module) {
  module->module_start = multiboot_module->module_start;
  module->module_end = multiboot_module->module_end;
}

bool is_module(const module_t *module) {
  return module->module_start != 0 && module->module_end != 0;
}
