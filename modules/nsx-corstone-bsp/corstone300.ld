/* SPDX-License-Identifier: BSD-3-Clause */
/* Secure TCM aliases on SSE-300, both TCMs configured to 512 KiB. */
ENTRY(Reset_Handler)
MEMORY {
    ITCM (rx) : ORIGIN = 0x10000000, LENGTH = 512K
    DTCM (rw) : ORIGIN = 0x30000000, LENGTH = 512K
}
PHDRS {
    code PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}
SECTIONS {
    .text : {
        KEEP(*(.vectors))
        *(.text*) *(.rodata*)
        KEEP(*(.init)) KEEP(*(.fini))
        . = ALIGN(4);
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array*))
        PROVIDE_HIDDEN(__preinit_array_end = .);
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*))) KEEP(*(.init_array))
        PROVIDE_HIDDEN(__init_array_end = .);
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*))) KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > ITCM :code
    .ARM.extab : { *(.ARM.extab*) } > ITCM :code
    .ARM.exidx : { __exidx_start = .; *(.ARM.exidx*) __exidx_end = .; } > ITCM :code
    .copy.table : ALIGN(4) {
        __copy_table_start__ = .;
        LONG(LOADADDR(.data)) LONG(ADDR(.data)) LONG(SIZEOF(.data) / 4)
        __copy_table_end__ = .;
    } > ITCM :code
    .zero.table : ALIGN(4) {
        __zero_table_start__ = .;
        LONG(ADDR(.bss)) LONG(SIZEOF(.bss) / 4)
        __zero_table_end__ = .;
    } > ITCM :code
    .data : ALIGN(8) { *(.data*) . = ALIGN(4); } > DTCM AT> ITCM :data
    .bss (NOLOAD) : ALIGN(8) {
        __bss_start__ = .; *(.bss*) *(COMMON) . = ALIGN(8); __bss_end__ = .;
    } > DTCM :NONE
    __end__ = .;
    PROVIDE(end = .);
    PROVIDE(_end = .);
    __StackTop = ORIGIN(DTCM) + LENGTH(DTCM);
    __StackLimit = __StackTop - 32K;
    __HeapLimit = __StackLimit;
    PROVIDE(__stack = __StackTop);
    ASSERT(__end__ <= __StackLimit, "Corstone-300 DTCM overlaps reserved stack")
}
