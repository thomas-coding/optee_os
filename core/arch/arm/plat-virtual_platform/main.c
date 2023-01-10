// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (C) 2020 VeriSilicon Holdings Co., Ltd.
 */

#include <console.h>
#include <drivers/serial8250_uart.h>
#include <drivers/gic.h>
#include <kernel/boot.h>
#include <kernel/panic.h>
#include <mm/core_memprot.h>
#include <mm/tee_pager.h>
#include <platform_config.h>
#include <stdint.h>

register_phys_mem_pgdir(MEM_AREA_IO_NSEC,
			PLAT_A55_BOOT_UART_BASE, SERIAL8250_UART_REG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC, GICD_BASE, GICD_SIZE);

static struct serial8250_uart_data console_data;

static struct gic_data a55_gic_data;

void console_init(void)
{
	serial8250_uart_init(&console_data, PLAT_A55_BOOT_UART_BASE,
				PLAT_A55_BOOT_UART_SRC_CLK,
				PLAT_A55_CONSOLE_BAUDRATE);
	register_serial_console(&console_data.chip);
}

void main_init_gic(void)
{
	/* On ARMv8, GIC configuration is initialized in ARM-TF */
	gic_init_base_addr(&a55_gic_data, 0, GICD_BASE);

	itr_init(&a55_gic_data.chip);
}

void itr_core_handler(void)
{
	gic_it_handle(&a55_gic_data);
}
