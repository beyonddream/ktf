#ifndef SETUP_SETUP_H
#define SETUP_SETUP_H

#define KERN_STACK_SIZE (3 * PAGE_SIZE)

#ifndef __ASSEMBLY__
#include <page.h>

extern uint8_t kernel_stack[KERN_STACK_SIZE + 2 * PAGE_SIZE];
extern uint8_t user_stack[PAGE_SIZE];

#define GET_KERN_STACK()    (&kernel_stack[KERN_STACK_SIZE])
#define GET_KERN_EX_STACK() (&kernel_stack[KERN_STACK_SIZE + PAGE_SIZE])
#define GET_KERN_EM_STACK() (&kernel_stack[KERN_STACK_SIZE + PAGE_SIZE])

extern char kernel_cmdline[PAGE_SIZE];

extern unsigned long __start_text[], __end_text[];
extern unsigned long __start_data[], __end_data[];
extern unsigned long __start_bss[],  __end_bss[];
extern unsigned long __start_rodata[], __end_rodata[];

extern unsigned long __start_text_user[], __end_text_user[];
extern unsigned long __start_data_user[], __end_data_user[];
extern unsigned long __start_bss_user[], __end_bss_user[];

extern unsigned long __start_text_init[], __end_text_init[];
extern unsigned long __start_data_init[], __end_data_init[];
extern unsigned long __start_bss_init[], __end_bss_init[];

struct addr_range {
    const char *name;
    unsigned long base;
    unsigned long flags;
    void *from;
    void *to;
};
typedef struct addr_range addr_range_t;

#define KERN_ADDR_RANGES_NUM 4
extern addr_range_t kern_addr_ranges[];
#define USER_ADDR_RANGES_NUM 3
extern addr_range_t user_addr_ranges[];
#define INIT_ADDR_RANGES_NUM 3
extern addr_range_t init_addr_ranges[];

extern void display_memory_map(void);
extern void dump_pagetables(void);

#endif /* __ASSEMBLY__ */

#endif /* SETUP_SETUP_H */
