// © 2021 Qualcomm Innovation Center, Inc. All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hypconstants.h>

#include <asm/asm_defs.inc>

const64 soc_qemu_hyp_arg0

	.section .text.boot
function __entry_el2, section=nosection
	// Enable I-cache
	abs64	x10, 0x30401800
	msr	SCTLR_EL2, x10
	adrl	x1, soc_qemu_hyp_arg0
	str	x0, [x1]

	// Get a 256-bit random seed into x0..x3
local get_x0:
	mrs	x0, RNDRRS
	b.eq	LOCAL(get_x0)
local get_x1:
	mrs	x1, RNDR
	b.eq	LOCAL(get_x1)
local get_x2:
	mrs	x2, RNDR
	b.eq	LOCAL(get_x2)
local get_x3:
	mrs	x3, RNDR
	b.eq	LOCAL(get_x3)

	mrs	x4, MPIDR_EL1
	ubfx	x4, x4, MPIDR_EL1_AFF0_SHIFT, MPIDR_EL1_AFF0_BITS

	b	aarch64_init

local prng_fail:
	wfe
	b	LOCAL(prng_fail)
function_end __entry_el2

function soc_qemu_entry_cold_secondary
	mrs	x1, MPIDR_EL1
	ubfx	x1, x1, MPIDR_EL1_AFF0_SHIFT, MPIDR_EL1_AFF0_BITS
	b	aarch64_secondary_init
function_end soc_qemu_entry_cold_secondary

function soc_qemu_entry_warm
	b	aarch64_warm_init
function_end soc_qemu_entry_warm
