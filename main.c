// Jump to a program stored in RAM

// The linker will point this to the section reserved for the program
// Right now this section is just a program in flash, but in the future it'll
// be a dynamically loaded program in SRAM
extern void _program();

int main(void)
{
    _program();
    return 0;
}

