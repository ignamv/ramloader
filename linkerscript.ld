/* See section 2.4 "Memory Model", Stellaris LM4F120H5QR Datasheet */
MEMORY
{
    FLASH (RX): ORIGIN = 0x0, LENGTH = 0x40000
    SRAM (RW): ORIGIN = 0x20000000, LENGTH = 0x8000
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
        _etext = .;
    } > FLASH
    /* Initialized variables
     * Their initial values are stored in Flash, and the startup routine must
     * copy them to SRAM
     * AT sets the LMA in Flash, right after the code */
    .data : AT ( ADDR (.text) + SIZEOF (.text) ) {
        _data = .;
        *(.data*)
        _program = .;
        KEEP(*(.program))
        _edata = .;
    } > SRAM /* Set the VMA (runtime address) in SRAM */
    .bss : {
        _bss = .;
        *(.bss*)
        _ebss = .;
    } > SRAM
}

ENTRY ( resetISR )

