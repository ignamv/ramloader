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
        *(.text.*)
    } > FLASH
    .rodata : {
        *(.rodata*)
        _etext = .;
    } > FLASH
    /* Initialized variables
     * Their initial values are stored in Flash, and the startup routine must
     * copy them to SRAM */
    .data : {
        _data = .;
        *(.data*)
        _edata = .;
    } > SRAM1 AT> FLASH /* Set the VMA (runtime address) in SRAM 
                                   LMA (load address) in FLASH */
    .bss : {
        _bss = .;
        *(.bss*)
        _ebss = .;
    } > SRAM1
    .sarasa : {
        /* To know where to point the vector table */
        _sram2 = .;
    } > SRAM2 AT> FLASH
}

ENTRY ( resetISR )

