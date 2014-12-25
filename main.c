// Jump to a program stored in RAM

// The linker will point this to the section reserved for the program.
// Right now this section is loaded from flash to SRAM by the reset ISR.

extern void _program();

int main(void)
{
    _program();
    return 0;
}

