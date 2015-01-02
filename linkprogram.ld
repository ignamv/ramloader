/* See section 2.4 "Memory Model", Stellaris LM4F120H5QR Datasheet */
MEMORY
{
    FLASH (RX): ORIGIN = 0x0, LENGTH = 0x40000
    SRAM1 (RW): ORIGIN = 0x20000000, LENGTH = 0x4000
    SRAM2 (RW): ORIGIN = 0x20004000, LENGTH = 0x4000
}

/* 
 * Good explanation of how this builds a ROM image in the binutils documentation
 * http://sourceware.org/binutils/docs-2.23.1/ld/Output-Section-LMA.html
 */

SECTIONS
{
    .text : {
        /* Prevent garbage collection of the vector table */
        KEEP(*(.vector_table))
        KEEP(*(.isr_vector))
        *(.text.*)
    } > SRAM2
    .rodata : {
        *(.rodata*)
    } > SRAM2
    /* Initialized variables */
    .data : {
        *(.data*)
    } > SRAM2
    .bss : {
        *(.bss*)
    } > SRAM2
}

ENTRY ( main )


