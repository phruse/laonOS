if (NOT DEFINED)
    option(LAON_TEST "test enable" ON)
endif ()

# build platform
set(PLATFORM_LIST x86 arm)

if (NOT DEFINED LAON_PLATFORM)
    set(LAON_PLATFORM x86)
endif ()

# kernel build mode
# PIC      -> position independent binary
# RELOCATE -> relocatable binary
# BUILD    -> build kernel dynamic
set(KERNEL_BUILD_MODE_LIST PIC RELOCATE BUILD)

if (NOT DEFINED LAON_KERNEL_BUILD_MODE)
    set(LAON_KERNEL_BUILD_MODE RELOCATE)
endif ()

#

# mode check

if (NOT LAON_PLATFORM IN_LIST PLATFORM_LIST)
    message(FATAL_ERROR "platform must be one of ${PLATFORM_LIST}")
endif ()

if (NOT LAON_KERNEL_BUILD_MODE IN_LIST KERNEL_BUILD_MODE_LIST)
    message(FATAL_ERROR "kernel build mode must be one of ${KERNEL_BUILD_MODE_LIST}")
endif ()

if (LAON_PLATFORM MATCHES x86)
    set(LAON_LOADER_PLATFORM x86)
    set(LAON_KERNEL_PLATFORM x86_64)
elseif (LAON_PLATFORM MATCHES arm)
    set(LAON_LOADER_PLATFORM armv7a)
    set(LAON_KERNEL_PLATFORM armv7a)
endif ()
