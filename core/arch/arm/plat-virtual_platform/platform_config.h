// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 */

#ifndef PLATFORM_CONFIG_H
#define PLATFORM_CONFIG_H

#include <mm/generic_ram_layout.h>

/* Make stacks aligned to data cache line length */
#define STACK_ALIGNMENT		64

#ifdef ARM64
#ifdef CFG_WITH_PAGER
#error "Pager not supported for ARM64"
#endif
#endif /* ARM64 */

#ifdef PLATFORM_FLAVOR_a55
#define PLAT_A55_NIC_CLK		24000000
#endif

/* 16550 UART */
#ifdef PLATFORM_FLAVOR_a55
#define UART0_BASE			0x40000000
#endif

#define PLAT_A55_BOOT_UART_BASE	UART0_BASE
#define PLAT_A55_BOOT_UART_SIZE	0x1000
#define PLAT_A55_CONSOLE_BAUDRATE	115200
#define PLAT_A55_BOOT_UART_SRC_CLK   PLAT_A55_NIC_CLK

#define GICD_BASE						0x60000000
#define GICD_SIZE						0x00010000


#endif /* PLATFORM_CONFIG_H */
