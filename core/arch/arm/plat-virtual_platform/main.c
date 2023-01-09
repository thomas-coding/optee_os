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
register_phys_mem_pgdir(MEM_AREA_IO_SEC, GIC_BASE, GIC_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			PLAT_A55_CRM_BASE, PLAT_A55_CRM_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			ISPV_AXIFE_CONFIG_BASE, ISPV_AXIFE_CONFIG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			ISPI_AXIFE_CONFIG_BASE, ISPI_AXIFE_CONFIG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			VIP_CONFIG_BASE, VIP_CONFIG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			VEC_CONFIG_BASE, VEC_CONFIG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			VDC_CONFIG_BASE, VDC_CONFIG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			DSS_CONFIG_BASE, DSS_CONFIG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			PERI_SYS_CONFIG_BASE, PERI_SYS_CONFIG_SIZE);
register_phys_mem_pgdir(MEM_AREA_IO_SEC,
			GPU_SYS_CSR_BASE, GPU_SYS_CSR_SIZE);

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
	vaddr_t gicc_base = 0;
	vaddr_t gicd_base = 0;

	assert(cpu_mmu_enabled());

#ifndef CFG_ARM_GICV3
	gicc_base = (vaddr_t)phys_to_virt(GIC_BASE + GICC_OFFSET,
		MEM_AREA_IO_SEC, GIC_SIZE);
	if (!gicc_base)
		panic();
#endif
	gicd_base = (vaddr_t)phys_to_virt(GIC_BASE + GICD_OFFSET,
		MEM_AREA_IO_SEC, GIC_SIZE);
	if (!gicd_base)
		panic();

	gic_init_base_addr(&a55_gic_data, gicc_base, gicd_base);

	itr_init(&a55_gic_data.chip);
}

void itr_core_handler(void)
{
	gic_it_handle(&a55_gic_data);
}
