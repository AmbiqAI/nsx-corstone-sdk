/* SPDX-License-Identifier: BSD-3-Clause */
ENTRY(Reset_Handler)
PHDRS {
    vectors PT_LOAD FLAGS(5);
    model PT_LOAD FLAGS(4);
    code PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}
SECTIONS {
    .vectors : { KEEP(*(.vectors)) } > BOOT :vectors
    .text : {
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
    } > CODE :code
    .ARM.extab : { *(.ARM.extab*) } > CODE :code
    .ARM.exidx : { __exidx_start = .; *(.ARM.exidx*) __exidx_end = .; } > CODE :code
    .copy.table : ALIGN(4) {
        __copy_table_start__ = .;
        LONG(LOADADDR(.data)) LONG(ADDR(.data)) LONG(SIZEOF(.data) / 4)
        __copy_table_end__ = .;
    } > CODE :code
    .zero.table : ALIGN(4) {
        __zero_table_start__ = .;
        LONG(ADDR(.bss)) LONG(SIZEOF(.bss) / 4)
        __zero_table_end__ = .;
    } > CODE :code
    .data : ALIGN(8) { *(.data*) . = ALIGN(4); } > DATA AT> CODE :data
    .bss (NOLOAD) : ALIGN(8) {
        __bss_start__ = .; *(.bss*) *(COMMON) . = ALIGN(8); __bss_end__ = .;
    } > DATA :NONE
    __end__ = .;
    PROVIDE(end = .);
    PROVIDE(_end = .);
    __StackTop = ORIGIN(DATA) + LENGTH(DATA);
    __StackLimit = __StackTop - 32K;
    __HeapLimit = __StackLimit;
    PROVIDE(__stack = __StackTop);
    .npu_model : ALIGN(32) { KEEP(*(.npu_model*)) } > DDR :model
    .npu_arena (NOLOAD) : ALIGN(32) { *(.npu_arena*) } > NPU_SRAM :NONE
    ASSERT(__end__ <= __StackLimit, "Corstone data overlaps reserved stack")
}
