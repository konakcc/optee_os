/*
 * Copyright (c) 2016, Linaro Limited
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef CORE_MEMPROT_H
#define CORE_MEMPROT_H

#include <types_ext.h>
#include <kernel/tee_common_unpg.h>
#include <mm/core_mmu.h>

/*
 * "pbuf_is" support.
 *
 * core_vbuf_is()/core_pbuf_is() can be used to check if a teecore mapped
 * virtual address or a physical address is "Secure", "Unsecure", "external
 * RAM" and some other fancy attributes.
 *
 * DO NOT use 'buf_is(Secure, buffer)==false' as a assumption that buffer is
 * UnSecured ! This is NOT a valid asumption ! A buffer is certified UnSecured
 * only if 'buf_is(UnSecure, buffer)==true'.
 */

/* memory atttributes */
enum buf_is_attr {
	CORE_MEM_SEC,
	CORE_MEM_NON_SEC,
	CORE_MEM_TEE_RAM,
	CORE_MEM_TA_RAM,
	CORE_MEM_NSEC_SHM,
	CORE_MEM_MULTPURPOSE,
	CORE_MEM_EXTRAM,
	CORE_MEM_INTRAM,
	CORE_MEM_CACHED,
};

/* redirect legacy tee_vbuf_is() and tee_pbuf_is() to our routines */
#define tee_pbuf_is     core_pbuf_is
#define tee_vbuf_is     core_vbuf_is

/* Convenience macros */
#define tee_pbuf_is_non_sec(buf, len) \
		core_pbuf_is(CORE_MEM_NON_SEC, (tee_paddr_t)(buf), (len))

#define tee_pbuf_is_sec(buf, len) \
		core_pbuf_is(CORE_MEM_SEC, (tee_paddr_t)(buf), (len))

#define tee_vbuf_is_non_sec(buf, len) \
		core_vbuf_is(CORE_MEM_NON_SEC, (void *)(buf), (len))

#define tee_vbuf_is_sec(buf, len) \
		core_vbuf_is(CORE_MEM_SEC, (void *)(buf), (len))

/*
 * This function return true if the buf complies with supplied flags.
 * If this function returns false buf doesn't comply with supplied flags
 * or something went wrong.
 *
 * Note that returning false doesn't guarantee that buf complies with
 * the complement of the supplied flags.
 */
bool core_pbuf_is(uint32_t flags, tee_paddr_t pbuf, size_t len);

/*
 * Translates the supplied virtual address to a physical address and uses
 * tee_phys_buf_is() to check the compliance of the buffer.
 */
bool core_vbuf_is(uint32_t flags, const void *vbuf, size_t len);

/*
 * Translate physical address to virtual address using specified mapping
 * Returns NULL on failure or a valid virtual address on success.
 */
void *phys_to_virt(paddr_t pa, enum teecore_memtypes m);

/*
 * Translate virtual address to physical address
 * Returns 0 on failure or a valid physical address on success.
 */
paddr_t virt_to_phys(void *va);

#endif /* CORE_MEMPROT_H */
