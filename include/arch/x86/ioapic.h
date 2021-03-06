/*
 * Copyright © 2020 Amazon.com, Inc. or its affiliates.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef KTF_IOAPIC_H
#define KTF_IOAPIC_H

#include <ktf.h>
#include <lib.h>
#include <list.h>

/* IOAPIC ID is 4 bits long */
#define MAX_IOAPICS (1 << 4)

/* IOAPIC register offsets */
#define IOAPIC_ID          0x00
#define IOAPIC_VERSION     0x01
#define IOAPIC_ARB         0x02
#define IOAPIC_REDIRTBL(n) (0x10 + 2 * (n))

/* MMIO access registers */
#define IOAPIC_REGSEL 0x00
#define IOAPIC_IOWIN  0x10

#define IOAPIC_DEST_ID_UNKNOWN ((uint8_t) 0xFF)

#ifndef __ASSEMBLY__

enum irq_override_polarity {
    IOAPIC_IRQ_OVR_POLARITY_BS = 0x00,
    IOAPIC_IRQ_OVR_POLARITY_AH = 0x01,
    IOAPIC_IRQ_OVR_POLARITY_RSVD = 0x02,
    IOAPIC_IRQ_OVR_POLARITY_AL = 0x03,
};
typedef enum irq_override_polarity irq_override_polarity_t;

enum irq_override_trigger_mode {
    IOAPIC_IRQ_OVR_TRIGGER_BS = 0x00,
    IOAPIC_IRQ_OVR_TRIGGER_ET = 0x01,
    IOAPIC_IRQ_OVR_TRIGGER_RSVD = 0x02,
    IOAPIC_IRQ_OVR_TRIGGER_LT = 0x03,
};
typedef enum irq_override_trigger_mode irq_override_trigger_mode_t;

struct irq_override {
    list_head_t list;
    uint8_t type;
    uint32_t src;
    uint32_t dst;
    uint8_t dst_id; /* IOAPIC or LAPIC */
    irq_override_polarity_t polarity;
    irq_override_trigger_mode_t trigger_mode;
};
typedef struct irq_override irq_override_t;

struct bus {
    list_head_t list;
    uint8_t id;
    char name[6];
    list_head_t irq_overrides;
};
typedef struct bus bus_t;

struct ioapic {
    uint8_t id;
    uint8_t version;
    bool enabled;
    uint64_t base_address;
    void *virt_address;
    uint32_t gsi_base;
    unsigned nr_entries;
};
typedef struct ioapic ioapic_t;

union ioapic_id {
    struct {
        /* clang-format off */
        uint32_t rsvd1   : 24,
                 apic_id :  4,
                 rsvd2   :  4;
        /* clang-format on */
    } __packed;
    uint32_t reg;
};
typedef union ioapic_id ioapic_id_t;

union ioapic_version {
    struct {
        /* clang-format off */
        uint32_t version         : 8,
                 rsvd1           : 8,
                 max_redir_entry : 8,
                 rsvd2           : 8;
        /* clang-format on */
    } __packed;
    uint32_t reg;
};
typedef union ioapic_version ioapic_version_t;

union ioapic_arb {
    struct {
        /* clang-format off */
        uint32_t rsvd1   : 24,
                 apic_id :  4,
                 rsvd2   :  4;
        /* clang-format on */
    } __packed;
    uint32_t reg;
};
typedef union ioapic_arb ioapic_arb_t;

enum ioapic_irq_type {
    IOAPIC_IRQ_TYPE_INT = 0x00,
    IOAPIC_IRQ_TYPE_NMI = 0x01,
    IOAPIC_IRQ_TYPE_SMI = 0x10,
    IOAPIC_IRQ_TYPE_EXTINT = 0x11,
};
typedef enum ioapic_irq_type ioapic_irq_type_t;

enum ioapic_delivery_mode {
    IOAPIC_DELIVERY_MODE_FIXED = 0x00,
    IOAPIC_DELIVERY_MODE_LOWPRIO = 0x01,
    IOAPIC_DELIVERY_MODE_SMI = 0x02,
    IOAPIC_DELIVERY_MODE_NMI = 0x04,
    IOAPIC_DELIVERY_MODE_INIT = 0x05,
    IOAPIC_DELIVERY_MODE_EXTINT = 0x07,
};
typedef enum ioapic_deliver_mode ioapic_delivery_mode_t;

enum ioapic_dest_mode {
    IOAPIC_DEST_MODE_PHYSICAL = 0x00,
    IOAPIC_DEST_MODE_LOGICAL = 0x01,
};
typedef enum ioapic_dest_mode ioapic_dest_mode_t;

enum ioapic_delivery_status {
    IOAPIC_DELIVERY_STATUS_IDLE = 0x00,
    IOAPIC_DELIVERY_STATUS_PENDING = 0x01,
};
typedef enum ioapic_delivery_status ioapic_delivery_status_t;

enum ioapic_trigger_mode {
    IOAPIC_TRIGGER_MODE_EDGE = 0x00,
    IOAPIC_TRIGGER_MODE_LEVEL = 0x01,
};
typedef enum ioapic_trigger_mode ioapic_trigger_mode_t;

enum ioapic_polarity {
    IOAPIC_POLARITY_AH = 0x00,
    IOAPIC_POLARITY_AL = 0x01,
};
typedef enum ioapic_polarity ioapic_polarity_t;

enum ioapic_int_mask {
    IOAPIC_INT_UNMASK = 0x00,
    IOAPIC_INT_MASK = 0x01,
};
typedef enum ioapic_int_mask ioapic_int_mask_t;

union ioapic_redirtbl_entry {
    struct {
        /* clang-format off */
        uint64_t vector       :  8,
                 deliv_mode   :  3,
                 dest_mode    :  1,
                 deliv_status :  1,
                 polarity     :  1,
                 remote_irr   :  1,
                 trigger_mode :  1,
                 int_mask     :  1,
                 rsvd         : 39,
                 destination  :  8;
        /* clang-format on */
    } __packed;
    struct {
        uint32_t low;
        uint32_t high;
    } __packed;
    uint64_t reg;
};
typedef union ioapic_redirtbl_entry ioapic_redirtbl_entry_t;

/* External declarations */

extern bus_t *add_system_bus(uint8_t id, const char *name, size_t namelen);
extern int add_system_bus_irq_override(uint8_t bus_id, irq_override_t *irq_override);
extern irq_override_t *get_system_isa_bus_irq(uint8_t irq_type, uint32_t irq_src);
extern irq_override_t *get_system_pci_bus_irq(uint8_t irq_type, uint32_t irq_src);

extern void init_ioapic(void);
extern ioapic_t *get_ioapic(uint8_t id);
extern ioapic_t *add_ioapic(uint8_t id, uint8_t version, bool enabled,
                            uint64_t base_address, uint32_t gsi_base);
extern int get_ioapic_redirtbl_entry(ioapic_t *ioapic, unsigned n,
                                     ioapic_redirtbl_entry_t *entry);
extern int set_ioapic_redirtbl_entry(ioapic_t *ioapic, unsigned n,
                                     ioapic_redirtbl_entry_t *entry);
extern void set_ioapic_irq_mask(ioapic_t *ioapic, unsigned irq, ioapic_int_mask_t mask);
extern void configure_isa_irq(unsigned irq_src, uint8_t vector,
                              ioapic_dest_mode_t dst_mode, uint8_t dst_ids);

/* Static declarations */

static inline uint32_t ioapic_read32(ioapic_t *ioapic, uint8_t reg) {
    *(volatile uint8_t *) (ioapic->virt_address + IOAPIC_REGSEL) = reg;
    return *(volatile uint32_t *) (ioapic->virt_address + IOAPIC_IOWIN);
}

static inline void ioapic_write32(ioapic_t *ioapic, uint8_t reg, uint32_t value) {
    *(volatile uint8_t *) (ioapic->virt_address + IOAPIC_REGSEL) = reg;
    *(volatile uint32_t *) (ioapic->virt_address + IOAPIC_IOWIN) = value;
}

#endif /* __ASSEMBLY__ */

#endif /* KTF_IOAPIC_H */
